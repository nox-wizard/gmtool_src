#if !defined(AFX_GENERICDIALOG_H__C062AEC1_B29D_11D5_A419_002078188525__INCLUDED_)
#define AFX_GENERICDIALOG_H__C062AEC1_B29D_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GenericDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenericDialog dialog

class CGenericDialog : public CDialog
{
// Construction
public:
	CGenericDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CGenericDialog)
	enum { IDD = IDD_GENERIC };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenericDialog)
	public:
	virtual int DoModal( Dialog * );
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	Dialog *m_CurrentDlg;
	CFont Font;

	void OnListDoubleClick( UINT );

	// Generated message map functions
	//{{AFX_MSG(CGenericDialog)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERICDIALOG_H__C062AEC1_B29D_11D5_A419_002078188525__INCLUDED_)
