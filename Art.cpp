/*
Accomplice GM Tool Version 2.XX - GM Tool for Ultima Online Emulators.
Copyright (C) 2001 Bryan "Zippy" Pass
	E-Mail: zippy@uoxdev.com or "Zippy-" on irc.stratics.com #uox
		Contact me electronicly reguarding written contact.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by 
the Free Software Foundation; either version 2 of the License, or (at 
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
General Public License for more details.

You should have received a copy of the GNU General Public License 
along with this program; if not, write to:
Free Software Foundation, Inc., 
59 Temple Place, Suite 330
Boston, MA 02111-1307 USA
*/

#include "StdAfx.h"
#include "Accomplice.h"
#include "Art.h"

//I love Krrios.  (Mr. Fixit too, although not as much as Krrios)
//Art.mul sucks :o)

CArtMul::CArtMul( void )
{
	HKEY hKey = NULL;
	DWORD dwSize, dwType;
	int Result;
	char szPath[256];
	CString csPath;

	Result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\Origin Worlds Online\\Ultima Online\\1.0", 0, KEY_READ, &hKey );
	if ( Result )
		Result = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "Software\\Origin Worlds Online\\Ultima Online Third Dawn\\1.0", 0, KEY_READ, &hKey );

	if ( !Result )
	{
		dwSize = 256; dwType = REG_SZ;
		RegQueryValueEx( hKey, "ExePath", 0, &dwType, (BYTE *)szPath, &dwSize ); 
		TrimDir( szPath );
		RegCloseKey( hKey );

		csPath = szPath;
		Idx.open( (csPath + "artidx.mul"), ios::in|ios::binary );
		Mul.open( (csPath + "art.mul"), ios::in|ios::binary );
	}

	if ( Result || !Mul.is_open() || !Idx.is_open() )
		MessageBox( NULL, "There was an error loading Art.mul and ArtIdx.mul, tile previews will not be displayed.", "Error Opening Muls", MB_OK|MB_ICONERROR );
}

CArtMul::~CArtMul()
{
	if ( Mul ) Mul.close();
	if ( Idx ) Idx.close();
}

bool CArtMul::GetTile( int Id, CDC *pOutDC, CRect DrawRect, unsigned char Brightness, bool Center )
{
	ArtIdx idx;
	ArtHeader Header = { 0, DrawRect.Width(), DrawRect.Height() };
	short *LookupTable = NULL;
	long SeekPos = 0;
	short xOffset, xRun;
	short *Colors = NULL;
	COLORREF theCol;
	int ColorSize = 0;
	int x,y;
	int CenterX = 0, CenterY = 0;

	if ( !Idx || !Mul || Id == 0xFFFF || Id == 0 || Id == 0xFFFFFFFF ) 
		return false;

	Idx.seekg( (Id+0x4000)*sizeof(ArtIdx), ios::beg );
	Idx.read( (char *)&idx, sizeof(ArtIdx) );

	if ( idx.FilePos == -1 || idx.Length == -1 )
		return false;

	Mul.seekg( idx.FilePos, ios::beg );
	Mul.read( (char *)&Header, sizeof( ArtHeader ) );
	if ( Center )
	{
		CenterX = DrawRect.Width() / 2;
		CenterX -= Header.Width/2;
		//CenterX = max(0,CenterX);

		if ( DrawRect.Height() > Header.Height )
		{
			CenterY = DrawRect.Height() / 2;
			CenterY -= Header.Height / 2;
			CenterY = max(0, CenterY);
		}
	}
	
	Colors = new short[ColorSize=Header.Width]; //it should never be wider than the whole image!
	LookupTable = new short[Header.Height];
	Mul.read( (char *)LookupTable, sizeof(short)*Header.Height );
	SeekPos = Mul.tellg();

	for (y=0;y<Header.Height;y++)
	{
		if ( y > DrawRect.bottom ) break;

		Mul.seekg( SeekPos+(LookupTable[y]*2), ios::beg );
		x=0;
		do {
			Mul.read( (char *)&xOffset, 2 );
			Mul.read( (char *)&xRun, 2 );
			
			if ( xRun <= 0 && xOffset <= 0 ) 
				break;

			if ( ColorSize < xRun )
			{
				if ( Colors ) delete[] Colors;
				Colors = new short[xRun];
				ColorSize = xRun;
			}

			Mul.read( (char *)Colors, sizeof(short)*xRun );
			for (x+=xOffset;x<xRun+xOffset;x++)
			{
				if ( x > DrawRect.right ) break;
				theCol = Color16to32( (unsigned short)Colors[x-xOffset] );
				theCol = RGB( min(255,GetRValue(theCol)+Brightness), min(255,GetGValue(theCol)+Brightness), min(255,GetBValue(theCol)+Brightness) );

				pOutDC->SetPixel( x+CenterX, y+CenterY, theCol );
			}
		} while ( xOffset != 0 && xRun != 0 );
	}

	if ( Colors ) delete[] Colors;
	if ( LookupTable ) delete[] LookupTable;

	return true;
}
