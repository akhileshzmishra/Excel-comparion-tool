#pragma once


#include "IProjectInterface.h"
// CSideBar
#include "IDualMap.h"


typedef DualMap<int, int> ItemPositionMap;
typedef DualMapItr<int, int> ItemPositionItr;

class CSideBar : public CWnd, public XLCtrlObserver
{
	int mH;
	int mW;
	ItemPositionMap   mPosMap[2];
	DECLARE_DYNAMIC(CSideBar)

public:
	CSideBar();
	virtual ~CSideBar();

	void Create(CWnd* pParentWnd);

	void Notify(XLObservedData* data, XLEventType* type);
protected:
	DECLARE_MESSAGE_MAP()

	BOOL RegisterWindowClass();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();

	void OnDraw(CDC* pDC);


private:
	void DrawStrip(CDC* pDC, int top, int portion);
};


