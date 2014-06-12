// ToolTipRessourceButton.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipRessourceButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolTipRessourceButton

CToolTipRessourceButton::CToolTipRessourceButton()
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
}

CToolTipRessourceButton::~CToolTipRessourceButton()
{
}


BEGIN_MESSAGE_MAP(CToolTipRessourceButton, baseCToolTipRessourceButton)
	//{{AFX_MSG_MAP(CToolTipRessourceButton)
		ON_WM_MOUSEMOVE()
		ON_MESSAGE(WM_MOUSELEAVE, &CToolTipRessourceButton::OnMouseLeave)
		ON_MESSAGE(WM_MOUSEHOVER, &CToolTipRessourceButton::OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolTipRessourceButton message handlers

BOOL CToolTipRessourceButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return baseCToolTipRessourceButton::PreTranslateMessage(pMsg);
}

void CToolTipRessourceButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item

	CDC * pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC * pMemDC = new CDC;
	pMemDC->CreateCompatibleDC(pDC);



	CBitmap * pOldBitmap;
	pOldBitmap = pMemDC->SelectObject(&m_Bitmap);
	
	CPoint point(0,0);
	
	if (IsWindowEnabled()) {	
		if (lpDrawItemStruct->itemState & ODS_SELECTED)
		{
			pDC->BitBlt(0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx, 0, SRCCOPY);
		} else {
			if (m_bHover) // focused ?
			{
				pDC->BitBlt(0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx*2, 0, SRCCOPY);
			} else {
				pDC->BitBlt(0,0,m_ButtonSize.cx,m_ButtonSize.cy,pMemDC,0,0,SRCCOPY);
			}
		}
	} else {
		pDC->BitBlt(0, 0, m_ButtonSize.cx, m_ButtonSize.cy, pMemDC, m_ButtonSize.cx*3, 0, SRCCOPY);
	}
	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;
}


// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/Hover
BOOL CToolTipRessourceButton::LoadBitmap(UINT bitmapid)
{
	m_Bitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP	bitmapbits;
	m_Bitmap.GetBitmap(&bitmapbits);
	m_ButtonSize.cy=bitmapbits.bmHeight;
	m_ButtonSize.cx=bitmapbits.bmWidth/4; // up, down, focused, disabled
	SetWindowPos( NULL, 0,0, m_ButtonSize.cx,m_ButtonSize.cy,SWP_NOMOVE   |SWP_NOOWNERZORDER   );
	return TRUE;
}


void CToolTipRessourceButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (m_bTracking == FALSE)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	baseCToolTipRessourceButton::OnMouseMove(nFlags, point);
}

LRESULT CToolTipRessourceButton::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHover = TRUE;
	Invalidate();
	return 0;
}


LRESULT CToolTipRessourceButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

