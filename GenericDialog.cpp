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
#include "GenericDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGenericDialog dialog


CGenericDialog::CGenericDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CGenericDialog::IDD, pParent), m_CurrentDlg(NULL)
{
	//{{AFX_DATA_INIT(CGenericDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CGenericDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenericDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenericDialog, CDialog)
	//{{AFX_MSG_MAP(CGenericDialog)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(LBN_DBLCLK, 1000, 10000, OnListDoubleClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenericDialog message handlers

int CGenericDialog::DoModal( Dialog *Script ) 
{
	int RetVal;
	m_CurrentDlg = Script;
	Control *ctrl;

	if ( Script == NULL )
	{
		pAccomplice->MessageBox( "Error creating dialog.\nTemplate was passed NULL.\nDialog will not be displayed.\n", "GenericDialog Error", MB_OK|MB_ICONERROR );
		return IDCANCEL;
	}

	RetVal = CDialog::DoModal();

	for (ctrl=m_CurrentDlg->First();!m_CurrentDlg->End();ctrl=m_CurrentDlg->Next())
	{
		if ( !ctrl ) continue;
		
		if ( ctrl->pObject )
		{
			switch ( ctrl->type )
			{
			case LABEL:
			case FRAME:
				delete (CStatic *)ctrl->pObject;
				break;

			case BUTTON:
				delete (CButton *)ctrl->pObject;
				break;

			case LISTBOX:
				delete (CPreviewList *)ctrl->pObject;
				break;

			default:
				delete ctrl->pObject;
				break;
			}
			ctrl->pObject = NULL;//this SHOULD have been deleted when the parent was destroyed
		}
	}

	return RetVal;
}

BOOL CGenericDialog::OnInitDialog() 
{
	Control *c = NULL;
	Label *l = NULL;
	Button *b = NULL;
	int id = 1000;
	CDialog::OnInitDialog();
	int x, y;
	x = max(0,pAccomplice->PosX+(pAccomplice->NormalSize.cx/2)-(m_CurrentDlg->MySize.cx/2)); //Mouse.x-(m_CurrentDlg->MySize.cx/2));
	y = max(0,Mouse.y-(m_CurrentDlg->MySize.cy/8));

	if ( x+m_CurrentDlg->MySize.cx > Screen.cx )
		x -= (x+m_CurrentDlg->MySize.cx)-Screen.cx-1;
	if ( y+m_CurrentDlg->MySize.cy > Screen.cy )
		y -= (y+m_CurrentDlg->MySize.cy)-Screen.cy-1;

	SetWindowText( m_CurrentDlg->GetName() );
	
	SetWindowPos( NULL, x, y, m_CurrentDlg->MySize.cx, m_CurrentDlg->MySize.cy, SWP_NOZORDER );
	Font.CreateFont( FontSize, 0, 0, 0, FF_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, csFontFace );
	SetFont( &Font );
	
	for (c=m_CurrentDlg->First();!m_CurrentDlg->End();c=m_CurrentDlg->Next())
	{
		if ( !c ) continue;

		switch (c->type)
		{
		case LABEL:
			c->pObject = (CWnd *)(new CStatic());
			((CStatic *)c->pObject)->Create( ((Label*)c)->szText, WS_CHILD, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this );
			c->pObject->SetFont( &Font );
			c->pObject->ShowWindow( TRUE );
			break;

		case BUTTON:
			c->pObject = (CWnd *)(new CButton());
			((Button*)c)->id = id;
			((CButton *)c->pObject)->Create( ((Button*)c)->szText, WS_CHILD|BS_PUSHBUTTON, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this, id++ );
			c->pObject->SetFont( &Font );
			c->pObject->ShowWindow( TRUE );
			break;

		case FRAME:
			c->pObject = (CWnd *)(new CButton());
			((CButton *)c->pObject)->Create( ((Frame*)c)->szText, WS_CHILD|BS_GROUPBOX, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this, 0xFFFF );
			c->pObject->ShowWindow( TRUE );
			c->pObject->SetFont( &Font );
			break;

		case LISTBOX:
			c->pObject = (CWnd *)(new CPreviewList((ListBox*)c));
			((ListBox*)c)->id = id;
			((CPreviewList *)c->pObject)->Create( LBS_OWNERDRAWFIXED|LBS_NOTIFY|
				LBS_NOINTEGRALHEIGHT|LBS_WANTKEYBOARDINPUT|WS_CHILD|WS_VSCROLL|WS_TABSTOP, 
				CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2),this,id++);//( WS_CHILD|WS_VSCROLL|LBS_NOTIFY|LBS_HASSTRINGS, CRect(c->x1,c->y1,c->x1+c->x2,c->y1+c->y2), this, id++ );
			((CPreviewList *)c->pObject)->PostCreate(c->x2, c->y2);

			((ListBox *)c)->InitList();
			c->pObject->SetFont( &Font );
			c->pObject->ShowWindow( TRUE );
			break;

		default:
			pAccomplice->MessageBox( "Error Initializing Controls!\nAn unknown control type was found, and cannot be created.", "Warning", MB_OK|MB_ICONINFORMATION );
			EndDialog(1);
			break;
		}
	}
	ShowWindow( TRUE );

	return TRUE;
}

BOOL CGenericDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int c = LOWORD(wParam);
	Control *ctrl;
	char Msg[256];

	for (ctrl=m_CurrentDlg->First();!m_CurrentDlg->End();ctrl=m_CurrentDlg->Next())
	{
		if ( !ctrl ) continue;

		if ( ctrl->id == c )
		{
			switch (ctrl->type)
			{
			case LABEL: break;
			case BUTTON:
				((Button *)ctrl)->OnPress( m_CurrentDlg, this );
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

void CGenericDialog::OnListDoubleClick( UINT nID ) 
{
	Control *ctrl;
	char Msg[256];

	for (ctrl=m_CurrentDlg->First();!m_CurrentDlg->End();ctrl=m_CurrentDlg->Next())
	{
		if ( !ctrl ) continue;

		if ( ctrl->id == nID )
		{
			switch (ctrl->type)
			{
			case LISTBOX:
				((ListBox *)ctrl)->OnDoubleClick( this, m_CurrentDlg );
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
