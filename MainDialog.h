#if !defined(AFX_MAINDIALOG_H__66E2A185_AD38_11D5_A419_002078188525__INCLUDED_)
#define AFX_MAINDIALOG_H__66E2A185_AD38_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MainDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMainDialog dialog

class CMainDialog : public CDialog
{
// Construction
public:
	CMainDialog(int id, CWnd* pParent = NULL);   // standard constructor
	~CMainDialog();
	CSize NormalSize;
	CSize HelpSize;
	int PosX, PosY;

// Dialog Data
	//{{AFX_DATA(CMainDialog)
	CStatic	m_ScpInfo;
	CTabCtrl	m_Tabs;
	//}}AFX_DATA

	void Update( bool = false );
	bool InitControls( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont Font;
	int m_HelpTab;
	int IDD;

	void HelpShow();
	void HelpHide();

	// Generated message map functions
	//{{AFX_MSG(CMainDialog)
	afx_msg void OnClose();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTabSelChanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTabSelChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnOptions();
	afx_msg void OnReload();
	afx_msg LRESULT OnClientShow( WPARAM, LPARAM );
	afx_msg LRESULT OnClientFocus( WPARAM, LPARAM );
	afx_msg void OnMove(int x, int y);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	afx_msg void OnListDoubleClick( UINT );
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINDIALOG_H__66E2A185_AD38_11D5_A419_002078188525__INCLUDED_)
