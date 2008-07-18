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

#include "numberofamacalls.h"
#include "output.h"

namespace bafprp
{
	const NumberOfAMACallsFieldMaker NumberOfAMACallsFieldMaker::registerThis;

	IField* NumberOfAMACallsFieldMaker::make() const
	{
		LOG_TRACE( "NumberOfAMACallsFieldMaker::make" );
		LOG_TRACE( "/NumberOfAMACallsFieldMaker::make" );
		return new NumberOfAMACalls;
		
	}

	NumberOfAMACalls::NumberOfAMACalls() : IField()
	{
		LOG_TRACE( "NumberOfAMACalls::NumberOfAMACallsFieldConverter" );
		LOG_TRACE( "/NumberOfAMACalls::NumberOfAMACallsFieldConverter" );
	}


	NumberOfAMACalls::~NumberOfAMACalls()
	{
		LOG_TRACE( "NumberOfAMACalls::~NumberOfAMACallsFieldConverter" );
		LOG_TRACE( "/NumberOfAMACalls::~NumberOfAMACallsFieldConverter" );
	}

	bool NumberOfAMACalls::convert ( const BYTE* data )
	{
		LOG_TRACE( "NumberOfAMACalls::convert" );
		_return = getChars( data, getSize() );
		_converted = true;

		if( _return.length() != getSize() ) 
		{
			_lastError = "Data read is not the correct size";
			_converted = false;
		}

		LOG_TRACE( "/NumberOfAMACalls::convert" );
		return _converted;
	}

	int NumberOfAMACalls::getInt() const
	{
		LOG_TRACE( "NumberOfAMACalls::getInt" );

		int ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get int before field was converted" );
			ret = 0;
		}
		else
			ret = atoi( _return.c_str() );

		LOG_TRACE( "/NumberOfAMACalls::getInt" );
		return ret;
	}

	long NumberOfAMACalls::getLong() const
	{
		LOG_TRACE( "NumberOfAMACalls::getLong" );

		long ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get long before field was converted" );
			ret = 0;
		}
		else
			ret = atol( _return.c_str() );

		LOG_TRACE( "/NumberOfAMACalls::getLong" );
		return ret;
	}

	std::string NumberOfAMACalls::getString() const
	{
		LOG_TRACE( "NumberOfAMACalls::getString" );

		std::string ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get string before field was converted" );
			ret = "";
		}
		else
			ret = _return;

		LOG_TRACE( "/NumberOfAMACalls::getString" );
		return ret;
	}
}
