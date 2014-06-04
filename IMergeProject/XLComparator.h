#ifndef _XLCOMPARATOR_H_
#define _XLCOMPARATOR_H_

#pragma once
#include "XLCommonHeader.h"
#include "ExcelDataExtraction.h"
#include "XLLCS.h"
#include "CommonHeader.h"

class XLDifferenceData
{
public:
	struct Column
	{
		int mCol1;
		int mCol2;
		Column():mCol1(0), mCol2(0) {}
		Column(int c1,int c2): mCol1(c1),mCol2(c2) {}
	};
private:
	int mRow1;
	int mRow2;
	int mNext;
	int mPrevious;
	typedef std::list<Column> DifferenceList;
	typedef DifferenceList::iterator DiffListItr;
	DifferenceList  mColList;
	DiffListItr     mItr;
	bool            mIteratorStarted;
	

public:
	XLDifferenceData();
	int& Row1()             {return mRow1;}
	int& Row2()             {return mRow2;}
	int RowDiffSize()       {return (int) mColList.size();}
	void InsertDifference(int first, int second);
	XLDifferenceData::Column* GetNextDifference();
	void RemovePresentColumn();
	void SetNext(int next)
	{
		mNext = next;
	}
	void SetPrevious(int prev)
	{
		mPrevious = prev;
	}
	void Reset();
};


class XLDifferenceDataList
{
	typedef std::vector<XLDifferenceData*> DifferenceData;
	DifferenceData  mList;
	int             mItr;
	bool            mInitialized;
	int             mCapacity;
	int             mSize;
	int             mPos;
public:
	XLDifferenceDataList(int intialCap = 2);  
	int RowSize()             {return mSize;}
	XLDifferenceData*  GetNextDifference();
	void Reset();
	void Clear();
	void RemovePresentRow();
	void Insert(XLDifferenceData* data);
};


class XCellDataComparator
{
public:
	bool operator() (XLCDRow& A, XLCDRow& B)
	{
		return CompareRows(A, B);
	}
private:
	bool CompareCells(XLCellData& one, XLCellData& two);
	bool CompareRows(XLCDRow& A, XLCDRow& B);
};

class XLCellDataProvider
{
	int m_iRowBase;
	XLCellDataContainer* m_pContainer;
public:
	XLCellDataProvider(int base, XLCellDataContainer* container):
	  m_iRowBase(base),
	  m_pContainer(container)
	  {
	  }

	  void SetContainer(XLCellDataContainer* container)
	  {
		  m_pContainer = container;
	  }

	  XLCDRow& operator [](int i)
	  {
		  return *(m_pContainer->CellRow(i));
	  }
};

typedef vector<int> XLCompareColumnType;
typedef XLLCS<XCellDataComparator, XLCellDataProvider>    XLDataCompareClass;

class XLComparator
{
	int         m_iBaseCellColIndex;
	RETURNTYPE  m_UnChangedRows;
	RETURNTYPE  m_ChangedRows;
	int         m_R1;
	int         m_R2;
public:
	XLComparator(int baseComparator = 0);
	~XLComparator(void);
	bool Compare(XLCellDataContainer* A, XLCellDataContainer* B);
	RETURNTYPE& UnChangedRows()   { return	m_UnChangedRows;}
	RETURNTYPE& ChangedRows()	  { return	m_ChangedRows;}
	void Reset();
private:
	void m_FindChangedRows();
};



class XLRowElementDataProvider
{
	XLCDRow* m_pContainer;
public:
	  XLRowElementDataProvider(XLCDRow* container):
	  m_pContainer(container)
	  {
	  }
	  void SetContainer(XLCDRow* container)
	  {
		  m_pContainer = container;
	  }
	  XLCellData& operator [](int i)
	  {
		  return ((*m_pContainer)[i]);
	  }
};


class XLRowDataComparator
{
public:
	bool operator() (XLCellData& A, XLCellData& B)
	{
		return CompareCells(A, B);
	}
private:
	bool CompareCells(XLCellData& one, XLCellData& two);
};

class XCellExhaustiveDataComparator
{
public:
	bool operator() (XLCDRow& A, XLCDRow& B)
	{
		return CompareRows(A, B);
	}
private:
	bool CompareRows(XLCDRow& A, XLCDRow& B);
};


typedef XLLCS<XLRowDataComparator, XLRowElementDataProvider>           XLRowCompareClass;
typedef XLLCS<XCellExhaustiveDataComparator, XLCellDataProvider>       XLExhaustiveCompareClass;





inline bool XCellDataComparator::CompareCells(XLCellData& one, XLCellData& two)
{
	return (one == two);
}


inline bool XLRowDataComparator::CompareCells(XLCellData& one, XLCellData& two)
{
	return (one == two);
}

#endif