//
// $Id: tooltipwnd.h,v 1.5 1999/04/12 20:18:16 Guy_Brousseau Development gbrousse $
//

#if !defined(AFX_TOOLTIPWND_H__2C52D3E4_2F5B_11D2_8FC9_000000000000__INCLUDED_)
#define AFX_TOOLTIPWND_H__2C52D3E4_2F5B_11D2_8FC9_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ToolTipWnd.h : header file
//
// Written by Shankar (sshank@mailcity.com)
// Last Revised : August 29 1998.
// Copyright (c) 1998.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage whatsoever.
// Distribute freely.
// Send bug reports, bug fixes, enhancements. You can reach 
// me at sshank@mailcity.com

// Acknowledgements:
// Thanks to Venkatesh who helped me in calculating the intersecting 
// point in the ellipse.

// Modified by Ludo (gbrousse@aveltech.ca)
// 1999-04-12
// * Corrected a bug: when m_stuck is not set and mouse moves over the tooltip, it moved
// * Corrected a bug: when you jumped quickly into a control and land onto a tooltip, the mouse moves and click were not forwarded
// * Add timer support (0 if no timer wanted)
// * Don't get clipped around screen corners (although incomplete).
// * Trap Non-client messages for mouse move and button click (cf. scrollbars in edit controls)
// * Replace tool in lookup table if inserted more than once
// * Explicitly display the tooltip by programmation request (rather than mouse move)
// * All revamped (simplified and commented) to be better understandable (sorry, but I must stick to local programming style...)
// ...enjoy!
// Still I introduced a kludge... (cf: m_bSkipNextMove)
//
// Thanks to my boss, Jean Côté, who joyfully granted me the 2 days necessary for this mission.
//

// Modified by Greg (greg_gagnaux@hotmail.com)
// 1999-04-20
// * Thanks to Shankar and Ludo!
// * Added a second timer to prevent the tooltip from appearing immediately. (Defaults to 1 second)
// * Fixed array overrun - CPoint ptTri[4];
//

// Modified by Greg (greg_gagnaux@hotmail.com)
// 1999-06-23
// * Removed global function reference
//

// --------------------------------------------------------------------------
const int ID_TIMER_TOOLTIP_HIDE = 1;
const int ID_TIMER_TOOLTIP_SHOW = 2;	


// --------------------------------------------------------------------------
struct BTOOLINFO
{
	CString  strToolText;
	COLORREF clrToolTextClr;
	UINT     iTimerDelay;		// in seconds
	UINT     iTimerDelayShow;	// in ms
};


//
// This struct is used by the EnumChildWindows function in the OnTimer Handler
//
typedef struct tagTTWndStruct 
{
	HWND	hWndToolTip;
	HWND	hWndToolTipParent;
} TTWNDSTRUCT;



// --------------------------------------------------------------------------
class CToolTipWnd : public CWnd
{
public:
	CToolTipWnd();
	virtual ~CToolTipWnd();

	bool m_bStuck; // stuck to mouse pointer when moving inside a control

	void RelayEvent( LPMSG );
	BOOL Create( CWnd *parent );

	void AddTool(CWnd *pWnd, CString strText, COLORREF clrTextColor = 0, int timerDelay = -1, int timerDelayShow = -1);

	void SetWidth( int iWidth );
	void SetHeight( int iHeight );
	void SetBkColor( COLORREF clrRef );
	void SetFrameColor( COLORREF clrRef );
	void SetDefTextColor( COLORREF clrRef );
	void SetFontHeight( int iHeight );
	void SetFontName( CString strFontName );
	void SetDelay( int iDelay );

	void Show( HWND hWnd, const char *overrideText = NULL, int timerDelay = -1, int timerDelayShow=-1);
	void Hide();

protected:
	//{{AFX_MSG(CToolTipWnd)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL CToolTipWnd::PreTranslateMessage( MSG *pMsg );
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	LPCTSTR m_lpWndCls;
	HWND    m_hParentWnd;

	CFont   m_strTextFont;
	CRgn    m_rgn;
	CRgn    m_rgnTri;
	CRgn    m_rgnComb;	
	CRect   m_rectText;

	CMapPtrToPtr m_toolPtr;		// all tooltips specifications
	HWND         m_pCurrwnd;	// control for which a tooltip is currently displayed

	// Tooltip default parameters
	int      m_iWidth;
	int      m_iHeight;
	COLORREF m_clrBkColor;
	COLORREF m_clrFrameColor;
	COLORREF m_clrTextColor;
	int      m_iFontHeight;
	CString  m_strFontName;
	CString  m_strText;
	int      m_iDelay;
	int      m_iDelayShow;

	bool m_bSkipNextMove; // to skip the next WM_MOUSEMOVE message
	UINT m_iTimer;
	UINT m_iTimer2;

	void KillTimer();
	void SetTimer( int iDelay );
	void Show( int x, int y, int timerDelayHide, int timerDelayShow );


	void SetShowTimer(int iTimeTillShow);
	void KillShowTimer(void);
};


// --------------------------------------------------------------------------
// Inline Method:

inline void CToolTipWnd::SetWidth(int iWidth)				{m_iWidth = iWidth;}
inline void CToolTipWnd::SetHeight(int iHeight)				{m_iHeight = iHeight;}
inline void CToolTipWnd::SetBkColor(COLORREF clrRef)		{m_clrBkColor = clrRef;}
inline void CToolTipWnd::SetFrameColor(COLORREF clrRef)		{m_clrFrameColor = clrRef;}
inline void CToolTipWnd::SetDefTextColor( COLORREF clrRef)	{m_clrTextColor = clrRef;}
inline void CToolTipWnd::SetFontHeight(int iHeight)			{m_iFontHeight = iHeight;}
inline void CToolTipWnd::SetFontName(CString strFontName)	{m_strFontName = strFontName;}
inline void CToolTipWnd::SetDelay(int iDelay)				{m_iDelay = iDelay;}


//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPWND_H__2C52D3E4_2F5B_11D2_8FC9_000000000000__INCLUDED_)
