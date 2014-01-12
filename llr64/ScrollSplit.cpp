// ScrollSplit.cpp : implementation file
//

#include "stdafx.h"
#include "prime95.h"
#include "ScrollSplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollSplit

IMPLEMENT_DYNCREATE(CScrollSplit, CFrameWnd)

CScrollSplit::CScrollSplit()
{
}

CScrollSplit::~CScrollSplit()
{
}


BEGIN_MESSAGE_MAP(CScrollSplit, CFrameWnd)
	//{{AFX_MSG_MAP(CScrollSplit)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollSplit message handlers

//  extern CSize sizeTotal;

BOOL CScrollSplit::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
//	CDC *dc = GetDC ();
//	CSize size;
//	size = dc->GetTextExtent ("A", 10);
	CSize 	size(128, 60);
	return m_wndSplit.Create(this, 2, 2, size, pContext);
//	return CFrameWnd::OnCreateClient(lpcs, pContext);
}
