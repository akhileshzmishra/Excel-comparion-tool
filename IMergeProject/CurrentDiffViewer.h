#pragma once

#include "Grid/GridCtrl.h"
#include "XLCellDataContainer.h"
// CCurrentDiffViewer dialog

class CCurrentDiffViewer : public CDialog
{
	CWnd*            m_pParent;
	CGridCtrl*       m_pGrid;
	XLCDRow          m_RowA;
	XLCDRow          m_RowB;

	DECLARE_DYNAMIC(CCurrentDiffViewer)

public:
	CCurrentDiffViewer(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCurrentDiffViewer();
	virtual BOOL Create(UINT nIDTemplate, CWnd* pParentWnd = NULL);
// Dialog Data
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
};
