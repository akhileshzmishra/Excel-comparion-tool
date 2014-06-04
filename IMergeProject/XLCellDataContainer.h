#ifndef _XLCELLDATACONTAINER_H_
#define _XLCELLDATACONTAINER_H_
#include "XLCommonHeader.h"
#include "XLCellData.h"
#include "PositionManager.h"

#pragma once
typedef std::vector<XLCellData> XLCDRow;

class XLCellDataContainer
{	
	typedef std::vector<XLCDRow>    XLTable;
	XLTable                         m_Table;
	int                             m_iRow;
	int                             m_iCol;
public:
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

	void ResizeToNew(int rows, int cols);
	XLCellData* CellData(int r, int c);
	XLCellData* CellData(CellIdentifier cI);
	XLCDRow* CellRow(int r);
public:
	~XLCellDataContainer();
};



#endif