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

#include "numtrunklegsused.h"
#include "output.h"

namespace bafprp
{
	const NumTrunkLegsUsedFieldMaker NumTrunkLegsUsedFieldMaker::registerThis;

	IField* NumTrunkLegsUsedFieldMaker::make() const
	{
		LOG_TRACE( "NumTrunkLegsUsedFieldMaker::make" );
		LOG_TRACE( "/NumTrunkLegsUsedFieldMaker::make" );
		return new NumTrunkLegsUsed;
		
	}

	NumTrunkLegsUsed::NumTrunkLegsUsed() : IField()
	{
		LOG_TRACE( "NumTrunkLegsUsed::NumTrunkLegsUsedFieldConverter" );
		LOG_TRACE( "/NumTrunkLegsUsed::NumTrunkLegsUsedFieldConverter" );
	}


	NumTrunkLegsUsed::~NumTrunkLegsUsed()
	{
		LOG_TRACE( "NumTrunkLegsUsed::~NumTrunkLegsUsedFieldConverter" );
		LOG_TRACE( "/NumTrunkLegsUsed::~NumTrunkLegsUsedFieldConverter" );
	}

	bool NumTrunkLegsUsed::convert ( const BYTE* data )
	{
		LOG_TRACE( "NumTrunkLegsUsed::convert" );
		_return = getChars( data, getSize() );
		_converted = true;

		if( _return.length() != getSize() ) 
		{
			_lastError = "Data read is not the correct size";
			_converted = false;
		}

		LOG_TRACE( "/NumTrunkLegsUsed::convert" );
		return _converted;
	}

	int NumTrunkLegsUsed::getInt() const
	{
		LOG_TRACE( "NumTrunkLegsUsed::getInt" );

		int ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get int before field was converted" );
			ret = 0;
		}
		else
			ret = atoi( _return.c_str() );

		LOG_TRACE( "/NumTrunkLegsUsed::getInt" );
		return ret;
	}

	long NumTrunkLegsUsed::getLong() const
	{
		LOG_TRACE( "NumTrunkLegsUsed::getLong" );

		long ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get long before field was converted" );
			ret = 0;
		}
		else
			ret = atol( _return.c_str() );

		LOG_TRACE( "/NumTrunkLegsUsed::getLong" );
		return ret;
	}

	std::string NumTrunkLegsUsed::getString() const
	{
		LOG_TRACE( "NumTrunkLegsUsed::getString" );

		std::string ret;
		if( !_converted )
		{
			LOG_WARN( "Tried to get string before field was converted" );
			ret = "";
		}
		else
			ret = _return;

		LOG_TRACE( "/NumTrunkLegsUsed::getString" );
		return ret;
	}
}
