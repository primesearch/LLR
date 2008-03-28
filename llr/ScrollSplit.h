#if !defined(AFX_SCROLLSPLIT_H__8CA00EB8_FD1B_459A_B015_F2964E9FD055__INCLUDED_)
#define AFX_SCROLLSPLIT_H__8CA00EB8_FD1B_459A_B015_F2964E9FD055__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollSplit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScrollSplit frame

class CScrollSplit : public CFrameWnd
{
	DECLARE_DYNCREATE(CScrollSplit)
protected:
	CScrollSplit();           // protected constructor used by dynamic creation

// Attributes
private:
	CSplitterWnd	m_wndSplit;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScrollSplit)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CScrollSplit();

	// Generated message map functions
	//{{AFX_MSG(CScrollSplit)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCROLLSPLIT_H__8CA00EB8_FD1B_459A_B015_F2964E9FD055__INCLUDED_)
