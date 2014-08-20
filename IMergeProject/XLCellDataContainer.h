#ifndef _XLCELLDATACONTAINER_H_
#define _XLCELLDATACONTAINER_H_
#include "XLCommonHeader.h"
#include "XLCellData.h"
#include "PositionManager.h"
#include "IndexTree.h"

#pragma once


class XLCDRow: public std::vector<XLCellData>
{
	bool        m_bDummy;
public:
	XLCDRow(int size, XLCellData defVal, bool IsDummy = false):
	std::vector<XLCellData>(size, defVal),
	m_bDummy(IsDummy)
	{
	}

	XLCDRow(int size, bool IsDummy = false):
	std::vector<XLCellData>(size),
	m_bDummy(IsDummy)
	{
	}

	XLCDRow(bool IsDummy = false):
	std::vector<XLCellData>(),
	m_bDummy(IsDummy)
	{
	}

	bool IsDummy()                      {return m_bDummy;}
	void SetDummy(bool set)             {m_bDummy = set;}
	bool operator == (XLCDRow& other);

	int GetMatchNumber(XLCDRow& other);
};

class XLCellDataContainer
{	
	typedef IndexTreeSpace::CIndexTree<XLCDRow*>    XLTable;
	XLTable                                         m_Table;
	int                                             m_iRow;
	int                                             m_iCol;
public:
	static const int   INVALID_ROW = -1;
	XLCellDataContainer(int r, int c);
	int Row()
	{
		return m_iRow;
	}
	int Col()
	{
		return m_iCol;
	}
	void AddRows(int rows); // Add AT last
	void AddCols(int cols); // Add AT last

	void InsertRowAt(int row, bool dummyRow = false);
	void InsertRowsAt(int startRow, int numRows, bool dummyRow = false);

	void ResizeToNew(int rows, int cols);
	XLCellData* CellData(int r, int c);
	XLCellData* CellData(CellIdentifier cI);
	XLCDRow* CellRow(int r);
	XLCDRow* FindInRow(int stRow, int endRow, int* colIdx, int numCols, WStrType& str);
	int GetBestMatch(int r1, int r2, XLCDRow* row);

	bool IsDummy(int row);
	bool IsEmpty(int row);
public:
	~XLCellDataContainer();
	bool mGoodRow(int r)
	{
		return ((r >= 0) && (r < m_iRow));
	}
	bool mGoodCol(int c)
	{
		return ((c >= 0) && (c < m_iCol));
	}
};



#endif