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

#include "fileoutput.h"

#include <iomanip>
#include <ios>
#include <stdio.h>

namespace bafprp
{
	const File File::registerThis;

	File::File() : Output( "file" )
	{
		// Not sure if its a good idea to lock a file for the duration of the program,
		// but I cannot find a way to unlock the file.
		if( fopen_s( &_fp, "bafprp.log", "w" ) != 0 )
		{
			Output::setLogOutput( "console" );
			LOG_ERROR( "Could not open log file, falling back to console log" );
		}
	}

	File::~File()
	{
		if( _fp ) fclose( _fp );
	}

	void File::error( IBafRecord* record, const std::string error )
	{
		if( !_fp )
		{
			Output::setLogOutput( "console" );
			LOG_ERROR( "File pointer no longer valid, falling back to console output" );
		}
		LOG_TRACE( "File::error" );

		fprintf_s( _fp, "* Record Error *********************************************\n" );
		fprintf_s( _fp, "*                                                          *\n" );
		fprintf_s( _fp, "*    %s                             *\n", NowTime().c_str() );

		std::ostringstream os;
		os.setf( std::ios::left );

		fprintf_s( _fp, "*                                                          *\n" );
		os << "* Report: " << std::setw(48) << error.substr(0,48) << " *" << std::endl;
		fprintf_s( _fp, os.str().c_str() );
		os.str("");
		std::string rest = "";
		
		// Avoid the out_of_range exception
		if( error.length() > 48 )
			rest = error.substr( 48 );

		// I like neatly formated messages.
		while( rest != "" )
		{
			os << "*         " << std::setw(48) << rest.substr(0,48) << " *" << std::endl;
			fprintf_s( _fp, os.str().c_str() );
			os.str("");

			if( rest.length() > 48 )
				rest = rest.substr( 48 );
			else
				rest = "";
		}

		fprintf_s( _fp, "*                                                          *\n" );
		os << "* Details: Type: " << std::setw(41) << record->getType() << " *" << std::endl;
		fprintf_s( _fp, os.str().c_str() );
		os.str("");
		os << "*          Length: " << std::setw(39) << record->getSize() << " *" << std::endl;
		fprintf_s( _fp, os.str().c_str() );
		os.str("");
		os << "*          Position: " << std::setw(37) << record->getFilePosition() << " *" << std::endl;	
		fprintf_s( _fp, os.str().c_str() );
		os.str("");


		//const BYTE* dp = record->getDataPointer();

		fprintf_s( _fp, "*                                                          *\n" );
		os << "* BYTES: " << std::setw(49) << record->getData() << " *" << std::endl;

		//os.unsetf( std::ios::dec );
		//os.unsetf( std::ios::showbase );
		//os.setf( std::ios::uppercase | std::ios::hex );
		//for( int i = 0; i < 42; i++ )
		//{
		//	os << (( dp[i] & 0xF0 ) >> 4 ) + 0x30;
		//	os << ( dp[i] & 0x0F ) + 0x30;
		//}
		//os << " *" << std::endl;	
		fprintf_s( _fp, os.str().c_str() );
		os.str("");

		LOG_TRACE( "/File::error" );
	}

	void File::log( const std::string log )
	{
		if( !_fp )
		{
			Output::setLogOutput( "console" );
			LOG_ERROR( "File pointer no longer valid, falling back to console output" );
		}
		fprintf_s( _fp, "%s\n", log.c_str() );
	}

	void File::record( IBafRecord* record )
	{
		if( !_fp )
		{
			Output::setLogOutput( "console" );
			LOG_ERROR( "File pointer no longer valid, falling back to console output" );
		}
		LOG_TRACE( "File::record" );

		IField* field;
		std::string lastName = "";
		
		fprintf_s( _fp, "---------------------------------------\n" );
		fprintf_s( _fp, "%s\n", record->getType().c_str() );
		fprintf_s( _fp, "---------------------------------------\n" );
		fprintf_s( _fp, "Length of record: %d\n", record->getSize() );
		fprintf_s( _fp, "---------------------------------------\n" );

		// sending a last name of "" effectively returns us the begining, getting the ball rolling.
		while( ( field = record->getNextField( lastName ) ) != NULL )
		{
			lastName = field->getName();
			fprintf_s( _fp, "%s: %s\n", lastName.c_str(), field->getString().c_str() );
		}

		LOG_TRACE( "/File::record" );
	}
}