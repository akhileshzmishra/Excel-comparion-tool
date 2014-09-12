#ifndef _GridTableCompareView_H_
#define _GridTableCompareView_H_
#pragma once
#include "XLCommonHeader.h"
#include "Grid/GridCtrl.h"
#include "XLCellDataContainer.h"
#include "Afxwin.h"
#include "IProjectInterface.h"
#include "CommonHeader.h"
#include "DifferencePointer.h"
#include "XLSimpleThreads.h"
#include "PositionManager.h"
#include "XLComparator.h"


enum CompareResult
{
	CompareResult_Unsuccessful,
	CompareResult_Success_NoChange,
	CompareResult_Success_Changes,

	CompareResult_Invalid
};



class GridTableCompareView	 : public XLCtrlSubject	
{
	XLComparator                          m_Comparator;
	static GridTableCompareView*          m_Instance;
	std::string                           m_pathOne;
	std::string                           m_pathTwo;
	std::vector<XLCellDataContainer*>     m_ContainerList;
	int                                   m_iX;
	int                                   m_iY;
	int                                   m_iTableWidth;
	int                                   m_iTableHeight;
	int                                   m_iRowHeight;
	std::vector<CGridCtrl* >              m_GridList;
	bool                                  m_bReady;
	int                                   m_iMaxRow;
	int                                   m_iMaxCol;
	CWnd*                                 m_pParentWnd;
	int                                   m_Partitions[3];
	RowComparisonList                     m_ChangedRowList;
	RowComparisonList                     m_UnchangedRowList;
	XLTableParam                          m_CurrentState;
	PositionMap                           m_GridToContainerMap[2];
	RowComparisonIterator                 m_CompItr;
	CellIdentifier                        m_NextFind;

	friend class GridViewHelper;
	
private:
	GridTableCompareView();
	GridTableCompareView(const	GridTableCompareView& other);
public:
	static GridTableCompareView* GetInstance();

	void SetParent(CWnd* parent)
	{
		m_pParentWnd = parent;
	}
	virtual ~GridTableCompareView(void);

	bool LoadTableA(std::string path);
	bool LoadTableB(std::string path);

	virtual bool Load();
	virtual void CreateTable();

	bool IsReady()
	{
		return (m_bReady && (__TotalDrawableTables() >= 2));
	}

	int GetMaxRow()
	{
		return m_iMaxRow;
	}

	int GetMaxCol()
	{
		return m_iMaxCol;
	}

	CompareResult Compare(XLTableParam param = XLTableParamReloadTable);

	void ResetTables();
	void ResetContainers();
	void ReCreateGridList();
	void NextDifference();
	void PrevDifference();
	CompareResult CreateDiffTable();
	CompareResult CreateSameTable();
	CompareResult CreateAllTable();
	void CreateEmptyGrid();

	//Function overloaded from observer
	void Notify(XLObservedData* data, XLEventType condition);
	void SaveToDatabase(int r, int c, CString& value);
#if 0
	void SaveToTable(int r, int c, CString& value, int table);
#endif
	void OnCopyFromRightToLeft(int r1, int r2, int c1, int c2);
	void OnCopyFromLeftToRight(int r1, int r2, int c1, int c2);
	void OnMoveFromRightToLeft(int r1, int r2, int c1, int c2);
	void OnMoveFromLeftToRight(int r1, int r2, int c1, int c2);




	//Functions  called by view. (includes button on taskbar)
	void Clear();   
	void ICopyRightToLeft();
	void ICopyLeftToRight();
	void LineCopyLeftToRight();
	void LineCopyRightToLeft();
	void SelectionCopyLeftToRight(int r1, int r2, int c1, int c2);
	void SelectionCopyRightToLeft(int r1, int r2, int c1, int c2);

	void Find(std::wstring& wstr);
	void FindBackward(std::wstring& wstr);
	void FindForward(std::wstring& wstr);

	bool SaveInFiles();
	bool SaveFileA();
	bool SaveFileB();
	void GridSameCol(int first, int r1, int col1, int second, int r2, int col2);
	void ArrowPosition(int X, int Y);
	void UndoChanges();
	void GoToRow(int row, int table);
	RowComparisonList& ChangedRowNumber()
	{
		return m_ChangedRowList;
	}




private:
	void __FillGrid(CGridCtrl* grid, XLCellDataContainer* container, int r, int c, int gr, int gc);
	void __FillGrid(CGridCtrl* grid, XLCellDataContainer* container, int r, int gr, int c);
	void __FillGrid(CGridCtrl* grid, XLCellDataContainer* container, int r, int c);


	void __CreateGridView(CGridCtrl*& grid, XLCellDataContainer* container, int position);
	int __TotalDrawableTables();

	void __SetRowColor(int table, int r, COLORREF ref);	

	void __MoveIndicators(bool ForceHide = false);

	void __InterChangeData(XLCellDataContainer* From, XLCellDataContainer* To, int r1, int c1, int r2, int c2);
	void __CreateComparisonDataStructure();
	void __InsertInUnchangedFromChanged(int row1, int row2);

	void __ShowIndicators(int Y, int TableNum);
	void __HideIndicators(int TableNum);

	void __DrawDiffTable();
	void __DrawSameTable();

	void __SaveDataFromGridToContainer(int r, int c, int table, CString& value);
	void __SaveDataFromHistory(int r, int c, int table, XLCellData* value);

	void __NotifyFileChange(int tbl);
	void __NotifyFileSaved();
	void __NotifySidebar(XLEventType eventType, int row1, int row2);
	void __SetMaxSizeForGridCreation();	

	void __RecordThisInHistory(int tableID, CellIdentifier contCell, XLCellData* data);

	void __FormatAllTables();
	void __FormatATable(int tableI);
	void __FormatARow(int gridrow, int tableI);

	
};


#endif
