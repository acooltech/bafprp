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

#include <map>

#include "bafdefines.h"
#include "ifield.h"


namespace bafprp
{
	class IBafRecord
	{
		friend class RecordMaker;
	public:
		bool hasField( const std::string& name ) const;

		IField* getField( const std::string& name ) const;
		IField* getNextField( bool reset = false ) const;

		virtual std::string getType() const = 0;

		int getSize() const { return _length + 5; }

		std::string getData() const { return getChars( _data, _length  * 2   ); }
		long getFilePosition() const { return _filePos; }

		DWORD getCRC() const { return _crc; }
		std::string getFilename() const { return _filename; }

		virtual ~IBafRecord();	
	protected:
		IBafRecord( const BYTE* data, int length, const std::string& filename, long filePos );

		void addField( const std::string& name, DWORD offset );
		void decodeModules();

	
		typedef std::map<std::string, DWORD> field_map;  
		field_map _fields;

		int _length;
	private:
		bool _modules;
		DWORD _crc;
		std::string _filename;
		long _filePos;
		const BYTE* _data;  // This one needs to stay constant since we have to delete it later
		field_map::iterator getNextItr;
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
		static IBafRecord* newRecord( const BYTE* data, int length, const std::string& filename, long filePos );
	protected:
		RecordMaker( int type )
		{
			getReg().insert ( std::make_pair ( type, this ) );
		}
		virtual IBafRecord* make( const BYTE* data, int length, const std::string& filename, long filePos ) const = 0;
	};

	bool recordequal( const IBafRecord* left, const IBafRecord* right );
	bool recordsort( const IBafRecord* left, const IBafRecord* right );

}

#endif
