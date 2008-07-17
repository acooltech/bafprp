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


// General purpose testing while building converters.



#include <iostream>
#include <cstdio>

#include "baffile.h"
#include "output.h"

using namespace bafprp;
using namespace std;

int main ( int argc, char *argv[] )
{	
	Output::setLogLevel( LOG_LEVEL_DEBUG );
	Output::setoutputRecord( "file" );
	Output::setoutputError( "file" );
	Output::setoutputLog( "file" );
	LOG_TRACE( "Global::main" );
	

	BafFile* file = new BafFile( "baf.pri" );
	IBafRecord* record;

	while( file->isOpen()  )
	{
		record = file->getNextRecord();
		if( record )
		{
			Output::outputRecord( record );
			delete record;
		}
	}

	file->close();

	LOG_TRACE( "/Global::main" );
	cout << "Press any key to exit..." << endl;
	getchar();

	
	return EXIT_SUCCESS;
}