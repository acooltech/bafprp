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

#include "studyindicator.h"
#include "output.h"

namespace bafprp
{
	const StudyIndicatorFieldMaker StudyIndicatorFieldMaker::registerThis;

	IField* StudyIndicatorFieldMaker::make() const
	{
		LOG_TRACE( "StudyIndicatorFieldMaker::make" );
		LOG_TRACE( "/StudyIndicatorFieldMaker::make" );
		return new StudyIndicator;
		
	}

	StudyIndicator::StudyIndicator() : IField()
	{
		LOG_TRACE( "StudyIndicator::StudyIndicatorFieldConverter" );
		LOG_TRACE( "/StudyIndicator::StudyIndicatorFieldConverter" );
	}


	StudyIndicator::~StudyIndicator()
	{
		LOG_TRACE( "StudyIndicator::~StudyIndicatorFieldConverter" );
		LOG_TRACE( "/StudyIndicator::~StudyIndicatorFieldConverter" );
	}

	bool StudyIndicator::convert ( const BYTE* data )
	{
		LOG_TRACE( "StudyIndicator::convert" );
		_return = getChars( data, getSize() );
		_converted = true;

		if( _return.length() != getSize() ) 
		{
			_lastError = "Data read is not the correct size";
			_converted = false;
		}

		LOG_TRACE( "/StudyIndicator::convert" );
		return _converted;
	}

	int StudyIndicator::getInt()
	{
		LOG_TRACE( "StudyIndicator::getInt" );

		int ret;
		if( !_converted )
		{
			_lastError = "Tried to get int before field was converted";
			ret = 0;
		}
		else
			ret = atoi( _return.c_str() );

		LOG_TRACE( "/StudyIndicator::getInt" );
		return ret;
	}

	long StudyIndicator::getLong()
	{
		LOG_TRACE( "StudyIndicator::getLong" );

		long ret;
		if( !_converted )
		{
			_lastError = "Tried to get long before field was converted";
			ret = 0;
		}
		else
			ret = atol( _return.c_str() );

		LOG_TRACE( "/StudyIndicator::getLong" );
		return ret;
	}

	std::string StudyIndicator::getString()
	{
		LOG_TRACE( "StudyIndicator::getString" );

		std::string ret;
		if( !_converted )
		{
			_lastError = "Tried to get string before field was converted";
			ret = "";
		}
		else
			ret = _return;

		LOG_TRACE( "/StudyIndicator::getString" );
		return ret;
	}
}
