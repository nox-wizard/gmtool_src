#if !defined(AFX_PROMPT_H__C5DAA904_ADC2_11D5_A419_002078188525__INCLUDED_)
#define AFX_PROMPT_H__C5DAA904_ADC2_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Prompt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrompt dialog

class CPrompt : public CDialog
{
// Construction
public:
	CPrompt(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPrompt)
	enum { IDD = IDD_PROMPT };
	CEdit	m_Value[8];
	CEdit	m_txtString;
	CString	m_PromptStr;
	//}}AFX_DATA

	CString csValue[8];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrompt)
	public:
	virtual int DoModal( int ValCount, char *Prompt, bool String );
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_nVal;

	// Generated message map functions
	//{{AFX_MSG(CPrompt)
	afx_msg BOOL OnInitDialog();
	afx_msg void OnOK();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROMPT_H__C5DAA904_ADC2_11D5_A419_002078188525__INCLUDED_)
