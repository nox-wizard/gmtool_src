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
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog


CSplash::CSplash(CWnd* pParent /*=NULL*/)
	: CDialog(CSplash::IDD, pParent), SplashTextArea( 10, 253, 190, 263 )
{
//	strcpy( szMessage, "" );
	//{{AFX_DATA_INIT(CSplash)
	//}}AFX_DATA_INIT
	Font.CreateFont( 14, 0, 0, 0, FF_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Comic Sans MS" );
	Bmp.LoadBitmap( IDB_LOGO );
}

CSplash::~CSplash()
{
	Font.DeleteObject();
	if ( BmpDC.GetSafeHdc() )
		BmpDC.DeleteDC();
}

void CSplash::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSplash)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSplash, CDialog)
	//{{AFX_MSG_MAP(CSplash)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

#define TIMER_STARTUP 1
#define TIMER_PROG_UPDATE 2

/////////////////////////////////////////////////////////////////////////////
// CSplash message handlers

void CSplash::OnOK()
{
}

void CSplash::OnCancel()
{
}

BOOL CSplash::OnInitDialog() 
{
	CDialog::OnInitDialog();

//	pBlack = new CBrush( RGB(0,0,0) );
//	SetClassLong( GetSafeHwnd(), GCL_HBRBACKGROUND, (LONG)(pBlack->GetSafeHandle()) );
	ShowWindow( TRUE );
//	SetMessage( "Starting Accomplice...." );

#ifndef _DEBUG
	SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE );
#endif

	m_Progress.SetRange( SPLASH_MIN, SPLASH_MAX );
	m_Progress.SetPos( SPLASH_MIN );

	if ( m_bWait )
	{
		SetTimer( TIMER_PROG_UPDATE, 500/SPLASH_PS, NULL );
		SetTimer( TIMER_STARTUP, 500, NULL );
	} else {
		SetTimer( TIMER_PROG_UPDATE, SPLASH_PS, NULL );
		SetTimer( TIMER_STARTUP, SPLASH_PS, NULL );
	}

	CDC *pDC = GetDC();
	if ( BmpDC.GetSafeHdc() )
		BmpDC.DeleteDC();
	BmpDC.CreateCompatibleDC( pDC );
	BmpDC.SelectObject( Bmp );
	ReleaseDC( pDC );

	return TRUE;
}

void CSplash::OnTimer(UINT nIDEvent) 
{
	static int Prog = 0;

	if ( nIDEvent == TIMER_STARTUP )
	{
		Prog = 0;
		KillTimer( TIMER_STARTUP );
		KillTimer( TIMER_PROG_UPDATE );

		pSplash->Message( "Loading tabs..." );
		m_Progress.SetPos( SPLASH_STEP2 );
		if ( !theApp.LoadTabs() )
		{
			EndDialog(1);
			return;
		}

		pSplash->Message( "Initializing Controls..." );
		m_Progress.SetPos( SPLASH_STEP3 );
		if ( !pAccomplice->InitControls() )
		{
			EndDialog(1);
			return;
		}
		m_Progress.SetPos( SPLASH_MAX );
		
		pSplash->Message( "Accomplice Startup Complete." );
		if ( m_bWait ) Sleep( 250 );

		EndDialog(0);
		return;
	} else if ( nIDEvent == TIMER_PROG_UPDATE )
	{
		if ( Prog == 0 ) Message( "Initializing...." );
		m_Progress.SetPos( min(SPLASH_PS,(++Prog)*2) );
	}

	CDialog::OnTimer(nIDEvent);
}

void CSplash::Message( const char *Msg, ... )
{
	int len;
	char OutTxt[512];
	va_list argList;

	//sprintf the stuff from the ...
	va_start( argList, Msg );
	len = _vsnprintf( OutTxt, 512, Msg, argList );
	OutTxt[len] = 0;
	va_end( argList );

	CDC *pDC = GetDC();
	
	pDC->BitBlt( SplashTextArea.left, SplashTextArea.top, SplashTextArea.right, SplashTextArea.bottom , &BmpDC, SplashTextArea.left, SplashTextArea.top, SRCCOPY );
	pDC->SelectObject( Font );
	pDC->SetBkMode( TRANSPARENT );
	pDC->SetTextColor( RGB(255,255,255) );
	pDC->DrawText( OutTxt, len, &SplashTextArea, DT_VCENTER|DT_CENTER|DT_SINGLELINE|DT_END_ELLIPSIS );
	
	ReleaseDC( pDC );
}

int CSplash::DoModal( bool Wait ) 
{
	m_bWait = Wait;
	return CDialog::DoModal();
}

