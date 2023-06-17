// SplitFrame.cpp : implementation file
//

#include "stdafx.h"
#include "prime95.h"
#include "SplitFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame

IMPLEMENT_DYNCREATE(CSplitFrame, CFrameWnd)

CSplitFrame::CSplitFrame()
{
}

CSplitFrame::~CSplitFrame()
{
}


BEGIN_MESSAGE_MAP(CSplitFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSplitFrame)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitFrame message handlers

BOOL CSplitFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	if(m_wndSplit.CreateStatic(this, 2, 1) == FALSE) {
		TRACE0("Failed to Create Static Splitter\n");
		return FALSE;
	}
	BOOL bCreated = m_wndSplit.CreateView(0, 0,
										pContext->m_pNewViewClass,
										CSize(128, 60),
										pContext);
	if (bCreated == FALSE) {
		TRACE0("Failed to Create first pane\n");
		return FALSE;
	}
	// add the second splitter pane
	bCreated = m_wndSplit.CreateView(1, 0,
										RUNTIME_CLASS(CWinApp),
										CSize(0, 0),
										pContext);
	if (bCreated == FALSE) {
		TRACE0("Failed to Create second pane\n");
		return FALSE;
	}
	// activate the input view
	SetActiveView((CView*)m_wndSplit.GetPane(0,0));
	return TRUE;
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
