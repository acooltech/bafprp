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

#ifndef BAFPRPBAFRECORD_H
#define BAFPRPBAFRECORD_H

#include <vector>

#include "bafdefines.h"
#include "ifield.h"


namespace bafprp
{
	class IBafRecord
	{
	public:
		// Allow override because I feel like it
		virtual IField* getField( const std::string name );
		virtual IField* getNextField( const std::string last = "" );

		virtual std::string getType() const = 0;

		int getSize() const { return _length + 5; }
		
		virtual ~IBafRecord();	
	protected:
		IBafRecord( const BYTE* data, int length );

		void addField( const std::string name );
			
		typedef std::vector<IField*> field_vector;
		field_vector _fields;

		int _length;
		
		BYTE* _fieldData;  // The data pointer we are allowed to modify
	private:
		BYTE* _data;  // This one needs to stay constant since we have to delete it later
	};

	class RecordMaker
	{
	private:
		typedef std::map<int, RecordMaker*> maker_map;

		static maker_map& getReg()
		{
			static maker_map registry;
			return registry;
		}
		RecordMaker() {}
	public:
		static IBafRecord* newRecord( const BYTE* data, int length );
	protected:
		RecordMaker( int type )
		{
			getReg().insert ( std::make_pair ( type, this ) );
		}
		virtual IBafRecord* make( const BYTE* data, int length ) const = 0;
	};
}

#endif