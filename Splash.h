#if !defined(AFX_SPLASH_H__66E2A184_AD38_11D5_A419_002078188525__INCLUDED_)
#define AFX_SPLASH_H__66E2A184_AD38_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Splash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplash dialog

class CSplash : public CDialog
{
// Construction
public:
	CSplash(CWnd* pParent = NULL);   // standard constructor
	~CSplash();

	void Message( const char *, ... );

// Dialog Data
	//{{AFX_DATA(CSplash)
	enum { IDD = IDD_SPLASH };
	CProgressCtrl	m_Progress;
	//}}AFX_DATA


//	void SetMessage( const char * );
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplash)
	public:
	virtual int DoModal(bool=true);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
// Implementation
protected:
	//CBrush *pBlack;
	//char szMessage[512];
	bool m_bWait;
	CFont Font;
	CBitmap Bmp;
	CDC BmpDC;

	CRect SplashTextArea;
	// Generated message map functions
	//{{AFX_MSG(CSplash)
	afx_msg void OnOK( void );
	afx_msg void OnCancel( void );
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLASH_H__66E2A184_AD38_11D5_A419_002078188525__INCLUDED_)
