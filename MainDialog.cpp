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
#include "MainDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog


CMainDialog::CMainDialog(int id, CWnd* pParent /*=NULL*/)
	: CDialog(id, pParent),
		NormalSize( 156, 439 ), PosX(0), PosY(0)
{
	//{{AFX_DATA_INIT(CMainDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMainDialog::~CMainDialog()
{

}

void CMainDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainDialog)
	DDX_Control(pDX, IDC_SCPINFO, m_ScpInfo);
	DDX_Control(pDX, IDC_TAB, m_Tabs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMainDialog, CDialog)
	//{{AFX_MSG_MAP(CMainDialog)
	ON_WM_CLOSE()
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB, OnTabSelChanging)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabSelChanged)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_OPTIONS, OnOptions)
	ON_BN_CLICKED(IDC_RELOAD, OnReload)
	ON_MESSAGE(WM_CLIENTSHOW,OnClientShow)
	ON_MESSAGE(WM_CLIENTFOCUS,OnClientFocus)
	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(LBN_DBLCLK, 1000, 0xFFFE, OnListDoubleClick)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMainDialog message handlers

void CMainDialog::OnOK() { }
void CMainDialog::OnCancel() { }

void CMainDialog::OnClose() 
{
	PostQuitMessage(0);
	CDialog::OnClose();
}

BOOL CMainDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CDC ScreenDC;
	ScreenDC.CreateDC( "DISPLAY", NULL, NULL, NULL );
	Screen.cx = ScreenDC.GetDeviceCaps( HORZRES );
	Screen.cy = ScreenDC.GetDeviceCaps( VERTRES );
	ScreenDC.DeleteDC();
	
	ShowWindow( FALSE );
	//SetWindowPos( &wndNoTopMost, PosX, PosY, NormalSize.cx, NormalSize.cy, SWP_NOZORDER );
	SetIcon( theApp.LoadIcon(IDI_ACCOMPLICE), FALSE );

	return TRUE;
}

bool CMainDialog::InitControls( void )
{
	Control *c = NULL;
	Label *l = NULL;
	Button *b = NULL;
	int t, id;

	Font.CreateFont( FontSize, 0, 0, 0, FF_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, csFontFace );
	SetFont( &Font, FALSE );

	if ( Horiz )
		m_Tabs.SetFont( &Font );
	
	for (t=0;t<Tabs.size();t++)
	{
		id = (t+1)*1000;
		m_Tabs.InsertItem( t, Tabs[t]->GetName() );
		pSplash->Message( "Initializing tab \"%s\"...", Tabs[t]->GetName() );

		for (c=Tabs[t]->First();!Tabs[t]->End();c=Tabs[t]->Next())
		{
			switch (c->type)
			{
			case LABEL:
				c->pObject = (CWnd *)(new CStatic());
				((CStatic *)c->pObject)->Create( ((Label*)c)->szText, WS_CHILD, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this );
				c->pObject->ShowWindow( FALSE );
				c->pObject->SetFont( &Font, FALSE );
				break;

			case BUTTON:
				c->pObject = (CWnd *)(new CButton());
				((Button*)c)->id = id;
				((CButton *)c->pObject)->Create( ((Button*)c)->szText, WS_CHILD|BS_PUSHBUTTON, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this, id++ );
				c->pObject->ShowWindow( FALSE );
				c->pObject->SetFont( &Font, FALSE );
				break;

			case FRAME:
				c->pObject = (CWnd *)(new CButton());
				((CButton *)c->pObject)->Create( ((Frame*)c)->szText, WS_CHILD|BS_GROUPBOX, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this, 0xFFFF );
				c->pObject->ShowWindow( FALSE );
				c->pObject->SetFont( &Font, FALSE );
				break;

			case LISTBOX:
				c->pObject = (CWnd *)(new CPreviewList((ListBox*)c));//CListBox());
				((ListBox*)c)->id = id;
				((CPreviewList *)c->pObject)->Create( LBS_OWNERDRAWFIXED|LBS_NOTIFY|
					LBS_NOINTEGRALHEIGHT|LBS_WANTKEYBOARDINPUT|WS_VSCROLL|WS_CHILD|WS_TABSTOP, 
					CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2),this,id++);//( WS_CHILD|WS_VSCROLL|LBS_NOTIFY|LBS_HASSTRINGS, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this, id++ );
				((CPreviewList *)c->pObject)->PostCreate( c->x2, c->y2 );
				c->pObject->ShowWindow( FALSE );

				((ListBox *)c)->InitList();
				c->pObject->SetFont( &Font, FALSE );
				break;

			default:
				pSplash->MessageBox( "Error Initializing Controls!\nAn unknown control type was found, and cannot be created.", "Warning", MB_OK|MB_ICONINFORMATION );
				break;
			}
		}
		pSplash->m_Progress.SetPos( SPLASH_STEP3 + ((SPLASH_PS/Tabs.size())*t) );
	}

	if ( !Horiz )
	{
		m_Tabs.InsertItem( t, "Help" );
		m_HelpTab = t;
	} else {
		m_HelpTab = -2;
	}
	
	return true;
}


void CMainDialog::OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int t = m_Tabs.GetCurSel();
	*pResult = 0;

	m_Tabs.SetFocus();
	m_Tabs.SetForegroundWindow();
	if ( t == m_HelpTab )
		HelpHide();
	else if ( t >= 0 && t < Tabs.size() )
		Tabs[t]->HideControls();
	else
		*pResult = 1;
}


void CMainDialog::OnTabSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int t = m_Tabs.GetCurSel();
	*pResult = 0;

	if ( t == m_HelpTab )
		HelpShow();
	else if ( t >= 0 && t < Tabs.size() )
		Tabs[t]->ShowControls();
	else
		*pResult = 1;

	SendMessage( WM_PAINT );
}

void CMainDialog::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	int c = m_Tabs.GetCurSel();
	CDialog::OnShowWindow(bShow, nStatus);
	
	if ( bShow )
	{
		if ( Tabs.size() )
		{
			if ( c == -1 )
			{
				m_Tabs.SetCurSel( 0 );
				Tabs[0]->ShowControls();
			} else {
				if ( c == m_HelpTab )
					HelpShow();
				else
					Tabs[c]->ShowControls();
			}
		}
	}
}

BOOL CMainDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int c = LOWORD(wParam);
	int t = (c/1000)-1;
	Control *ctrl;
	char Msg[256];

	if ( t<0 || t>Tabs.size() )
		return CDialog::OnCommand(wParam, lParam);

	for (ctrl=Tabs[t]->First();!Tabs[t]->End();ctrl=Tabs[t]->Next())
	{
		if ( !ctrl ) continue;

		if ( ctrl->id == c )
		{
			switch (ctrl->type)
			{
			case LABEL: break;
			case BUTTON:
				((Button *)ctrl)->OnPress(Tabs[t],pAccomplice);
				break;
			case FRAME: break;
			case LISTBOX: break;
			default:
				sprintf( Msg, "Unknown command received.\nControl ID: %i\nControl Type: %i\n", c, ctrl->type );
				MessageBox( Msg, "Control Error", MB_OK|MB_ICONERROR );
				break;
			}

			break;
		}
	}

	return CDialog::OnCommand(wParam, lParam);
}

void CMainDialog::OnListDoubleClick( UINT nID ) 
{
	int t = (nID/1000)-1;
	Control *ctrl;
	char Msg[256];

	if ( t<0 || t>=Tabs.size() ) return;
	if ( !Tabs[t] ) return;

	for (ctrl=Tabs[t]->First();!Tabs[t]->End();ctrl=Tabs[t]->Next())
	{
		if ( !ctrl ) continue;

		if ( ctrl->id == nID )
		{
			switch (ctrl->type)
			{
			case LISTBOX:
				((ListBox *)ctrl)->OnDoubleClick( this, Tabs[t] );
				break;
			default:
				sprintf( Msg, "Unknown notification received.\nControl ID: %i\nControl Type: %i\n", nID, ctrl->type );
				MessageBox( Msg, "Control Error", MB_OK|MB_ICONERROR );
				break;
			}

			break;
		}
	}
}


void CMainDialog::OnOptions() 
{
	pOptions->DoModal();
	Update();
}

void CMainDialog::OnReload() 
{
	int i;

	for (i=0;i<Tabs.size();i++)
	{
		if ( Tabs[i] )
		{
			delete Tabs[i];
			Tabs[i] = NULL;
		}
	}
	Tabs.clear();
	m_Tabs.DeleteAllItems();
	HelpHide();

	Font.DeleteObject();

	if ( pSplash->DoModal( false ) )
		OnClose();
	m_Tabs.SetCurSel( m_HelpTab );
	HelpShow();
}

void CMainDialog::HelpShow( void )
{
	int i;
	CRect Rect;
	CSize Size;
//	CWnd *ctrl;

	for (i=990;i<998;i++)
		GetDlgItem(i)->ShowWindow( TRUE );
/*	{
		ctrl = GetDlgItem(i);
		ctrl->SetFont( &Font, FALSE );
		ctrl->ShowWindow( TRUE );
	}*/

	GetWindowRect( Rect );
	Size = Rect.Size();
	if ( Size != HelpSize )
		SetWindowPos( NULL, 0, 0, HelpSize.cx, HelpSize.cy, SWP_NOZORDER|SWP_NOMOVE );
}

void CMainDialog::HelpHide( void )
{
	int i;
	CRect Rect;
	CSize Size;

	for (i=990;i<998;i++)
		GetDlgItem(i)->ShowWindow( FALSE );

	GetWindowRect( Rect );
	Size = Rect.Size();
	if ( Size != NormalSize )
		SetWindowPos( NULL, 0, 0, NormalSize.cx, NormalSize.cy, SWP_NOZORDER|SWP_NOMOVE );
}

void CMainDialog::Update( bool FirstShow )
{
	HWND hUO = ::FindUOWindow();

	if ( AlwaysOnTop )
		SetWindowPos( &wndTopMost, PosX, PosY, NormalSize.cx, NormalSize.cy, FirstShow ? 0 : SWP_NOMOVE );
	else
		SetWindowPos( &wndNoTopMost, PosX, PosY, NormalSize.cx, NormalSize.cy, FirstShow ? 0 : SWP_NOMOVE );

	if ( Link2UO )
		InstallMaxMinHook( GetSafeHwnd(), hUO, WM_CLIENTSHOW, WM_CLIENTFOCUS );
	else
		RemoveMaxMinHook();
}

LRESULT CMainDialog::OnClientShow( WPARAM wParam, LPARAM lParam )
{
	if ( LOWORD(wParam) == WA_INACTIVE && HIWORD(wParam) && !IsIconic() )
		ShowWindow( SW_MINIMIZE );
	else if ( LOWORD(wParam) && HIWORD(wParam) && IsIconic() )
	{
		ShowWindow( SW_SHOWNOACTIVATE );
			
		::SetForegroundWindow( FindUOWindow() );
	}

	return 0;
}

LRESULT CMainDialog::OnClientFocus( WPARAM ClientHasFocus, LPARAM Wnd )
{
	if ( ClientHasFocus )
		SetWindowPos( &wndTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
	else
	{
		SetWindowPos( &wndNoTopMost, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
		SetWindowPos( &wndBottom, 0,0,0,0, SWP_NOMOVE|SWP_NOSIZE );
	}
	
	return 0;
}

void CMainDialog::OnMove(int x, int y) 
{
	CDialog::OnMove(x, y);
	
	if ( !IsIconic() )
	{
		PosX = x;
		PosY = y;
	}
}

/*
LRESULT CMainDialog::OnShellIcon( WPARAM nIcon, LPARAM nMsg )
{
	if ( nIcon == 1017 && ( nMsg == WM_RBUTTONDOWN || nMsg == WM_LBUTTONDOWN ) )
	{
		CMenu Popup;
		SetForegroundWindow(); SetFocus();
		Popup.CreatePopupMenu();
		Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1025, "Open Accomplice Options" );
		Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1026, "Reload Script" );
		if ( bHidden )
			Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1027, "Show Accomplice" );
		else
			Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1027, "Hide Accomplice" );
		Popup.AppendMenu( MF_BYPOSITION|MF_SEPARATOR );
		Popup.AppendMenu( MF_BYPOSITION|MF_STRING, 1028, "Close Accomplice" );
		Popup.SetDefaultItem( 1026 );

		switch ( Popup.TrackPopupMenu( TPM_LEFTALIGN|TPM_BOTTOMALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD, Screen.cx-75, Screen.cy-20, this ) )
		{
		case 1025://config
			OnOptions();
			break;

		case 1026:
			OnReload();
			break;

		case 1027:
			if ( bHidden )
			{
				SetForegroundWindow();
				SetFocus();
				ShowWindow( TRUE );
				bHidden = false;
			} else {
				ShowWindow( FALSE );
				bHidden = true;
			}
			break;

		case 1028://quit
			OnClose();
			break;
		}
		Popup.DestroyMenu();
	} else if ( nIcon == 1017 && nMsg == WM_LBUTTONDBLCLK )
	{
		bHidden = !bHidden;
		if ( bHidden )
			ShowWindow( FALSE );
		else {
			SetForegroundWindow();
			SetFocus();
			ShowWindow( TRUE );
		}
	}

	return TRUE;
}*/

void CMainDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect Rect;
	CDialog::OnMouseMove(nFlags, point);

	GetWindowRect( Rect );
	Mouse=CPoint(point.x+Rect.left,point.y+Rect.top);
}
