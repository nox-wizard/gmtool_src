#if !defined(AFX_PREVIEWLIST_H__35AFFA27_D2B0_11D5_A419_002078188525__INCLUDED_)
#define AFX_PREVIEWLIST_H__35AFFA27_D2B0_11D5_A419_002078188525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PreviewList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPreviewList window

class CPreviewList : public CListBox
{
// Construction
public:
	CPreviewList(ListBox*);

// Attributes
public:

// Operations
public:
	void PostCreate( int, int );
	void Clear( void );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPreviewList)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPreviewList();

	// Generated message map functions
protected:
	CDC MemDC;
	CBitmap MemBMP;
	ListBox *pList;
	CPen HighlightPen;
	CRect ClientRect;
	int LastDraw;

	void DoArt( int );
	void Redraw( void );

	//{{AFX_MSG(CPreviewList)
	afx_msg void OnSelChange();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PREVIEWLIST_H__35AFFA27_D2B0_11D5_A419_002078188525__INCLUDED_)
