#if !defined(AFX_SCROLLDISPLAY_H__3F7E4113_20F2_4C22_B6A4_DAB82DB76995__INCLUDED_)
#define AFX_SCROLLDISPLAY_H__3F7E4113_20F2_4C22_B6A4_DAB82DB76995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollDisplay.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollDisplay view

class CScrollDisplay : public CScrollView
{
protected:
	CScrollDisplay();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CScrollDisplay)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollDisplay)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScrollDisplay();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CScrollDisplay)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLDISPLAY_H__3F7E4113_20F2_4C22_B6A4_DAB82DB76995__INCLUDED_)
