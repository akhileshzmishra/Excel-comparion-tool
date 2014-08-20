// IMergeProjectView.h : interface of the IMergeProjectView class
//


#pragma once
#include "Grid/GridCtrl.h"
#include "GridTableCompareView.h"
#include "DifferencePointer.h"
#include "DPTextBox.h"
#include "Settings.h"
#include "OnCompDialogBox.h"
#include "SaveDialog.h"
#include "AskBeforeExit.h"
#include "FindItemDlg.h"
#include "SideBar.h"
#include "ToolTipRessourceButton.h"
#include "OptionSettings.h"


class IMergeProjectView : public CView
{
protected: // create from serialization only
	GridTableCompareView*            m_pCreator;
	std::string                      m_pathOne;
	std::string                      m_pathTwo;
	bool                             m_bLoadFileA;
	bool                             m_bLoadFileB;
	int                              m_iMode;  
	DPTextBox                        m_PathTxtBox[2];
	class  XLUtility*                m_Utility; 
	CToolTipRessourceButton          m_PathBtns[2];	 //IDB_BITMAP_FILELOAD
	CDifferencePointer*              m_DiffIndicator[2];
	OnCompDialogBox*                 m_CompareDlg;
	CSaveDialog*                     m_SaveDialog;
	CAskBeforeExit*                  m_AskBeforeExitDlg;
	CFindItemDlg*                    m_FindDlg;
	COptionSettings*                 m_OptionsDlg;
	CSideBar                         m_SideBar;

protected:
	IMergeProjectView();
	DECLARE_DYNCREATE(IMergeProjectView)
	enum
	{
		FirstModeStartView,
		FileLoadAModeStartView,
		FileLoadBModeStartView,
		CompareModeStartView,
		CompareModeView,
		MergeModeStartView,
		MergeModeView,
		InvalidModeView,
	};
// Attributes
public:
	IMergeProjectDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void OnDraw1(CDC* pDC, int top, int bottom);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void OnActivateView(BOOL bActivate, CView* pActivateView,
					CView* pDeactiveView);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnSize(UINT nType, int cx, int cy);

// Implementation
public:
	virtual ~IMergeProjectView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void CreateGrid();
	void DestroyGrid();
	void CreateSideBar();
	void ChangeMode(int past);

	void CreatePathBox();

	void FileLoaded(int fileNum);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLoadFileOne();
public:
	afx_msg void OnLoadFileTwo();
public:
	afx_msg void OnFileCompare();
public:
	afx_msg void OnSettingSetbaserow();
public:
	afx_msg void OnSettingMerge();
public:
	afx_msg void OnPaint();
	afx_msg void OnFileSave();
	afx_msg void OnNextDifference();
	afx_msg void OnPreviousDifference();
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnShowDifferenceOnly();
	afx_msg void OnShowSameOnly();
	afx_msg void OnShowAll();
	afx_msg void OnMoveRight();
	afx_msg void OnMoveLeft();
	afx_msg void OnLeftArrow();
	afx_msg void OnRightArrow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnFindUserString();
	afx_msg void OnFindNextUserString();
	afx_msg void OnFindPrevUserString();
	afx_msg void OnFindClosed();
	afx_msg void OnEditUndo();
	afx_msg void OnSettingOptions();
};

#ifndef _DEBUG  // debug version in DeepikaProjectView.cpp
inline IMergeProjectDoc* IMergeProjectView::GetDocument() const
   { return reinterpret_cast<IMergeProjectDoc*>(m_pDocument); }
#endif

