/*
Copyright (C) 2008 by Charles Solar
charlessolar@gmail.com

This file is part of bafprp.

bafprp is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

bafprp is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with bafprp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef _WIN32
#include <windows.h>
#endif

#include <sql.h>
#include <sqlext.h>

#include "mssqloutput.h"
#include "baffile.h"


namespace bafprp
{
	const MSSQL MSSQL::registerThis;

	void MSSQL::error( const IBafRecord* record, const std::string& error )
	{
		LOG_TRACE( "MSSQL::error" );
		checkDB( _errorProperties, true );
		if( !_dbConnected ) 
		{
			Output::setOutputError( "file" );
			LOG_ERROR( "Failed to connect to the database, check your properties, falling back to file output" );

			// Play nice
			Output::outputError( record, error );
			return;
		}

		SQLHSTMT stmt;
		SQLAllocHandle(SQL_HANDLE_STMT, _dbc, &stmt);

		std::ostringstream os;

		os << "INSERT INTO " << _table << " ( ts, error, filename, filepos, type, length, bytes ) VALUES ( ";
		os << "'" << sanitize( NowTime() ) << "', ";
		os << "'" << sanitize( error ) << "', ";
		os << "'" << sanitize( record->getFilename() ) << "', ";
		os << "'" << record->getFilePosition() << "', ";
		os << "'" << sanitize( record->getType() ) << "', ";
		os << "'" << record->getSize() << "', ";
		os << "'" << sanitize( record->getData() ) << "' ";
		os << ")";

		if( SQLExecDirectA(stmt, (SQLCHAR*)os.str().c_str(), SQL_NTS) == SQL_ERROR )
		{
			Output::setOutputError( "file" );
			LOG_ERROR( "Failed to insert error message into database, falling back to file output" );

			Output::outputError( record, error );
		}

		SQLFreeHandle( SQL_HANDLE_STMT, stmt );

		checkDB( _errorProperties, false );
		LOG_TRACE( "/MSSQL::error" );
	}

	void MSSQL::log( LOG_LEVEL level, const std::string& log )
	{
		checkDB( _logProperties, true );

		if( !_dbConnected ) 
		{
			Output::setOutputLog( "file" );
			LOG_ERROR( "Failed to connect to the database, check your properties\n" );

			// Play nice
			Output::outputLog( level, log );
			return;
		}

		SQLHSTMT stmt;
		SQLAllocHandle(SQL_HANDLE_STMT, _dbc, &stmt);

		std::ostringstream os;
		os << "INSERT INTO " << _table << " ( ts, loglevel, msg ) VALUES ( '" << NowTime() << "', '" << level << "', '" << sanitize( log ) << "' )";

		if( SQLExecDirectA(stmt, (SQLCHAR*)os.str().c_str(), SQL_NTS) == SQL_ERROR )
		{
			Output::setOutputLog( "file" );
			LOG_ERROR( "Failed to insert log into database, falling back to file output" );

			Output::outputLog( level, log );
		}

		SQLFreeHandle( SQL_HANDLE_STMT, stmt );
		
		checkDB( _logProperties, false );
	}

	void MSSQL::record( const IBafRecord* record )
	{
		LOG_TRACE( "MSSQL::record" );
		checkDB( _recordProperties, true );
		
		if( !_dbConnected ) 
		{
			Output::setOutputRecord( "file" );
			LOG_ERROR( "Failed to connect to the database, check your properties, falling back to file output" );

			// Play nice
			Output::outputRecord( record );
			return;
		}

		// Query table to find out what the column names are
		SQLHSTMT stmt;
		SQLAllocHandle(SQL_HANDLE_STMT, _dbc, &stmt);

		std::ostringstream os;

		os << "SELECT COLUMN_NAME, DATA_TYPE FROM INFORMATION_SCHEMA.Columns WHERE TABLE_NAME = '" << _table << "'";
		if( SQLExecDirectA(stmt, (SQLCHAR*)os.str().c_str(), SQL_NTS) == SQL_ERROR )
		{
			Output::setOutputError( "file" );
			LOG_ERROR( "Failed to query columns in database, falling back to file output" );
			SQLFreeHandle( SQL_HANDLE_STMT, stmt );
			return;
		}

		std::map<std::string,std::string> columns;

		SQLCHAR columnName[50];
		SQLCHAR columnType[50];
		SQLINTEGER cbData;
		while( SQLFetch(stmt) == SQL_SUCCESS )
		{
		  SQLGetData( stmt, 1, SQL_C_CHAR, columnName, sizeof( columnName ), &cbData );
		  SQLGetData( stmt, 2, SQL_C_CHAR, columnType, sizeof( columnType ), &cbData );
		  columns.insert( std::make_pair( (char*)columnName, (char*)columnType ) );
		}

		SQLFreeHandle( SQL_HANDLE_STMT, stmt );

		// Construct an insert query from the record values corresponding to those names
		// Two part query, see below
		std::ostringstream query1; 
		std::ostringstream query2;

		query1 << "INSERT INTO " << _table << " ( ";
		query2 << "VALUES ( ";
		
		for( std::map<std::string,std::string>::iterator itr = columns.begin(); itr != columns.end(); itr++ )
		{
			if( itr->first == "id" ) continue;  // Skip 'id' field

			// Check for 'special' columns
			if( itr->first == "filename" )
			{
				query1 << "filename, ";
				query2 << "'" << sanitize( record->getFilename() ) << "', ";
				continue;
			}
			if( itr->first == "filepos" )
			{
				query1 << "filepos, ";
				query2 << "'" << record->getFilePosition() << "', ";
				continue;
			}
			if( itr->first == "type" )
			{
				query1 << "type, ";
				query2 << "'" << sanitize( record->getType() ) << "', ";
				continue;
			}
			if( itr->first == "size" )
			{
				query1 << "size, ";
				query2 << "'" << record->getSize() << "', ";
				continue;
			}
			if( itr->first == "crc" )
			{
				query1 << "crc, ";
				query2 << "'" << record->getCRC() << "', ";
				continue;
			}

			if( !record->hasField( itr->first ) ) continue; // Skip fields the record does not have

			// Find field in record (if it exists)
			const IField* field = record->getField( itr->first );

			if( !field ) 
			{
				LOG_WARN( "Failed to retreive field " << itr->first << " from record for sql output" );
				continue;
			}

			if( itr->second == "int" || itr->second == "bigint" )
			{
				query1 << itr->first << ", ";
				query2 << "'" << field->getLong() << "', ";
			}
			else if( itr->second == "decimal" || itr->second == "float" )
			{
				query1 << itr->first << ", ";
				query2 << "'" << field->getFloat() << "', ";
			}
			else
			{
				query1 << itr->first << ", ";
				query2 << "'" << sanitize( field->getString() ) << "', ";
			}
		}

		// Make the full query, trimming off the surplus commas
		std::string fullQuery = query1.str().substr( 0, query1.str().length() - 2 ) + ") " + query2.str().substr( 0, query2.str().length() - 2 ) + ")";

		SQLAllocHandle(SQL_HANDLE_STMT, _dbc, &stmt);

		if( SQLExecDirectA(stmt, (SQLCHAR*)fullQuery.c_str(), SQL_NTS) == SQL_ERROR )
		{
			ERROR_OUTPUT( record, "Failed to insert record: " << record->getType() << " into database.  Query: " << fullQuery );
		}

		SQLFreeHandle( SQL_HANDLE_STMT, stmt );

		checkDB( _recordProperties, false );
		LOG_TRACE( "/MSSQL::record" );
	}

	void MSSQL::checkDB( property_map& props, bool start )
	{
		LOG_TRACE( "MSSQL::checkDB" );

		std::string database;
		std::string server;
		std::string port;
		std::string user;
		std::string password;
		std::string table;

		property_map::iterator itr = props.find( "database" );
		if( itr != props.end() )
			database = itr->second;
		else
			database = "";

		itr = props.find( "server" );
		if( itr != props.end() )
			server = itr->second;
		else
			server = "localhost";

		itr = props.find( "user" );
		if( itr != props.end() )
			user = itr->second;
		else
			user = "";

		itr = props.find( "password" );
		if( itr != props.end() )
			password = itr->second;
		else
			password = "";

		itr = props.find( "table" );
		if( itr != props.end() )
			table = itr->second;
		else
			table = "";


		// Check database connection
		if( start )
		{
			if( _dbConnected )
			{
				if( database != _database || server != _server || user != _user || password != _password )
				{
					_storedDatabases.push_back( _database );
					_storedDatabases.push_back( _server );
					_storedDatabases.push_back( _user );
					_storedDatabases.push_back( _password );
					disconnect();
					connect( database, server, user, password );
				}
				// Current database matches desired
			}
			else
			{
				// No database connected, so connect
				connect( database, server, user, password );
			}
		}
		else
		{
			// End of function database check
			if( _storedDatabases.size() > 0 )
			{
				// restore the previous database connection
				disconnect();
				password = _storedDatabases.back();
				_storedDatabases.pop_back();
				user = _storedDatabases.back();
				_storedDatabases.pop_back();
				server = _storedDatabases.back();
				_storedDatabases.pop_back();
				database = _storedDatabases.back();
				_storedDatabases.pop_back();
				connect( database, server, user, password );
			}
		}

		// Should always be connected to 1 database
		if( !_dbConnected )
		{
			printf( "Failed to connect to the database: %s, ms sql output will not be available\n", database.c_str() );
		}

		
		LOG_TRACE( "/MSSQL::checkDB" );
	}

	void MSSQL::disconnect()
	{
		if( !_dbConnected ) return;
		SQLDisconnect( _dbc );
		SQLFreeHandle( SQL_HANDLE_DBC, _dbc );
		SQLFreeHandle( SQL_HANDLE_ENV, _env );
		_dbConnected = false;
	}

	void MSSQL::connect( const std::string& database, const std::string& server, const std::string& user, const std::string& password )
	{
		// We disconnect before the check because if the information passed is invalid, we want the program to
		// not corrupt the current database.
		if( _dbConnected ) disconnect();

		if( database == "" || server == "" || user == "" || password == "" )
		{
			printf( "Not enough information to connect to sql database\n" );
			return;
		}

		SQLRETURN ret;
		SQLCHAR OutConnStr[255];
		SQLSMALLINT OutConnStrLen;


		if( (ret=SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &_env )) != SQL_SUCCESS )
		{
			printf( "Could not alocate environment handle\n" );
			disconnect();
			return;
		}

		if( (ret=SQLSetEnvAttr( _env, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER )) != SQL_SUCCESS )
		{
			printf( "Could not set environment variable\n" );
			disconnect();
			return;
		}

		if( (ret=SQLAllocHandle( SQL_HANDLE_DBC, _env, &_dbc )) != SQL_SUCCESS )
		{
			printf( "Could not alocate database handle\n" );
			disconnect();
			return;
		}

		std::string dsn = "DRIVER=sql server;DATABASE=" + database + ";SERVER=" + server + ";Uid=" + user + ";Pwd=" + password + ";";

		ret = SQLDriverConnectA( _dbc, NULL, (SQLCHAR*)dsn.c_str(), SQL_NTS, OutConnStr, sizeof( OutConnStr ), &OutConnStrLen, SQL_DRIVER_COMPLETE );

		if( SQL_SUCCEEDED( ret ) )
		{
			_database = database;
			_server = server;
			_user = user;
			_password = password;
			_dbConnected = true;
		}
		else
		{
			extractError( "SQLDriverConnect", _dbc, SQL_HANDLE_DBC );
			disconnect();
		}
	}

	// Sanitize a string so it works in a sql query
	std::string MSSQL::sanitize( const std::string& string )
	{
		std::string retVal = string;
		int pos = 0;
		while( ( pos = retVal.find( "'" ) ) != std::string::npos )
			retVal = retVal.replace( pos, 1, "\"" );
		
		return retVal;
	}

	void MSSQL::extractError( char* fn, SQLHANDLE handle, SQLSMALLINT type )
	{
		SQLINTEGER	 i = 0;
		SQLINTEGER	 native;
		SQLCHAR	 state[ 7 ];
		SQLCHAR	 text[256];
		SQLSMALLINT	 len;
		SQLRETURN	 ret;

		fprintf(stderr,
				"\n"
				"The driver reported the following diagnostics whilst running "
				"%s\n\n",
				fn);

		do
		{
			ret = SQLGetDiagRecA(type, handle, ++i, state, &native, text,
								sizeof(text), &len );
			if (SQL_SUCCEEDED(ret))
				printf("%s:%ld:%ld:%s\n", state, i, native, text);
		}
		while( ret == SQL_SUCCESS );

	}

}
