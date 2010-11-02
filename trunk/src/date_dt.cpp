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

#include <time.h>

#include "date_dt.h"
#include "output.h"

namespace bafprp
{
	const DateFieldMaker DateFieldMaker::registerThis;

	IField* DateFieldMaker::make() const
	{
		return new DateField;
	}

	DateField::DateField() : IField()
	{
		LOG_TRACE( "DateField::Date" );
		LOG_TRACE( "/DateField::Date" );
	}


	DateField::~DateField()
	{
		LOG_TRACE( "DateField::~Date" );
		LOG_TRACE( "/DateField::~Date" );
	}

	bool DateField::convert ( const BYTE* data )
	{
		LOG_TRACE( "DateField::convert" );
		_return = getChars( data, getSize() );
		_converted = true;

		if( _return.length() != getSize() ) 
		{
			_lastError = "Data read is not the correct size";
			_converted = false;
		}

		LOG_TRACE( "/DateField::convert" );
		return _converted;
	}

	std::string DateField::getString() const
	{
		LOG_TRACE( "DateField::getString" );

		std::string ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get string before field was converted" );
			ret = "";
		}
		else
		{
			char year[5] = "";
			time_t ltime; 
			struct tm* mytm;
			ltime = time( NULL );
			mytm = localtime( &ltime );
			strftime( year, sizeof( year ), "%Y", mytm );
			year[3] = _return[0];

			if( ( mytm->tm_year + 1900 ) < atoi( year ) )
			{
				LOG_WARN( "The year read from the baf file " << year << " is greater than the current system year " << (mytm->tm_year+1900) << " assuming decade mismatch, subtracting 10 from baf file year" );
				year[2]--;  // playing with the ascii code, not bothering to convert to int
				if( year[2] < '0' )
					year[2] = '9';
			}
			
			std::string format = getProperty( "format", true );
			if( format != "" && format.find( "Y" ) != std::string::npos && format.find( "M" ) != std::string::npos && format.find( "D" ) != std::string::npos )
			{
				ret = format;
				ret.replace( ret.find("Y"), 1, year );
				ret.replace( ret.find("M"), 1, _return.substr(1,2) );
				ret.replace( ret.find("D"), 1, _return.substr(3,2) );
			}
			else
			{
				std::ostringstream os;
				os << year << "-" << _return[1] << _return[2] << "-" << _return[3] << _return[4];
				ret = os.str();
			}
		}

		LOG_TRACE( "/DateField::getString" );
		return ret;
	}
}
