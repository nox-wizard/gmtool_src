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
#include "MapNav.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapNav

CMapNav::CMapNav()
{
	HKEY hKey = NULL;
	DWORD dwSize, dwType;
	int Result, i;
	short c;
	char szPath[256];
	WhichMap = 0;
	ifstream RadarCol;
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
		RadarCol.open( (csPath + "RadarCol.mul"), ios::in|ios::binary );
		if ( RadarCol )
		{
			for (i=0;i<0x8000;i++)
			{
				RadarCol.read( (char *)&c, sizeof(short) );
				Radar[i] = Color16to32(c);
			}
			RadarCol.close();

			Map[0] = fopen( (csPath + "Map0.mul"), "rb" );
			Map[1] = fopen( (csPath + "Map2.mul"), "rb" );
		} else {
			Map[0] = NULL;
			Map[1] = NULL;
		}
	}

	CreateEx( 0, NULL, "Map Navigator", WS_OVERLAPPED|WS_SYSMENU|WS_CAPTION|WS_THICKFRAME|WS_MINIMIZEBOX, CRect(0,0,250,250), NULL, 0 );
	SetIcon( theApp.LoadIcon(IDI_ACCOMPLICE), FALSE );
	Center.x = 0; Center.y = 0;
	ShowWindow( FALSE );
	CDC *pDC = GetDC();
	MemDC.CreateCompatibleDC( pDC );
	MemBMP.CreateCompatibleBitmap( pDC, 1000, 1000 );
	MemDC.SelectObject( MemBMP );
	MemDC.FillSolidRect( 0, 0, 1000, 1000, 0 );
	ReleaseDC( pDC );
}

CMapNav::~CMapNav()
{
	if ( Map[0] ) fclose( Map[0] );
	if ( Map[1] ) fclose( Map[1] );
	pNav = NULL;
}


BEGIN_MESSAGE_MAP(CMapNav, CWnd)
	//{{AFX_MSG_MAP(CMapNav)
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapNav message handlers

void CMapNav::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch ( nChar )
	{
	case VK_UP:	Center.y -= 25; Redraw(); break;
	case VK_DOWN: Center.y += 25;  Redraw(); break;
	case VK_LEFT: Center.x -= 25;  Redraw(); break;
	case VK_RIGHT: Center.x += 25;  Redraw(); break;
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapNav::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect r;
	GetClientRect(r);
	Center.x += point.x-r.Width()/2;
	Center.y += point.y-r.Height()/2;
	Redraw();
}

void CMapNav::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CRect r;
	GetClientRect(r);
	//GoTo( Center.x, Center.y );
	GoTo( Center.x+point.x-r.Width()/2, Center.y + point.y-r.Height()/2 );
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMapNav::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CMenu Popup;
	char szTemp[256];
	CPrompt prompt(this);
	CRect r; GetClientRect(r);
	
	Popup.CreatePopupMenu();
	sprintf( szTemp, "Send Client to (%i,%i)", Center.x+point.x-r.Width()/2, Center.y+point.y-r.Height()/2 );
	Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1005, szTemp );
	
	Popup.AppendMenu( MF_SEPARATOR );
	sprintf( szTemp, "Center Map here (%i,%i)", Center.x+point.x-r.Width()/2, Center.y+point.y-r.Height()/2 );
	Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1001, szTemp );
	Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1002, "Enter new Map Choords..." );
	/*if ( WhichMap == 0 && Map[1] )
		Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1006, "Switch to Map2.mul" );
	else if ( WhichMap == 1 && Map[0] )
		Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1006, "Switch to Map0.mul" );
	else*/
		Popup.AppendMenu( MF_BYPOSITION|MF_STRING|MF_GRAYED, 1006, "Switch to Map2.mul" );

	Popup.AppendMenu( MF_SEPARATOR );
	Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1003, "Repaint Display" );
	Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1007, "Redraw Display" );
	
	Popup.AppendMenu( MF_SEPARATOR );
	Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1004, "Close Nav Window" );

	GetWindowRect(r);
	Popup.SetDefaultItem( 1005 );
	switch ( Popup.TrackPopupMenu( TPM_LEFTALIGN|TPM_TOPALIGN|TPM_NONOTIFY|TPM_RETURNCMD, point.x+r.left+3, point.y+r.top+21, this ) )
	{
	case 1000://Goto ...
		GoTo( Center.x, Center.y );
		break;
		
	case 1001://Recenter
		OnLButtonDown( nFlags, point );
		break;

	case 1002://Recenter Prompt
		if ( prompt.DoModal( 2, "Enter New (x,y) Choords to center map:", false ) == IDOK )
		{
			Center.x = atoi(prompt.csValue[0]);
			Center.y = atoi(prompt.csValue[1]);
			Redraw();
		}
		break;

	case 1003://Repaint
		SendMessage( WM_PAINT );
		break;

	case 1004://Hide
		ShowWindow( FALSE );
		break;

	case 1005://Goto offset
		GetClientRect(r);
		GoTo( Center.x+point.x-r.Width()/2, Center.y + point.y-r.Height()/2 );
		break;

	case 1006://Switch Maps
		WhichMap = ( WhichMap == 0 ? 1 : 0 );
		Center.x = Center.y = 0;
		Redraw();
		break;

	case 1007://Redraw
		Redraw();
		break;
	}
}

void CMapNav::OnPaint() 
{
	CPaintDC dc(this);
	CRect r; GetClientRect(r);
	//Redraw( &dc );
	dc.BitBlt( 0, 0, r.Width(), r.Height(), &MemDC, 0, 0, SRCCOPY );
}

void CMapNav::Redraw( CDC *pDC )
{
	const MapHeight[2] = { 512, 288 };
	CRect r; GetClientRect(r);
	int Width=r.Width(), Height=r.Height();
	register int x, y;
	register int TopX = max(0,Center.x - (Width/2)), TopY = max(0,Center.y - (Height/2));
	int FirstXB = TopX/8, FirstYB = TopY/8;
	int LastXB = (r.Width()+TopX)/8+1, LastYB = (r.Height()+TopY)/8+1;
	register int xBlock, yBlock, xCell, yCell;
	char szTitle[64];
	MaxZ = -128;
	MapCell Block[8][8];
	register HDC hDC = pDC->GetSafeHdc();
	LARGE_INTEGER freq, start, end;
	
	QueryPerformanceCounter( &start );
	QueryPerformanceFrequency( &freq );
	
	if ( !Map[WhichMap] ) return;

	SetWindowText( "Map Navigator - Working...." );

	for (xBlock=FirstXB;xBlock<LastXB;xBlock++)
	{
		fseek( Map[WhichMap], ((xBlock * MapHeight[WhichMap]) + FirstYB) * 196, SEEK_SET );
		for (yBlock=FirstYB;yBlock<LastYB;yBlock++)
		{
			fseek( Map[WhichMap], 4, SEEK_CUR );
			fread( Block, 1, 3*8*8, Map[WhichMap] );
			for (yCell=0;yCell<8;yCell++)
			{
				for (xCell=0;xCell<8;xCell++)
				{
					x = ((xBlock<<3)+xCell)-TopX;
					y = ((yBlock<<3)+yCell)-TopY;
					if ( x < 0 || x >= Width || y < 0 || y >= Height )
						continue;
					
					if ( Block[yCell][xCell].ColorIdx >= 0x8000 )
						::SetPixel( hDC, x, y, 0 );
					else
						::SetPixel( hDC, x, y, Radar[Block[yCell][xCell].ColorIdx] ); //::SetPixel( pDC->m_hDC, ((xBlock<<3)+xCell)-TopX, ((yBlock<<3)+yCell)-TopY, Radar[RadIndex] );
					MaxZ = min(25,max(MaxZ, Block[yCell][xCell].Z ));
				}
			}
		}
	}

	pDC->SetPixel( Width/2+1, Height/2, RGB(255,0,0) );
	pDC->SetPixel( Width/2-1, Height/2, RGB(255,0,0) );
	pDC->SetPixel( Width/2, Height/2, RGB(255,0,0) );
	pDC->SetPixel( Width/2, Height/2+1, RGB(255,0,0) );
	pDC->SetPixel( Width/2, Height/2-1, RGB(255,0,0) );
	QueryPerformanceCounter( &end );

	_snprintf( szTitle, 63, "Map Navigator - (%i, %i, %i) [Time: %.03fs]", Center.x, Center.y, MaxZ, (float(end.QuadPart-start.QuadPart)/float(freq.QuadPart)) );
	SetWindowText( szTitle );

	MemDC.BitBlt( 0, 0, Width, Height, pDC, 0, 0, SRCCOPY );
}

void CMapNav::Show( CString Cmd, const char *Pos )
{
	static bool Reset = true;

	int x=0, y=0;
	char *p;
	char *t;
	if ( Pos != NULL )
	{
		t = new char[strlen(Pos)+1];
		strcpy( t, Pos );
		p = strtok( t, " ,\n\r" );
		if ( p )
		{
			x = atoi(p);
			p = strtok( NULL, " ,\n\r" );

			if ( p )
				y = atoi(p);
		}
		delete[] t;
		Center = CPoint(x,y);
	}
	
	GoCmd = Cmd;
	ShowWindow( TRUE );
	Redraw();
}

void CMapNav::OnClose() 
{
	ShowWindow( FALSE );
}

void CMapNav::GoTo( int x, int y )
{
	PostCommand( "%s %i %i %i", (LPCSTR)GoCmd, Center.x, Center.y, MaxZ );
}

void CMapNav::OnSize(UINT nType, int cx, int cy) 
{
	static int ox=cx, oy=cy;
	CWnd::OnSize(nType, cx, cy);
	
	if ( ox < cx || oy < cy ) Redraw();
	ox=cx; oy=cy;
}

