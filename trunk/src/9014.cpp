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

#include "9014.h"
#include "output.h"

namespace bafprp
{

	const r9014Maker r9014Maker::registerThis;

	IBafRecord* r9014Maker::make( const BYTE* data, int length, long filePos ) const
	{
		LOG_TRACE( "r9014Maker::make" );
		LOG_TRACE( "/r9014Maker::make" );
		return new r9014( data, length, filePos );
	}

	r9014::r9014( const BYTE* data, int length, long filePos ) : IBafRecord( data, length, filePos )
	{
		// make the real structure
		addField( "structuretype" ); 
		addField( "calltype" ); 
		addField( "sensortype" ); 
		addField( "sensorid" ); 
		addField( "recordofficetype" );
		addField( "recordofficeid" );
		addField( "date" );
		addField( "time" );
		addField( "genericid" );
		addField( "traceraudittype" );
		addField( "filesequenceno" );
		addField( "numberofrecords" );
		addField( "fileblocks" );
	}

	std::string r9014::getType() const
	{
		// This type of record only has 1 use
		return "Record 9014: File End Marker";
	}

	r9014::~r9014()
	{
	}

}