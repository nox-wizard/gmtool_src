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
#include "Prompt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrompt dialog


CPrompt::CPrompt(CWnd* pParent /*=NULL*/)
	: CDialog(CPrompt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPrompt)
	m_PromptStr = _T("");
	//}}AFX_DATA_INIT
}


void CPrompt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrompt)
	DDX_Control(pDX, IDC_EDIT8, m_Value[7]);
	DDX_Control(pDX, IDC_EDIT7, m_Value[6]);
	DDX_Control(pDX, IDC_EDIT6, m_Value[5]);
	DDX_Control(pDX, IDC_EDIT5, m_Value[4]);
	DDX_Control(pDX, IDC_EDIT4, m_Value[3]);
	DDX_Control(pDX, IDC_EDIT3, m_Value[2]);
	DDX_Control(pDX, IDC_EDIT2, m_Value[1]);
	DDX_Control(pDX, IDC_EDIT1, m_Value[0]);
	DDX_Text(pDX, IDC_PROMPT, m_PromptStr);
	DDX_Control(pDX, IDC_STRING, m_txtString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrompt, CDialog)
	//{{AFX_MSG_MAP(CPrompt)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrompt message handlers

int CPrompt::DoModal( int ValCount, char *Prompt, bool String )
{
	if ( String ) 
		m_nVal = 9 + max(0,min(4,ValCount-1));
	else 
		m_nVal = max(1,min(ValCount,8));

	if ( Prompt )
		m_PromptStr = Prompt;
	else
		m_PromptStr.Format( "Value%srequired for this command:", m_nVal > 1 ? "s " : " " );

	return CDialog::DoModal();
}

BOOL CPrompt::OnInitDialog( void )
{
	int i;
	CDialog::OnInitDialog();

	ShowWindow( TRUE );
	if ( m_nVal < 9 )
	{
		for (i=0;i<m_nVal;i++)
			m_Value[i].ShowWindow( TRUE );
		for (;i<8;i++)
			m_Value[i].ShowWindow( FALSE );
		m_txtString.ShowWindow( FALSE );
		m_Value[0].SetFocus();
	} else {
		for (i=0;i<m_nVal-9;i++)
			m_Value[i].ShowWindow( TRUE );
		for (;i<8;i++)
			m_Value[i].ShowWindow( FALSE );
		m_txtString.ShowWindow( TRUE );
		if ( m_nVal > 9 )
			m_Value[0].SetFocus();
		else
			m_txtString.SetFocus();
	}

	return FALSE;
}

void CPrompt::OnOK( void )
{
	int i;
	if ( m_nVal < 9 )
	{
		for (i=0;i<8;i++)
			m_Value[i].GetWindowText( csValue[i] );
	} else {
		for (i=0;i<m_nVal-9;i++)
			m_Value[i].GetWindowText( csValue[i] );
		m_txtString.GetWindowText( csValue[i] );
	}

	CDialog::OnOK();
}
