#if !defined(AFX_SPLITFRAME_H__3C3403E0_FE65_4596_B054_3603BEFD8F02__INCLUDED_)
#define AFX_SPLITFRAME_H__3C3403E0_FE65_4596_B054_3603BEFD8F02__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SplitFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame frame

class CSplitFrame : public CFrameWnd
{
	DECLARE_DYNCREATE(CSplitFrame)
protected:
	CSplitFrame();           // protected constructor used by dynamic creation

// Attributes
private:
	CSplitterWnd	m_wndSplit;
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitFrame)
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSplitFrame();

	// Generated message map functions
	//{{AFX_MSG(CSplitFrame)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITFRAME_H__3C3403E0_FE65_4596_B054_3603BEFD8F02__INCLUDED_)
