#if !defined(AFX_MAPNAV_H__140559E3_D37F_11D5_A419_002078188525__INCLUDED_)
#define AFX_MAPNAV_H__140559E3_D37F_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapNav.h : header file
//

struct MapCell 
{ 
	unsigned short ColorIdx; 
	char Z; 
};

/////////////////////////////////////////////////////////////////////////////
// CMapNav window

class CMapNav : public CFrameWnd
{
// Construction
public:
	CMapNav();
	virtual ~CMapNav();

	void Show( CString, const char * = NULL );
	void GoTo( int x, int y );

	CPoint Center;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapNav)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	FILE *Map[2];
	signed char MaxZ;
	char WhichMap;
	COLORREF Radar[0x8000];
	CString GoCmd;
	CDC MemDC;
	CBitmap MemBMP;

	void Redraw( CDC * );
	void Redraw( void ) { CDC *pDC = GetDC();  Redraw(pDC); ReleaseDC(pDC); }

	//{{AFX_MSG(CMapNav)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPNAV_H__140559E3_D37F_11D5_A419_002078188525__INCLUDED_)
