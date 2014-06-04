//
// ToolTipWnd.cpp : implementation file
//

#include "stdafx.h"

#include "ToolTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



// --------------------------------------------------------------------------
CToolTipWnd::CToolTipWnd()
{
	m_lpWndCls = AfxRegisterWndClass(0);
	m_bStuck = false;
	m_bSkipNextMove = false;
	m_pCurrwnd = NULL;
	m_iTimer = 0;
	m_iTimer2 = 0;
	m_iDelay = 5;				// seconds
	m_iDelayShow = 1000;		// ms

	//Defaults
	m_clrFrameColor = RGB(0, 0, 255);  //blue
	m_clrBkColor = RGB(249, 254, 188); //light yellow
	m_iWidth = 160;
	m_iHeight = 80;

	m_clrTextColor = RGB( 0, 0, 0 ); //black
	m_iFontHeight = 14;
	m_strFontName = "Arial";
	m_strText = "";
}


// --------------------------------------------------------------------------
CToolTipWnd::~CToolTipWnd()
{
	if (m_iTimer > 0)
		KillTimer();

	if (m_iTimer2 > 0)
		KillShowTimer();

	// delete all BTOOLINFO entries put in the map
	BTOOLINFO*	stToolInfo = NULL;
	HWND*		pWnd;	// dummy
	for (POSITION pos = m_toolPtr.GetStartPosition(); pos != NULL;)
	{
		m_toolPtr.GetNextAssoc(pos, (void *&)pWnd, (void *&)stToolInfo);
		delete stToolInfo;
	}
	m_toolPtr.RemoveAll();
}


// --------------------------------------------------------------------------
BEGIN_MESSAGE_MAP(CToolTipWnd, CWnd)
	//{{AFX_MSG_MAP(CToolTipWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// --------------------------------------------------------------------------
BOOL CToolTipWnd::Create( CWnd* pParentWnd ) 
{
	BOOL bRet = CWnd::CreateEx(NULL, m_lpWndCls, NULL, WS_POPUP, 0, 0, m_iWidth, m_iHeight, 
							   pParentWnd->GetSafeHwnd(), NULL, NULL );
   
	m_hParentWnd = pParentWnd->GetSafeHwnd();

	if (bRet)
		SetOwner(pParentWnd);

	return bRet;
}


// --------------------------------------------------------------------------
void CToolTipWnd::OnPaint() 
{
   CPaintDC dc( this ); // device context for painting
   
   CRect rectCl;
   GetClientRect( &rectCl );
   
   CRgn rgnComb;	
   rgnComb.CreateRectRgn( rectCl.left+10,rectCl.top,rectCl.right,rectCl.bottom );
   
   int iRetComb = rgnComb.CombineRgn( &m_rgnTri, &m_rgn, RGN_OR );
   if ( iRetComb == ERROR )
   {
      AfxMessageBox( (LPCTSTR)"ERROR in Combining Region" );
      return;
   }
   
   CBrush pBrush;
   pBrush.CreateSolidBrush( m_clrFrameColor );
   
   CBrush pBrush1;
   pBrush1.CreateSolidBrush( m_clrBkColor );
   
   dc.FillRgn( &rgnComb, &pBrush1 );
   dc.FrameRgn( &rgnComb, &pBrush, 2, 1 );
   
   dc.SetBkMode( TRANSPARENT );
   dc.SetTextColor( m_clrTextColor );
   
   CFont *pFont = dc.SelectObject( &m_strTextFont );
   
   CSize czTextWidth = dc.GetTextExtent( m_strText );
   
   if ( czTextWidth.cx < m_rectText.Width() )
      dc.DrawText( m_strText, m_rectText, DT_CENTER | DT_VCENTER | DT_SINGLELINE );
   else
      dc.DrawText( m_strText, m_rectText, DT_CENTER | DT_WORDBREAK );

   dc.SelectObject( pFont );
}


// --------------------------------------------------------------------------
int CToolTipWnd::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
   if ( CWnd::OnCreate( lpCreateStruct ) == -1 )
      return -1;
   
   CRect rectCl;
   GetClientRect( &rectCl );
   
   int x=0, y=0;
   CRect rectTemp;
   
   rectTemp = rectCl;
   rectTemp.left = rectTemp.left + 10;
   
   x = (int)( (float)( (float)rectTemp.Width() / 2.0 ) / 1.41421 );
   y = (int)( (float)( (float)rectTemp.Height() / 2.0 ) / 1.41421 );
   
   m_rectText.top = ( ( rectTemp.Height() / 2 ) - y );
   m_rectText.left = ( ( rectTemp.Width() / 2 ) - x ) + 10;
   m_rectText.right = ( ( rectTemp.Width() / 2 ) + x ) + 10;
   m_rectText.bottom = ( ( rectTemp.Height() / 2 ) + y );
   
   m_rgn.m_hObject = NULL;
   m_rgnTri.m_hObject = NULL;
   m_rgnComb.m_hObject = NULL;
   
   BOOL bRegRet = m_rgn.CreateEllipticRgn( rectCl.left+10,rectCl.top,rectCl.right,rectCl.bottom );
   
   CPoint ptTri[4];
   ptTri[0].x = rectCl.left;
   ptTri[0].y = ( rectCl.bottom / 2 ) - 10;
   
   ptTri[1].x = rectCl.left + 15;
   ptTri[1].y = ( rectCl.bottom / 2 ) - 5;
   
   ptTri[2].x = rectCl.left + 15;
   ptTri[2].y = ( rectCl.bottom / 2 ) + 5;
   
   ptTri[3].x = rectCl.left;
   ptTri[3].y = ( rectCl.bottom / 2 ) - 10;
   
   BOOL bRegTriRet = m_rgnTri.CreatePolygonRgn( ptTri, 3, ALTERNATE );
   
   m_rgnComb.CreateRectRgn( rectCl.left+10,rectCl.top,rectCl.right,rectCl.bottom );
   int iRetComb = m_rgnComb.CombineRgn( &m_rgnTri, &m_rgn, RGN_OR );
   
   if ( iRetComb == ERROR )
   {
      AfxMessageBox((LPCTSTR) "ERROR in Combining Region" );
      return -1;
   }
   
   int bRgnWnd = SetWindowRgn( m_rgnComb.operator HRGN( ), TRUE );	
   
   m_strTextFont.CreateFont( m_iFontHeight, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, m_strFontName );
   
   return 0;
}


// --------------------------------------------------------------------------
void CToolTipWnd::RelayEvent( LPMSG lpMsg )
{
   switch( lpMsg->message ) 
   {
      case WM_KEYDOWN:
         Hide();
         break;
  
      case WM_LBUTTONDOWN:
      case WM_RBUTTONDOWN:
      case WM_NCLBUTTONDOWN:
      case WM_NCRBUTTONDOWN:
         Hide();
         break;
      
      case WM_MOUSEMOVE:
      case WM_NCMOUSEMOVE:
      {
         // This is a fix to allow for messages to be made visible when not
         // using the mouse.
         if ( m_bSkipNextMove )
         { 
//            TRACE0("Move skipped\n");
            m_bSkipNextMove = false; 
            return;
         }
         else
		 {
//	         TRACE0("Move\n");
		 }

         HWND wndPt = lpMsg->hwnd;
         CPoint pt;
         pt.x = lpMsg->pt.x;
         pt.y = lpMsg->pt.y;

         // Don't show the tooltips if the application does not have the input focus
         CWnd *pFocusWnd = AfxGetApp()->m_pMainWnd->GetFocus();
         if ( pFocusWnd == NULL )
            break;

         // There are 3 possible states regarding tooltip controls:
         // a) moving outside any ctrl
         // b) going from outside a ctrl to inside
         // c) moving inside the control
         // d) going from inside a ctrl to outside

         BTOOLINFO *stToolInfo = NULL;
         BOOL found = m_toolPtr.Lookup( wndPt, (void *&)stToolInfo );

         if ( m_pCurrwnd == NULL ) // was not in a control
         {
            if ( found ) // enters a control (now in a control)
            {
//               TRACE0("OUT -> IN\n");
               m_clrTextColor = stToolInfo->clrToolTextClr;
               m_strText = stToolInfo->strToolText;
               Show( pt.x, pt.y, stToolInfo->iTimerDelay, stToolInfo->iTimerDelayShow );
               m_pCurrwnd = wndPt;
            }
            else // still not in a control
			{
//               TRACE0("OUT -> OUT\n");
			}
         }
         else // was in a control
         {
            ASSERT( m_pCurrwnd != NULL );
            CRect rect;
            ::GetWindowRect( m_pCurrwnd, &rect );
            if ( rect.PtInRect( lpMsg->pt ) ) // still in the same control
            {
//               TRACE0("IN -> IN (same)\n");
               if ( m_bStuck )
                  if ( IsWindowVisible() )
                  {
                     // may be over a tooltip, so look for previous control
                     if ( ! found )
                        found = m_toolPtr.Lookup( m_pCurrwnd, (void *&)stToolInfo );
                     ASSERT( found );
		               Show( pt.x, pt.y, stToolInfo->iTimerDelay, stToolInfo->iTimerDelayShow );
                  }
            }
            else // gone outside the control
            {
               Hide(); 
               m_pCurrwnd = NULL;

               if ( found )  // to another control
               {
//                  TRACE0("IN -> IN (other)\n");
                  m_clrTextColor = stToolInfo->clrToolTextClr;
                  m_strText = stToolInfo->strToolText;
	               Show( pt.x, pt.y, stToolInfo->iTimerDelay, stToolInfo->iTimerDelayShow );
                  m_pCurrwnd = wndPt;
               }
               else
			   {
//                  TRACE0("IN -> OUT\n");
			   }
            }
         }
      }	
      break; //WM_MOUSEMOVE
   }
}


// --------------------------------------------------------------------------
// Add or Replace tooltip if already present

void CToolTipWnd::AddTool(CWnd *pWnd, CString strText, COLORREF clrTextColor/*=NULL*/, int timerDelay /*= -1*/, int timerDelayShow /*=-1*/)
{
   ASSERT( pWnd->GetSafeHwnd() != NULL );

   BTOOLINFO *stToolInfo = NULL;
   if ( m_toolPtr.Lookup( pWnd->GetSafeHwnd(), (void *&)stToolInfo ) )
   {
      m_toolPtr.RemoveKey( pWnd );
      delete stToolInfo;
   }

   stToolInfo = new BTOOLINFO;
   stToolInfo->strToolText = strText;
	stToolInfo->clrToolTextClr = clrTextColor;
   if ( timerDelay < 0 )
      stToolInfo->iTimerDelay = m_iDelay; // default value
   else
      stToolInfo->iTimerDelay = timerDelay;

   if (timerDelayShow < 0)
      stToolInfo->iTimerDelayShow = m_iDelayShow;
   else
      stToolInfo->iTimerDelayShow = timerDelayShow;


   m_toolPtr.SetAt( pWnd->GetSafeHwnd(), stToolInfo );
}





//
// This callback is used by the EnumChildWindows function in the OnTimer Handler
//
BOOL CALLBACK EnumChildProc(HWND hwnd, LPARAM lParam)
{
	BOOL bStatus = TRUE;
	TTWNDSTRUCT* pws = (TTWNDSTRUCT*)lParam;
	if (pws)
	{
		HWND hWndToolTip = pws->hWndToolTip;
		HWND hWndToolTipParent = pws->hWndToolTipParent;

		// Are we over the same window as when the timer message was fired?
		if (hwnd == hWndToolTipParent)
		{
			// Display the tooltip
			::ShowWindow(hWndToolTip, SW_SHOWNOACTIVATE);

			// Stop enumerating
			bStatus = FALSE;
		}
	}

	return bStatus;
}
 
  


// --------------------------------------------------------------------------
void CToolTipWnd::OnTimer( UINT nIDEvent ) 
{
	if (nIDEvent == ID_TIMER_TOOLTIP_HIDE)
	{
		KillTimer();
		KillShowTimer();
		ShowWindow(SW_HIDE);

	}
	else if (nIDEvent == ID_TIMER_TOOLTIP_SHOW)
	{
		POINT pt;

		// Where is the mouse right now?
		if (GetCursorPos(&pt))
		{
			// Over which window?
			CWnd* pWnd = WindowFromPoint(pt);

			// Save the relevant window handles
			TTWNDSTRUCT ws;
			ZeroMemory(&ws, sizeof(ws));
			ws.hWndToolTip = GetSafeHwnd();
			ws.hWndToolTipParent = pWnd->GetSafeHwnd();

			// Now do the window handle comparisons
			EnumChildWindows(m_hParentWnd, EnumChildProc, (LPARAM)&ws); 
		}
 
	}


	CWnd::OnTimer(nIDEvent);
}


// --------------------------------------------------------------------------
void CToolTipWnd::SetTimer( int iDelay )
{
   KillTimer(); 
   if ( iDelay > 0 ) // no timer if <= 0
      m_iTimer = CWnd::SetTimer( ID_TIMER_TOOLTIP_HIDE, iDelay*1000, NULL );
}


void CToolTipWnd::SetShowTimer(int iTimeTillShow)
{
   KillShowTimer(); 
   if ( iTimeTillShow > 0 ) // no timer if <= 0
      m_iTimer2 = CWnd::SetTimer(ID_TIMER_TOOLTIP_SHOW, iTimeTillShow, NULL );
}

// --------------------------------------------------------------------------
void CToolTipWnd::KillTimer()
{
   if ( m_iTimer > 0 && m_hWnd != NULL ) 
      CWnd::KillTimer( m_iTimer ); 
   m_iTimer = 0;
}

void CToolTipWnd::KillShowTimer()
{
   if ( m_iTimer2 > 0 && m_hWnd != NULL ) 
      CWnd::KillTimer( m_iTimer2 ); 
   m_iTimer2 = 0;
}


// --------------------------------------------------------------------------
// Make the tooltip appear just as if the mouse just entered 'hWnd'
// 'hWnd' must be the active window.

void CToolTipWnd::Show( HWND hWnd, const char *overrideText, int timerDelayHide, int timerDelayShow )
{
   // make sure the window is active
   CWnd *pFocusWnd = AfxGetApp()->m_pMainWnd->GetFocus();
   if ( pFocusWnd == NULL )
      return;

   // If another one was present, hide it.
   Hide();

   // take the 'hWnd' coordinates and compute where the tooltip should appear
   RECT rect;
   ::GetWindowRect( hWnd, &rect );
   POINT pt;
   pt.x = rect.left + ( ( rect.right - rect.left ) / 2 );
   pt.y = rect.bottom;

   // Determine the text to dislay: 'overrideText' or original tooltip text
   if ( overrideText != NULL )
      m_strText = overrideText;
   else
   {
      // Reset 'm_strText' to original from tooltip
      BTOOLINFO *stToolInfo;
      if ( m_toolPtr.Lookup( hWnd, (void *&)stToolInfo ) )
         m_strText = stToolInfo->strToolText;
   }

   // Make it appear at 'pt'
   if ( timerDelayHide <= 0 ) // don't permit a tooltip to stay forever
      timerDelayHide = m_iDelay;
   Show( pt.x, pt.y, timerDelayHide, timerDelayShow);

   m_bSkipNextMove = true; // it seems we receive a WM_MOUSEMOVE whenever a window is displayed...
   m_pCurrwnd = hWnd;
}


// --------------------------------------------------------------------------
void CToolTipWnd::Show( int x, int y, int timerDelayToHide, int timerDelayToShow ) // force apparition of the tooltip
{
   // Check to see if the tooltip has a portion outside the
   // screen, if so, adjust.
   if (x < 0)
      x = 0;

   if (y < 0)
      y = 0;

   RECT r;
   r.left = x;
   r.right = r.left + m_iWidth;
   r.top = y;
   r.bottom = r.top + m_iHeight;

   // Compare to screen coordinates (don't take in account the desktop toolbar)
   int screenHeight = GetSystemMetrics( SM_CYSCREEN );
   if ( r.bottom > screenHeight )
      r.top = screenHeight - ( m_iHeight );

   int screenWidth = GetSystemMetrics( SM_CXSCREEN );
   if ( r.right > screenWidth )
      r.left = screenWidth - m_iWidth;

   // Move the window
   SetWindowPos( &wndTop, r.left, r.top, m_iWidth, m_iHeight, SWP_NOACTIVATE );

   // Only show after show delay has passed
   // ShowWindow( SW_SHOWNOACTIVATE );

    // Start the hide delay timer
	SetTimer( timerDelayToHide );

	// Start the show delay timer
	SetShowTimer(timerDelayToShow);
}


// --------------------------------------------------------------------------
void CToolTipWnd::Hide()
{
   if ( IsWindowVisible() )
   {
      ShowWindow( SW_HIDE );
      KillTimer();
	  KillShowTimer();
   }
}


// --------------------------------------------------------------------------
// Relay mouse messages occuring when and we move over a tooltip.
// Try doing a fast jump with the mouse into a control where the tooltip is first drawn...

BOOL CToolTipWnd::PreTranslateMessage( MSG *pMsg )
{
   RelayEvent( pMsg );

	return CWnd::PreTranslateMessage( pMsg );
}
