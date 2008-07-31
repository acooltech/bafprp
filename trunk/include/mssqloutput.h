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

#ifndef BAFPRPMSSQL_H
#define BAFPRPMSSQL_H

#include <sqltypes.h>

#include "output.h"

namespace bafprp
{
	class MSSQL : public Output
	{
	public:
		// Register the output type
		MSSQL() : Output( "mssql" ) {}
		~MSSQL() { disconnect(); }

		void record( const IBafRecord* record );
		void error( const IBafRecord* record, const std::string error );
		void log( LOG_LEVEL level, const std::string log );

	private:
		void checkDB( property_map& props, bool start );
		void connect( const std::string database, const std::string server, const std::string user, const std::string password );
		void disconnect();
		std::string sanitize( const std::string& string );

		// property variables
		std::string _database;
		std::string _server;
		std::string _user;
		std::string _password;

		std::string _table;

		// private non-property variables
		SQLHENV _env;
		SQLHDBC _dbc;
		bool _dbConnected;

		std::vector<std::string> _storedDatabases;
		
		// This variable simply initializes a class that registers with the main output code
		static const MSSQL registerThis;
	};
}

#endif