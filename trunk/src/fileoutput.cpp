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

/* NOTES NOTES NOTES NOTES NOTES NOTES NOTES NOTES NOTES NOTES
// 
// It is very important that this class does not change
// Do not edit it, copy it, or in the worst case, delete it
// When output encounters a problem it will default to this
// form of output, and if this class is changed you will encounter
// unexpected results.
//
NOTES NOTES NOTES NOTES NOTES NOTES NOTES NOTES NOTES NOTES */

#include <iomanip>
#include <ios>

#include "fileoutput.h"
#include "baffile.h"


namespace bafprp
{
	const File File::registerThis;

	File::~File()
	{
		if( _file.is_open() ) _file.close();
	}

	// Best to avert your eyes.
	void File::error( const BafRecord* record, const std::string& error )
	{
		LOG_TRACE( "File::error" );
		checkFile( _errorProperties, true );
		if( !_file.is_open() )
		{
			LOG_ERROR( "No valid file for error output, falling back to console output" );
			Output::setOutputError( "console" );

			// be nice
			Output::outputError( record, error );
			return;
		}

		_file.setf( std::ios::left );

		_file << "* Record Error *********************************************" << std::endl;
		_file << "*                                                          *" << std::endl;
		_file << "*    " << std::setw(53) << NowTime() << " *" << std::endl;

		_file << "*                                                          *" << std::endl;

		std::string rest = "";
		int space = 0;

		if( error.length() > 48 )
		{
			space = error.substr( 0, 48 ).find_last_of( " " );
			if( space == std::string::npos )
					space = 48;
		}
		else
			space = 48;

		_file << "* Report: " << std::setw( 48 ) << error.substr( 0, space ) << " *" << std::endl;
		
		// Avoid the out_of_range exception
		if( error.length() > 48 )
			rest = error.substr( space + 1 );		
		
		// I like neatly formated messages.
		while( rest != "" )
		{
			if( rest.length() > 49 ) 
			{
				space = rest.substr( 0, 48 ).find_last_of( " " );
				if( space == std::string::npos )
					space = 48;
			}
			

			_file << "*         " << std::setw( 48 ) << rest.substr( 0, space ) << " *" << std::endl;
			
			if( rest.length() > 49 )	
				rest = rest.substr( space + 1 );
			else
				rest = "";
		}

		_file << "*                                                          *" << std::endl;
		_file << "* Details: Type: " << std::setw(41) << record->getType() << " *" << std::endl;
		_file << "*          Length: " << std::setw(39) << record->getSize() << " *" << std::endl;
		_file << "*          Position: " << std::setw(37) << record->getFilePosition() << " *" << std::endl;
		_file << "*          Filename: " << std::setw(37) << record->getFilename() << " *" << std::endl;

		_file << "*                                                          *" << std::endl;
		std::string bytes = record->getData();
		_file << "* BYTES: " << std::setw(49) << bytes.substr( 0, 48 ) << " *" << std::endl;

		rest = "";

		if( bytes.length() > 48 )
			rest = bytes.substr( 48 );
		
		while( rest != "" )
		{
			_file << "*        " << std::setw(49) << rest.substr( 0, 48 ) << " *" << std::endl;

			if( rest.length() > 48 )
				rest = rest.substr( 48 );
			else
				rest = "";
		}
		_file << "*                                                          *" << std::endl;
		_file << "************************************************************" << std::endl;
		_file << std::endl;

		_file.flush();
	
		checkFile( _errorProperties, false );
		LOG_TRACE( "/File::error" );
	}

	void File::log( LOG_LEVEL level, const std::string& log )
	{
		checkFile( _logProperties, true );
		if( !_file.is_open() )
		{
			Output::setOutputLog( "console" );
			LOG_ERROR( "No valid file for log output, falling back to console output" );

			// be nice
			Output::outputLog( level, log );
			return;
		}

		_file << "- " << NowTime() << " " << getStrLogLevel( level ) << ": " << log << std::endl;
		_file.flush();
		checkFile( _logProperties, false );
	}

	void File::record( const BafRecord* record )
	{
		LOG_TRACE( "File::record" );
		checkFile( _recordProperties, true );
		if( !_file.is_open() )
		{
			LOG_ERROR( "No valid file for record output, falling back to console output" );
			Output::setOutputRecord( "console" );

			// be nice
			Output::outputRecord( record );
			return;
		}

		const IField* field;
		DWORD lastUID = 0;
		
		_file << "--------------------------------------------------------------------------" << std::endl;
		_file << record->getType() << std::endl;
		_file << "--------------------------------------------------------------------------" << std::endl;
		_file << "Length of record: " << record->getSize() << ", CRC32: " << record->getCRC() << std::endl;
		_file << "--------------------------------------------------------------------------" << std::endl;
		_file << "File: " << record->getFilename() << std::endl;
		_file << "--------------------------------------------------------------------------" << std::endl;
		_file << "Position: " << record->getFilePosition() << std::endl;
		_file << "--------------------------------------------------------------------------" << std::endl;

		// sending a lastUID of 0 effectively returns us the begining, getting the ball rolling.
		while( ( field = record->getNextField( lastUID ) ) != NULL )
		{
			lastUID = field->getUID();
			if( !field->filter() )
				_file << field->getDesc() << ": " << field->getID() << ": " << field->getString() << std::endl;

		}

		_file << std::endl;
		_file.flush();

		checkFile( _recordProperties, false );
		LOG_TRACE( "/File::record" );
	}


	// Function explanation:
	//
	// Well basically this function is the heart of file output.
	// You would think writing to a file would be nice and easy but consider a sitatuation
	// where the user set record output to "record.log", error output to "error.log", and log
	// output to "log.log".  
	// Since there is only 1 instance of each output allowed the class needs to be able to switch
	// between these files seamlessly.
	// Also, if we generate an log message while printing a record, we need to be able to 
	// close the current file, open the log file, write the log, close THAT file, and restore the previous file
	// so record output continues like nothing happened.
	// Basically this function connects all three output worlds in a nice and seamless manner so the individual
	// functions do not have to worry about things like this.
	//
	// Note from the future
	// I looked back on this about a year after writing it and I am wondering why I did not just keep three 
	// file pointers around.  It took a day to remember but when I originally wrote this I was expecting
	// the posibility of filenames changing mid program.
	// The problem with that is this class is never notified about property changes, it just has access to its properties.
	// It would be easy yes to check that the filenames and the open files match each time we want to write to a file,
	// and in fact that would be the 'proper' way to do those.
	// To be honest I am not exactly sure why I thought I only had access to 1 file pointer for the entire 
	// program, I must not have been thinking.
	// I would fix it, but wise man once said, why fix what aint broke?
	void File::checkFile( property_map& props, bool start )
	{
		// The property should have a filename parameter
		property_map::iterator filename = props.find( "filename" );
		if( filename == props.end() )
		{
			printf( "Error: no 'filename' property for output\n" );
			return;	
		}
		
		std::string desiredFilename = filename->second;
		while( desiredFilename.find('$') != std::string::npos )
		{
			int begin = desiredFilename.find('$');
			int end = desiredFilename.find( '$', begin + 1 );

			if( end == std::string::npos )
				break;

			std::string keyword = desiredFilename.substr( begin + 1, end - begin - 1 );
			
			if( keyword == "filename" )
			{
				std::string processingFile = BafFile::getFilename();
				if( processingFile == "" ) 
					processingFile = "bafprp";
				desiredFilename.replace( begin, end - begin + 1, processingFile );
			}
		}

		// If the function is at the begining, we need to make sure the right file is open
		if( start )
		{
			if( _file.is_open() ) 
			{
				if( _filename != desiredFilename )  // Is the RIGHT file open?
				{
					// If not, we need to store the current file's name, and open the correct file
					_storedFilenames.push_back( _filename );
					_filename = "";
					_file.close();

					// If the filename we are opening has been used before we need to open it to APPEND, however
					// if it has NOT been used before, open it normally, wiping out the previous contents.
					bool bFound = false;
					for( std::vector<std::string>::iterator itr = _usedFilenames.begin(); itr != _usedFilenames.end(); itr++ )
					{
						if( (*itr) == desiredFilename )
						{
							bFound = true;
							break;
						}
					}

					if( !bFound )
						_file.open( desiredFilename.c_str() );  // Clear original file
					else
						_file.open( desiredFilename.c_str(), std::ios::app );  // Append

					// does not matter if file opened or not, individual functions check and make better errors then
					// we could dream of making

					_filename = desiredFilename;
					_usedFilenames.push_back( _filename );  // Make sure we open it for APPENDING next time
				}
				// current open file matches desired file.
			}
			else // No file is open, need to open a new file for output
			{
				// Since there should ALWAYS been one file open, we can assume this is the first time this
				// file is being opened and not check for appending.
				// If you encounter a bug where your files are being cleared in the middle of the program,
				// you are probably doing some very weird things to your output, but if need be, we can copy 
				// the used file check here.
				_file.open( desiredFilename.c_str() );

				// does not matter if file opened or not, individual functions check and make better errors then
				// we could dream of making

				_filename = desiredFilename;
				_usedFilenames.push_back( _filename );  // Open for appending next time
			}
		}
		else  // end of function file check
		{
			// The function is ending, we need to restore the previous open file, if there was one.
			if( _storedFilenames.size() > 0 )  // we had to push off another file to output
			{
				// restore that file
				_file.close();
				_file.open( _storedFilenames.back().c_str(), std::ios::app );
				_filename = _storedFilenames.back();
				_storedFilenames.pop_back();
			}
		}
	}
}
