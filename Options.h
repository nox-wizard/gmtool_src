#if !defined(AFX_OPTIONS_H__3307FA81_B054_11D5_A419_002078188525__INCLUDED_)
#define AFX_OPTIONS_H__3307FA81_B054_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptions dialog
struct path
{
	char str[MAX_PATH];
};

class COptions : public CDialog
{
// Construction
public:
	COptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptions)
	enum { IDD = IDD_OPTIONS };
	//}}AFX_DATA

	void Load();
	void Save();
	void Update();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	vector<path> Scripts;

	// Generated message map functions
	//{{AFX_MSG(COptions)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_H__3307FA81_B054_11D5_A419_002078188525__INCLUDED_)
