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
#include "Accomplice.h"

CThisApp			theApp;
CMainDialog			*pAccomplice = NULL;
CArtMul				*pArt = NULL;
CMapNav				*pNav = NULL;
CSplash				*pSplash = NULL;
COptions			*pOptions = NULL;
vector<Tab *>		Tabs;
int					Col=1,Row=1,HSpace=0,VSpace=0;
CString				CmdPrefix = "\'";
CString				csFontFace = "MS Sans Serif";
bool				Horiz = false;
int					FontSize = 8;
BOOL				PrefixHex = FALSE;
BOOL				AlwaysOnTop = TRUE;
BOOL				Link2UO = FALSE;
CPoint				Mouse;
CSize				Screen;


BOOL CThisApp::InitInstance( void )
{
	//Create our window objects
	pSplash = new CSplash();
	//pAccomplice = new CMainDialog();
	pOptions = new COptions();
	pArt = new CArtMul();

	pAccomplice = new CMainDialog( IDD_ACCOMPLICE );
	pAccomplice->Create( IDD_ACCOMPLICE );
	pNav = new CMapNav();
	pOptions->Load();

	//returns 0 if no error
	if ( !pSplash->DoModal() )
	{
		pAccomplice->ShowWindow( TRUE );
		pAccomplice->Update(true);
		m_pMainWnd = m_pActiveWnd = (CWnd *)pAccomplice;
		return TRUE;
	} else {
		return FALSE;//false=stop execution
	}
}

int CThisApp::ExitInstance( void )
{
	int i;

	if ( pAccomplice )
	{
		if ( pAccomplice->GetSafeHwnd() )
			pAccomplice->ShowWindow( FALSE );
	}

	for (i=0;i<Tabs.size();i++)
	{
		if ( Tabs[i] )
		{
			delete Tabs[i];
			Tabs[i] = NULL;
		}
	}

	pOptions->Save();

	if ( pAccomplice )
	{
		if ( pAccomplice->GetSafeHwnd() )
			pAccomplice->DestroyWindow();

		delete pAccomplice;
		pAccomplice = NULL;
	}

	if ( pOptions )
	{
		delete pOptions;
		pOptions = NULL;
	}

	if ( pArt )
	{
		delete pArt;
		pArt = NULL;
	}

	if ( pNav )
	{
		pNav->DestroyWindow();
		pNav = NULL;
	}

	if ( pSplash )
	{
		delete pSplash;
		pSplash = NULL;
	}

	return CWinApp::ExitInstance();
}

bool CThisApp::LoadTabs( void )
{
	const char *Delim = ",\"=[]{}\n\r";
	char Buff[256], *part, *part2, Msg[256];
	ifstream inFile( "accomplice.cfg" );
	CString csInfo[4], ScriptInfo;
	int t=0;
	CSize LoadSize;
	float FileLen = 1, CurPos = 1;
	int LastProg = -1;

//	pSplash->SetMessage( "Loading script...." );

	if ( !inFile )
	{
		pSplash->MessageBox( "Unnable to locate \"accomplice.cfg\"\nAccomplice was unnable to find it's script.\nAccomplice cannot run.", "No Script", MB_OK|MB_ICONERROR );
		return false;
	}

	inFile.seekg( 0, ios::end );
	FileLen = (float)inFile.tellg();
	inFile.seekg( 0, ios::beg );

	while ( !inFile.eof() && !inFile.fail() )
	{
		inFile.getline( Buff, 256 );

		//skip blank lines & comments
		if ( Buff[0] == 0 || (Buff[0] == '/' && Buff[1] == '/') ) continue;
	
		if ( Buff[0] == '[' )//new tab
		{
			part=strtok(Buff, Delim);//get name
			pSplash->Message( "Loading tab \"%s\"...", part );
			Tabs.push_back( new Tab(part) );
/*			strcpy( Msg, "Processing tab \"" );
			strncat( Msg, part, 256 );
			strncat( Msg, "\"....", 256 );
			pSplash->SetMessage( Msg );*/
			Tabs[t]->Load( inFile, pSplash->m_Progress, FileLen );
			t++;
		} else {
			//this is a global tag, parse it as such
			part = strtok(Buff,Delim);
			part2 = strtok(NULL,Delim);

			if ( !part || !part2 )
			{
				pSplash->MessageBox( "WARNING: Token Error parsing globals in \"Accomplice.cfg\".", "Parse Error", MB_OK|MB_ICONERROR );
				continue;
			}

			if ( !strcmpi(part, "columns") )
				Col = atoi(part2);
			else if ( !strcmpi(part, "rows") )
				Row = atoi(part2);
			else if ( !strcmpi(part, "vspace") )
				VSpace = atoi(part2);
			else if ( !strcmpi(part, "hspace") )
				HSpace = atoi(part2);
			else if ( !strcmpi(part, "XSize") )
				pAccomplice->NormalSize.cx = atoi(part2);
			else if ( !strcmpi(part, "YSize") )
				pAccomplice->NormalSize.cy = atoi(part2);
			else if ( !strcmpi(part, "FontFace") )
				csFontFace = &Buff[9];
			else if ( !strcmpi(part, "FontSize") )
				FontSize = atoi(part2);
			else if ( !strcmpi(part, "name") )
				csInfo[0] = &Buff[5];
			else if ( !strcmpi(part, "version") )
				csInfo[1] = &Buff[8];
			else if ( !strcmpi(part, "creator") )
				csInfo[2] = &Buff[8];
			else if ( !strcmpi(part,"desc") )
				csInfo[3] = &Buff[5];
			else if ( !strcmpi(part,"HORIZONTAL") )
			{
				if ( !strcmpi(part2,"true") )
					Horiz = true;
			} else {
				strcpy( Msg, "Error in Accomplice.cfg:\nToken \"" );
				strncat( Msg, part, 256 );
				strncat( Msg, "\" undefined.\n", 256 );
				pSplash->MessageBox( Msg, "Cfg Error", MB_OK|MB_ICONERROR );
			}
		}
		CurPos = (float)inFile.tellg();
		if ( (int)((CurPos/FileLen)*SPLASH_PS) != LastProg && CurPos >= 0 )
		{
			LastProg = (int)((CurPos/FileLen)*SPLASH_PS);
			pSplash->m_Progress.SetPos( SPLASH_STEP2 + LastProg );
		}
	}

	if ( Horiz )
	{
		pAccomplice->m_Tabs.ModifyStyle( TCS_VERTICAL, 0, 0 );
		pAccomplice->m_Tabs.SetWindowPos( NULL, 0, 0, 800, 20, SWP_NOZORDER );
	}

	if ( Horiz )
		pAccomplice->HelpSize = CSize( 439, 156 );
	else
		pAccomplice->HelpSize = CSize( 156, 439 );


	ScriptInfo = "Name: " + csInfo[0] + "\nVersion: " + csInfo[1] + "\n\nCreator: " + csInfo[2] + "\n\nDescription: " + csInfo[3];
	pAccomplice->GetDlgItem(IDC_SCPINFO)->SetWindowText( ScriptInfo );
	
	inFile.close();
	return true;
}

void PostCommand( const char *szFormat, ... )
{
	//sprintf a command and send it to the client.

	char OutTxt[512];
	bool Loop = false;
	int i, len;
	HWND hClient = NULL;
	va_list argList;
	
	do {//loop until they cancel or we find the window
		Loop = false;
		hClient = FindUOWindow();//dll function
		if ( !hClient )
			Loop = ( pAccomplice->MessageBox( "Error! Unnable to locate Ultima Online client window.\nPlease start your UO Client.\n\nCommand not sent.", "Client Not Found", MB_RETRYCANCEL|MB_ICONERROR ) == IDRETRY );
	} while ( Loop );

	if ( !hClient ) return;//we didn't find the window

	//sprintf the stuff from the ...
	va_start( argList, szFormat );
	len = _vsnprintf( OutTxt, 512, szFormat, argList );
	va_end( argList );
	
	//send a return to make sure we are typing on a new blank line
	SendMessage( hClient, WM_CHAR, '\n', 0 );
	
	//send the prefix
	for (i=0;i<CmdPrefix.GetLength();i++)
		SendMessage( hClient, WM_CHAR, CmdPrefix[i], 0 );

	for (i=0;i<len;i++)
		SendMessage( hClient, WM_CHAR, OutTxt[i], 0 );

	//enter again to send command out.
	SendMessage( hClient, WM_CHAR, '\n', 0 );
}

void TrimDir( char *Dir )
{//remove the file name from the end of a Path String.
	int i, l=strlen(Dir);

	for (i=l-1;i>=0;i--)
	{
		if ( Dir[i] == '\\' || Dir[i] == '//' )
		{
			Dir[i+1] = 0;
			return;
		}
	}
}
