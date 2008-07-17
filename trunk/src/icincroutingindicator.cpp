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

#include "icincroutingindicator.h"
#include "output.h"

namespace bafprp
{
	const ICINCRoutingIndicatorFieldMaker ICINCRoutingIndicatorFieldMaker::registerThis;

	IField* ICINCRoutingIndicatorFieldMaker::make() const
	{
		LOG_TRACE( "ICINCRoutingIndicatorFieldMaker::make" );
		LOG_TRACE( "/ICINCRoutingIndicatorFieldMaker::make" );
		return new ICINCRoutingIndicator;
		
	}

	ICINCRoutingIndicator::ICINCRoutingIndicator() : IField()
	{
		LOG_TRACE( "ICINCRoutingIndicator::ICINCRoutingIndicatorFieldConverter" );
		LOG_TRACE( "/ICINCRoutingIndicator::ICINCRoutingIndicatorFieldConverter" );
	}


	ICINCRoutingIndicator::~ICINCRoutingIndicator()
	{
		LOG_TRACE( "ICINCRoutingIndicator::~ICINCRoutingIndicatorFieldConverter" );
		LOG_TRACE( "/ICINCRoutingIndicator::~ICINCRoutingIndicatorFieldConverter" );
	}

	bool ICINCRoutingIndicator::convert ( const BYTE* data )
	{
		LOG_TRACE( "ICINCRoutingIndicator::convert" );
		_return = getChars( data, getSize() );
		_converted = true;

		if( _return.length() != getSize() ) 
		{
			_lastError = "Data read is not the correct size";
			_converted = false;
		}

		LOG_TRACE( "/ICINCRoutingIndicator::convert" );
		return _converted;
	}

	int ICINCRoutingIndicator::getInt()
	{
		LOG_TRACE( "ICINCRoutingIndicator::getInt" );

		int ret;
		if( !_converted )
		{
			_lastError = "Tried to get int before field was converted";
			ret = 0;
		}
		else
			ret = atoi( _return.c_str() );

		LOG_TRACE( "/ICINCRoutingIndicator::getInt" );
		return ret;
	}

	long ICINCRoutingIndicator::getLong()
	{
		LOG_TRACE( "ICINCRoutingIndicator::getLong" );

		long ret;
		if( !_converted )
		{
			_lastError = "Tried to get long before field was converted";
			ret = 0;
		}
		else
			ret = atol( _return.c_str() );

		LOG_TRACE( "/ICINCRoutingIndicator::getLong" );
		return ret;
	}

	std::string ICINCRoutingIndicator::getString()
	{
		LOG_TRACE( "ICINCRoutingIndicator::getString" );

		std::string ret;
		if( !_converted )
		{
			_lastError = "Tried to get string before field was converted";
			ret = "";
		}
		else
			ret = _return;

		LOG_TRACE( "/ICINCRoutingIndicator::getString" );
		return ret;
	}
}
