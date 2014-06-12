#if !defined(AFX_TOOLTIPRESSOURCEBUTTON_H__663049D7_9D25_11D5_8F75_0048546F01E7__INCLUDED_)
#define AFX_TOOLTIPRESSOURCEBUTTON_H__663049D7_9D25_11D5_8F75_0048546F01E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipRessourceButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolTipRessourceButton window

#include "ToolTipButton.h"

#define baseCToolTipRessourceButton CToolTipButton
// if you don't want tooltip, you can put
// #define baseCToolTipRessourceButton CBitmapButton
// or
// #define baseCToolTipRessourceButton CButton


// CToolTipRessourceButton by Jean-Louis GUENEGO
// Thanks to Niek Albers.
// A cool CBitmapButton derived class with 4 states.
class CToolTipRessourceButton : public baseCToolTipRessourceButton
{
// Construction
public:
	CToolTipRessourceButton();

// Attributes
public:
	CBitmap m_Bitmap; // bitmap containing the 4 images (up, down, focused, disabled)
	CSize m_ButtonSize; // width and height of the button
	BOOL m_bHover; // indicates if mouse is over the button (it is like focused on a CBitmapButton)
	BOOL m_bTracking; // true when the mouse just arrives on the button, or leaves the button, false other

// Operations
public:
	BOOL LoadBitmap(UINT bitmapid);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipRessourceButton)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTipRessourceButton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolTipRessourceButton)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPRESSOURCEBUTTON_H__663049D7_9D25_11D5_8F75_0048546F01E7__INCLUDED_)
