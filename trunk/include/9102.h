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

#ifndef BAFPRP9102_H
#define BAFPRP9102_H

#include "ibafrecord.h"

namespace bafprp
{
	class r9102 : public IBafRecord
	{
		friend class r9102Maker;
	public:
		std::string getType() const;

		~r9102();
	protected:
		r9102( const BYTE* data, int length, const std::string filename, long filePos );
	};

	class r9102Maker : public RecordMaker
	{
	public:
		r9102Maker() : RecordMaker( 9102 ) {}
	protected:
		IBafRecord* make( const BYTE* data, int length, const std::string filename, long filePos ) const;
	private:
		static const r9102Maker registerThis;
	};
}

#endif