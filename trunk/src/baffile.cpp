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

#include "baffile.h"

namespace bafprp
{
	BafFile::BafFile() : _filename( "" )
	{
	}

	BafFile::BafFile( const std::string filename ) : _filename( filename ), _fp( NULL )
	{
		if( !open( _filename ) ) return;
	}

	BafFile::BafFile(const char *filename) : _filename( filename ), _fp( NULL )
	{
		if( !open( _filename ) ) return;
	}

	BafFile::~BafFile()
	{
		_filename.clear();
	}

	bool BafFile::isOpen()
	{
		return ( _fp ? true : false );
	}

	BafRecord* BafFile::getNextRecord()
	{
		BYTE size[2] = "\x0";

		fseek( _fp, _offset, SEEK_SET );

		if( fread_s( size, 2, 1, 2, _fp ) != 2 ) return NULL;

		_length_of_record = ( size[0] * 256 ) + size[1];

		BYTE* data;
		BafRecord* record;
		try
		{
			data = new BYTE[ _length_of_record + 1 ];
			if( fread_s( data, _length_of_record, 1, _length_of_record, _fp ) != _length_of_record ) throw;

			record = new BafRecord( data, _length_of_record );
		
		}
		catch( ... )
		{}

		_offset += _length_of_record;
		delete[] data;
		return record;
	}

	BafRecord* BafFile::getCurrentRecord()
	{
		fseek( _fp, _offset - _length_of_record, SEEK_SET );

		

		return NULL;
	}

	bool BafFile::open( const char* filename )
	{
		return open( std::string( filename ) );
	}

	bool BafFile::open( const std::string filename )
	{
		if( fopen_s( &_fp, filename.c_str(), "rb" ) != 0 ) return false;
		_offset = 0;
		return true;
	}

	void BafFile::close()
	{
		if( _fp ) fclose( _fp );
	}
}