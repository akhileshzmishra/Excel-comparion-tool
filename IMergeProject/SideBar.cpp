// SideBar.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "SideBar.h"
#include "CommonHeader.h"
#include "GridTableCompareView.h"
#define SIDEBAR_CLASS_NAME L"XLSIDE_BAR"

// CSideBar
static const XLEventType RegisterConditions[] = {GRID_ON_COMPARE_DONE };


IMPLEMENT_DYNAMIC(CSideBar, CWnd)

CSideBar::CSideBar():
XLCtrlObserver(GridTableCompareView::GetInstance(), 
			   (XLEventType*)RegisterConditions, 
			   (sizeof(RegisterConditions)/sizeof(XLEventType))),
mH(TABLE_HEIGHT),
mW(DIFF_POINTER_BUTTON_X)
{
	RegisterWindowClass();
}

CSideBar::~CSideBar()
{
}


BEGIN_MESSAGE_MAP(CSideBar, CWnd)
	ON_WM_PAINT()
    ON_WM_LBUTTONUP()
    ON_WM_LBUTTONDOWN()
    ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()




// CSideBar message handlers

 // Register the window class if it has not already been registered.
BOOL CSideBar::RegisterWindowClass()
{
    WNDCLASS wndcls;
    //HINSTANCE hInst = AfxGetInstanceHandle();
    HINSTANCE hInst = AfxGetResourceHandle();

    if (!(::GetClassInfo(hInst, SIDEBAR_CLASS_NAME, &wndcls)))
    {
        // otherwise we need to register a new class
        wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
        wndcls.lpfnWndProc      = ::DefWindowProc;
        wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
        wndcls.hInstance        = hInst;
        wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
        wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
        wndcls.hCursor          = 0;
#endif
        wndcls.hbrBackground    = (HBRUSH) (COLOR_BTNHILIGHT + 1);
        wndcls.lpszMenuName     = NULL;
        wndcls.lpszClassName    = SIDEBAR_CLASS_NAME;

        if (!AfxRegisterClass(&wndcls))
        {
            AfxThrowResourceException();
            return FALSE;
        }
    }

    return TRUE;
}

void CSideBar::Create(CWnd* pParentWnd)
{
	RECT rect;
	rect.top = 0;//XLUtility::GetInstance()->GetTableY();
	rect.bottom = XLUtility::GetInstance()->GetWindowHeight() - 200;
	mH = rect.bottom - rect.top;
	rect.left = 0;
	rect.right = DIFF_POINTER_BUTTON_X;
	mW = rect.right - rect.left;
	CWnd::Create(SIDEBAR_CLASS_NAME, NULL, WS_CHILD | WS_BORDER | WS_VISIBLE, rect, pParentWnd,24234232);
}


void CSideBar::OnLButtonDblClk(UINT nFlags, CPoint point)
{
}

void CSideBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	int row;
	int X = point.y;
	int X_dash = X + 1;
	GridTableCompareView* gView = GridTableCompareView::GetInstance();
	if(!gView)
	{
		return;
	}
	bool Went = false;
	for(int i = 0; i < 2; i++)
	{
		if(mPosMap[i].FindFirst(X, row))
		{
			gView->GoToRow(row, i);
			Went = true;
			break;
		}
		else if(mPosMap[i].FindFirst(X_dash, row))
		{
			gView->GoToRow(row, i);
			Went = true;
			break;
		}
	}
	if(!Went)
	{
		int rowk = gView->GetMaxRow() + 1; 
		double multiplier = (double)rowk/(double)mH;
		row = multiplier*X;
		gView->GoToRow(row, 0);
	}
}

void CSideBar::OnLButtonUp(UINT nFlags, CPoint point)
{
}
void CSideBar::OnDraw(CDC* pDC)
{
	ItemPositionItr itrs1(&mPosMap[0], true);
	while(!itrs1.IsEnd())
	{
		DrawStrip(pDC, itrs1.GetFValue(), 0);
		itrs1++;
	}
	ItemPositionItr itrs2(&mPosMap[1], true);
	while(!itrs2.IsEnd())
	{
		DrawStrip(pDC, itrs2.GetFValue(), 1);
		itrs2++;
	}
}
void CSideBar::OnPaint()
{
	CPaintDC dc(this);
	OnDraw(&dc);
}


void CSideBar::Notify(XLObservedData* data, XLEventType* type)
{
	GridTableCompareView* gView = GridTableCompareView::GetInstance();
	if(gView)
	{
		mPosMap[0].Clear();
		mPosMap[1].Clear();
		RETURNTYPE rt = gView->ChangedRowNumber();

		int row = gView->GetMaxRow() + 1;

		double multiplier = (double)mH/(double)row;

		for(int i = 0; i < rt.size(); i++)
		{
			int Y = (int)(multiplier*rt[i].first);
			mPosMap[0].Insert(rt[i].first, Y);

			Y = (int)(multiplier*rt[i].second);
			mPosMap[1].Insert(rt[i].second, Y);
		}
		Invalidate();

	}
}


void CSideBar::DrawStrip(CDC* pDC, int top, int portion)
{
	RECT rect;
	rect.top = top;
    rect.bottom = top + 1;
	rect.left = 0;
	rect.right = mW;
	COLORREF ref;
	if(portion == 0)
	{
		rect.left = 0;
		rect.right = mW/2;
		ref = DARKREDCOL;
	}
	else
	{
		rect.left = mW/2;
		rect.right = mW;
		ref = BLUECOL;
	}

	CBrush brush(ref);

	pDC->FillRect(&rect, &brush);

   // TODO: add draw code for native data here	  
	
}

