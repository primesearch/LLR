// ScrollDisplay.cpp : implementation file
//

#include "stdafx.h"
#include "prime95.h"

#include "Prime95Doc.h"
#include "ScrollDisplay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScrollDisplay

IMPLEMENT_DYNCREATE(CScrollDisplay, CScrollView)

CScrollDisplay::CScrollDisplay()
{
}

CScrollDisplay::~CScrollDisplay()
{
}


BEGIN_MESSAGE_MAP(CScrollDisplay, CScrollView)
	//{{AFX_MSG_MAP(CScrollDisplay)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScrollDisplay drawing

CSize sizeTotal;

void CScrollDisplay::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CScrollDisplay::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
	if (lines[0] == NULL) {
		CPrime95Doc* pDoc = (CPrime95Doc*)GetDocument();
		pDoc->title ("IDLE");
		return;
	}

	RECT	r;
	int	ypos;
	int	i;

	pDC->SetBkMode (TRANSPARENT);
	pDC->SetTextColor (GetSysColor (COLOR_WINDOWTEXT));

	GetClientRect (&r);
	for (ypos = r.bottom, i = 1;
	     ypos > 0 && i < NumLines;
	     i++) {
		ypos -= charHeight;
		pDC->TextOut (0, ypos, lines[i], strlen (lines[i]));
	}
}

/////////////////////////////////////////////////////////////////////////////
// CScrollDisplay diagnostics

#ifdef _DEBUG
void CScrollDisplay::AssertValid() const
{
	CScrollView::AssertValid();
}

void CScrollDisplay::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CScrollDisplay message handlers

void CScrollDisplay::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if (charHeight == 0) {
		CDC *dc = GetDC ();
		CSize size;
		size = dc->GetTextExtent ("A", 1);
		charHeight = size.cy;
	}
	if (lHint == 0) ScrollWindow (0, -charHeight, NULL, NULL);
	else Invalidate ();
	UpdateWindow ();	
}
