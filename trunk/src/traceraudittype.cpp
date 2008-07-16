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

#include "traceraudittype.h"
#include "output.h"

namespace bafprp
{
	const TracerAuditTypeFieldMaker TracerAuditTypeFieldMaker::registerThis;

	IField* TracerAuditTypeFieldMaker::make() const
	{
		LOG_TRACE( "TracerAuditTypeFieldMaker::make" );
		return new TracerAuditType();
		LOG_TRACE( "TracerAuditTypeFieldMaker::make" );
	}

	TracerAuditType::TracerAuditType() : IField()
	{
		LOG_TRACE( "TracerAuditType::TracerAuditType" );
		LOG_TRACE( "/TracerAuditType::TracerAuditType" );
	}


	TracerAuditType::~TracerAuditType()
	{
		LOG_TRACE( "TracerAuditType::~TracerAuditType" );
		LOG_TRACE( "/TracerAuditType::~TracerAuditType" );
	}

	bool TracerAuditType::convert ( const BYTE* data )
	{
		LOG_TRACE( "TracerAuditType::convert" );
		_return = getChars( data, getSize() );
		LOG_TRACE( "/TracerAuditType::convert" );
		return true;
	}

	long TracerAuditType::getLong()
	{
		LOG_TRACE( "TracerAuditType::getLong" );
		LOG_TRACE( "/TracerAuditType::getLong" );
		return atol( _return.c_str() );
	}

	int TracerAuditType::getInt()
	{
		LOG_TRACE( "TracerAuditType::getInt" );
		LOG_TRACE( "/TracerAuditType::getInt" );
		return atoi( _return.c_str() );
	}

	std::string TracerAuditType::getString()
	{
		LOG_TRACE( "TracerAuditType::getString" );
		LOG_TRACE( "/TracerAuditType::getString" );
		return _return;
	}
}