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

#include "stdafx.h"
#include "accomplice.h"
#include "Options.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
{
	/*m_HexPrefix = FALSE;
	m_LinkUO = FALSE;
	m_CmdPrefix = _T("");
	m_AlwaysTop = FALSE;
	SysTray = FALSE;*/
	//{{AFX_DATA_INIT(COptions)
	//}}AFX_DATA_INIT
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(COptions)
	DDX_Check(pDX, IDC_HEX, PrefixHex);
	DDX_Check(pDX, IDC_LINK, Link2UO);
	DDX_Text(pDX, IDC_PREFIX, CmdPrefix);
	DDX_Check(pDX, IDC_TOP, AlwaysOnTop);
	DDX_Text(pDX, IDC_PREFIX, CmdPrefix);
	DDV_MaxChars(pDX, CmdPrefix, 2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	//{{AFX_MSG_MAP(COptions)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

void COptions::OnOK() 
{
	CDialog::OnOK();
	Save();
}

void COptions::OnCancel() 
{
	CDialog::OnCancel();
}

void COptions::Load( void )
{
	/*No errors on load here.  Instead just keep moving.  This file 
	shouldn't be user modified, so just keep us from crashing, and 
	let defaults take over for errored values. */

	ifstream Ini( "accomplice.ini" );
	if ( !Ini ) return;
	char Buffer[128], *Word;
	CRect Rect;
	const char *Delim = "=/,\n\r ";
	const char *PrefixDelim = "\n\r ";
		
	while ( !Ini.eof() && !Ini.fail() )
	{	
		Ini.getline( Buffer, 128 );
		if ( Buffer[0] == 0 || ( Buffer[0] == '/' && Buffer[1] == '/' ) )
			continue;

		Word = strtok(Buffer, Delim);
		if ( !strcmpi("Window",Word) )
		{
			if ( (Word = strtok(NULL,Delim)) )
				pAccomplice->PosX = atoi(Word);

			if ( (Word = strtok(NULL,Delim)) )
				pAccomplice->PosY = atoi(Word);
		} else if ( !strcmpi( "NavMap", Word ) )
		{
			Word=strtok(NULL,Delim);
			pNav->Center.x = atoi(Word);
			Word=strtok(NULL,Delim);
			pNav->Center.y = atoi(Word);
			
			Word=strtok(NULL,Delim);
			Rect.left = atoi(Word);
			Word=strtok(NULL,Delim);
			Rect.top = atoi(Word);
			Word=strtok(NULL,Delim);
			Rect.right = atoi(Word);
			Word=strtok(NULL,Delim);
			Rect.bottom = atoi(Word);
			pNav->SetWindowPos( NULL, Rect.left, Rect.top, Rect.Width(), Rect.Height(), SWP_NOZORDER );
		} else if ( !strcmpi("AlwaysTop",Word) )
		{
			if ( (Word = strtok(NULL,Delim)) )
			{
				if ( !strcmpi(Word,"TRUE") )
					AlwaysOnTop = TRUE;
				else
					AlwaysOnTop = FALSE;
			}
		} else if ( !strcmpi("Link2UO",Word) )
		{
			if ( (Word = strtok(NULL,Delim)) )
			{
				if ( !strcmpi(Word,"TRUE") )
					Link2UO = TRUE;
				else
					Link2UO = FALSE;
			}
		} else if ( !strcmpi("PrefixHex",Word) )
		{
			if ( (Word = strtok(NULL,Delim)) )
			{
				if ( !strcmpi(Word,"TRUE") )
					PrefixHex = TRUE;
				else
					PrefixHex = FALSE;
			}
		} else if ( !strcmpi("CommandPrefix",Word) )
		{
			CmdPrefix = strtok(NULL,PrefixDelim);
		} else {
			pSplash->MessageBox( "Error reading \"Accomplice.ini\"\nUnknown token.", "Ini Error", MB_OK|MB_ICONERROR );
		}
	}

	Ini.close();
}

void COptions::Save( void )
{
	CRect Rect;
	ofstream Ini( "Accomplice.ini" );
	if ( !Ini ) return;

	Ini << "// Accomplice Options Save File" << endl;
	Ini << "Window=" << max(0,pAccomplice->PosX-3) << "," << max(0,pAccomplice->PosY-22) << endl;
	Ini << "AlwaysTop=" << ( AlwaysOnTop ? "TRUE" : "FALSE" ) << endl;
	Ini << "Link2UO=" << ( Link2UO ? "TRUE" : "FALSE" ) << endl;
	Ini << "PrefixHex=" << ( PrefixHex ? "TRUE" : "FALSE" ) << endl;
	Ini << "CommandPrefix=" << (LPCSTR)CmdPrefix << endl;
	pNav->GetWindowRect( Rect );
	Ini << "NavMap=" << pNav->Center.x << "," << pNav->Center.y << "," <<
		Rect.left << "," << Rect.top << "," << Rect.right << "," << Rect.bottom << endl;

	Ini.close();
}
