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

#include "360.h"
#include "output.h"

namespace bafprp
{

	const r360Maker r360Maker::registerThis;

	IBafRecord* r360Maker::make( const BYTE* data, int length, const std::string& filename, long filePos ) const
	{
		LOG_TRACE( "r360Maker::make" );
		LOG_TRACE( "/r360Maker::make" );
		return new r360( data, length, filename, filePos );
	}

	r360::r360( const BYTE* data, int length, const std::string& filename, long filePos ) : IBafRecord( data, length, filename, filePos )
	{
		// make the real structure
		addField( "structurecode" ); 
		addField( "calltype" ); 
		addField( "sensortype" ); 
		addField( "sensorid" ); 
		addField( "recordofficetype" );
		addField( "recordofficeid" );
		addField( "date" );
		addField( "timingindicator" );
		addField( "studyindicator" );
		addField( "calledpartyanswerindicator" );
		addField( "servicetrafficind" );
		addField( "operatoraction" );
		addField( "servicefeaturecode" );
		addField( "originatingnpa" );
		addField( "originatingnumber" );
		addField( "dialednpa" );
		addField( "dialednumber" );
		addField( "overseasindicator" );
		addField( "terminatingnpa" );
		addField( "terminatingnumber" );
		addField( "time" );
		addField( "elapsedtime" );
		addField( "icincprefix" );
		addField( "carrierconnectdate" );
		addField( "carrierconnecttime" );
		addField( "carrierelapsedtime" );
		addField( "icinceventstatus" );
		addField( "trunkgroupnumber" );
		addField( "icincroutingind" );
		addField( "anicpnindicator" );
		addField( "rao" );
		addField( "lata" );
		decodeModules();
	}

	std::string r360::getType() const
	{
		// This type of record only has 1 use
		return "Record 360: Unknown";
	}

	r360::~r360()
	{
	}

}
