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

#include <fstream>

#include "baffile.h"
#include "output.h"

namespace bafprp
{

	BafFile::BafFile() : _offset(0), _fileSize(0), _fileData(NULL), _length_of_record(0)
	{
	}

	BafFile::~BafFile()
	{
		LOG_TRACE( "BafFile::~BafFile" );
		clear();
		LOG_TRACE( "/BafFile::~BafFile" );
	}

	bool BafFile::clear()
	{
		try
		{
			_offset = 0;
			_fileSize = 0;
			_length_of_record = 0;
			_filename.clear();

			_recordCRCs.clear();

			if( _fileData )
			{
				delete[] _fileData;
				_fileData = NULL;
			}
		}
		catch( ... )
		{
			LOG_ERROR( "Encountered an error while clearing file object" );
			return false;
		}

		return true;
	}

	bool BafFile::parse( const std::string& filename, bool listDups )
	{
		LOG_TRACE( "BafFile::parse" );
		
		int dups = 0;

		if( filename != _filename ) 
		{
			if( !read( filename ) )
			{
				LOG_ERROR( "File::Parse failed to read the file" );
				return false;
			}
		}

		if( _fileData[0] == 0x1C )
		{
			LOG_DEBUG( "Decoding AMADNS header information" );
			BafRecord* record = RecordMaker::newRecord( _fileData, 0x1C, _filename, 0 );
			if( record )
			{
				LOG_DEBUG( "Read AMADNS header" );
				Output::outputRecord( record );
				_recordCRCs.insert( record->getCRC() );
				delete record;
			}
			else
				LOG_WARN( "Failed to read AMADNS header, file may be corrupt" );

			_offset += 0x1C;
		}

		while( _offset < _fileSize )
		{
			if( _fileData[_offset] == 0xAA )
			{
				_offset++;
				continue;
			}

			// DIRP format packs records into groups and has a 'container' record that basically just says "this group of records is this long"
			// This little check skips that
			if( _fileData[_offset + 4] != 0xAA && _fileData[_offset + 4 ] != 0xAB )
			{
				_offset += 4;
				continue;
			}
		
			_length_of_record = ( _fileData[_offset] * 256 ) + _fileData[_offset + 1];

			if( _length_of_record == 0 )
			{
				LOG_FATAL( "Length of record is zero, bad baf file." );
				return false;
			}

			if( ( _offset + _length_of_record ) > _fileSize )
			{
				LOG_FATAL( "Unexpected end of file at " << _offset << ".  Record is " << _length_of_record << " bytes long, but only " << ( _fileSize - _offset ) << " bytes are left" );
				return false;
			}

			LOG_DEBUG( "Reading record of length " << _length_of_record << " at file offset " << _offset );
			BafRecord* record = RecordMaker::newRecord( _fileData + _offset, _length_of_record, _filename, _offset ); 
			if( record )
			{
				LOG_DEBUG( "Read " << record->getType() << " at " << record->getFilePosition() );
				if( _recordCRCs.find( record->getCRC() ) != _recordCRCs.end() )
				{
					if( listDups )
						ERROR_OUTPUT( record, "Record is a duplicate." );
					dups++;
				}
				else
				{
					Output::outputRecord( record );
					_recordCRCs.insert( record->getCRC() );
				}
				delete record;
			}
			
			_offset += _length_of_record;
		}

		LOG_INFO( "Parsed " << _recordCRCs.size() << " records" );
		LOG_INFO( "Removed " << dups << " duplicates" );

		LOG_TRACE( "/BafFile::parse" );
		return true;
	}

	bool BafFile::read( const std::string& filename )
	{
		LOG_TRACE( "BafFile::read" );
		_offset = 0;

		std::ifstream file( filename.c_str(), std::ios::in | std::ios::binary | std::ios::ate );
		if( file.is_open() )
		{
			_fileSize = file.tellg();
			_fileData = new BYTE[_fileSize];
			memset( _fileData, 0, sizeof(BYTE) * _fileSize );
			file.seekg( 0, std::ios::beg );
			file.read( (char*)_fileData, _fileSize );
			file.close();
		}
		else
		{
			LOG_ERROR( "Could not open file: " << filename );
			return false;
		}

		_filename = filename;

		LOG_TRACE( "/BafFile::read" );
		return true;
	}
}
