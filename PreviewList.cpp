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
#include "PreviewList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPreviewList

CPreviewList::CPreviewList( ListBox *List )
{
	pList = List;
	LastDraw = -1;
}

CPreviewList::~CPreviewList()
{
	MemDC.SelectObject( (CGdiObject*)NULL );
}


BEGIN_MESSAGE_MAP(CPreviewList, CListBox)
	//{{AFX_MSG_MAP(CPreviewList)
	ON_CONTROL_REFLECT(LBN_SELCHANGE, OnSelChange)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPreviewList message handlers

void CPreviewList::PostCreate( int sx, int sy )
{
	ModifyStyleEx( 0, WS_EX_CLIENTEDGE );
		
	CDC *pDC = GetDC();
	MemDC.CreateCompatibleDC( pDC );
	MemBMP.CreateCompatibleBitmap( pDC, sx, sy );
	MemDC.SelectObject( MemBMP );
	MemDC.FillSolidRect( 0, 0, sx, sy, RGB(255,255,255) );
	ReleaseDC( pDC );

	GetClientRect( ClientRect );

	HighlightPen.CreatePen( PS_SOLID, 1, ::GetSysColor(COLOR_HIGHLIGHT) );
}

void CPreviewList::DrawItem( LPDRAWITEMSTRUCT pDrawItem ) 
{
	if ( !pDrawItem ) return;
	if ( !pDrawItem->itemData || !pDrawItem->hDC ) return;

	LPCTSTR lpszText = (LPCTSTR)pDrawItem->itemData;
	CRect r(pDrawItem->rcItem);
	CDC dc;
	
	dc.Attach(pDrawItem->hDC);
	int OldMode = dc.SetBkMode( TRANSPARENT );
	int OldColor = dc.SetTextColor( 0 );
	
	dc.BitBlt( r.left, r.top, r.Width(), r.Height(), &MemDC, r.left, r.top, SRCCOPY ); 
	if ( pDrawItem->itemState & ODS_SELECTED )
	{
		dc.SelectObject( &HighlightPen );
		dc.MoveTo( pDrawItem->rcItem.left, pDrawItem->rcItem.top+1 );
		dc.LineTo( pDrawItem->rcItem.right-1, pDrawItem->rcItem.top+1 );
		dc.LineTo( pDrawItem->rcItem.right-1, pDrawItem->rcItem.bottom-1 );
		dc.LineTo( pDrawItem->rcItem.left, pDrawItem->rcItem.bottom-1 );
		dc.LineTo( pDrawItem->rcItem.left, pDrawItem->rcItem.top+1 );
		dc.SelectObject( (CGdiObject *)NULL );
	}
	dc.DrawText( lpszText, strlen(lpszText), &pDrawItem->rcItem, DT_SINGLELINE|DT_END_ELLIPSIS|DT_VCENTER );
	
	dc.SetBkMode( OldMode );
	dc.SetTextColor( OldColor );
	dc.Detach();
}

void CPreviewList::MeasureItem( LPMEASUREITEMSTRUCT pMeasureItem ) 
{
	return;//we don't need to specify any special size (but we must provide this function
}

void CPreviewList::OnSelChange() 
{
	DoArt( pList->GetArtID( GetCurSel() ) );	
}

void CPreviewList::DoArt( int Index )
{
	if ( Index != LastDraw )
	{
		LastDraw = Index;
		MemDC.FillSolidRect( ClientRect, RGB(255,255,255) );
		pArt->GetTile( Index, &MemDC, ClientRect );
		Redraw();
	}
}

void CPreviewList::Redraw( void )
{
	InvalidateRect( ClientRect );
}

void CPreviewList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	Redraw();

	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CPreviewList::Clear( void )
{
	MemDC.FillSolidRect( ClientRect, RGB(255,255,255) );
	ResetContent();
}
