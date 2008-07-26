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

#include "duplicate.h"

namespace bafprp
{

	std::string BafFile::_filename = "";

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

			// Clean up records
			if( _records.size() > 0 )
			{
				for( std::vector<IBafRecord*>::iterator itr = _records.begin(); itr != _records.end(); itr++ )
					if( *itr ) delete *itr;
			}

			_records.clear();

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

	bool BafFile::parse( const std::string filename )
	{
		LOG_TRACE( "BafFile::parse" );
		if( filename != _filename ) 
		{
			if( !read( filename ) )
			{
				LOG_ERROR( "File::Parse failed to read the file" );
				return false;
			}
		}

		while( _offset < _fileSize )
		{
		
			_length_of_record = ( _fileData[_offset] * 256 ) + _fileData[_offset + 1];

			if( _length_of_record == 0 )
			{
				LOG_FATAL( "Length of record is zero, bad baf file." );
				return false;
			}

			LOG_DEBUG( "Reading record of length " << _length_of_record << " at file offset " << _offset );
			IBafRecord* record = RecordMaker::newRecord( _fileData + _offset, _length_of_record, _offset ); 
			LOG_DEBUG( "Read " << record->getType() << " at " << record->getFilePosition() );
			if( record )
				_records.push_back( record );
			
			_offset += _length_of_record;
		}

		LOG_INFO( "Parsed " << _records.size() << " records" );

		LOG_TRACE( "/BafFile::parse" );
		return true;
	}

	bool BafFile::read( const std::string filename )
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

	bool BafFile::process( const std::string filename, bool listDups )
	{
		if( filename != _filename ) 
		{
			if( !parse( filename ) ) // has the effect of calling both read and parse
			{
				LOG_ERROR( "File::Process failed to parse the file" );
				return false;
			}
		}

		long size = _records.size();

		if( listDups ) Duplicate::list( _records );
		Duplicate::remove( _records );
		
		LOG_INFO( "Removed " << size - _records.size() << " duplicates" );

		for( std::vector<IBafRecord*>::iterator itr = _records.begin(); itr != _records.end(); itr++ )
		{
			if( *itr ) Output::outputRecord( *itr );
		}
		
		return true;
	}
}