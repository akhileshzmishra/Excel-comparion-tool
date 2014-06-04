#include "StdAfx.h"
#include "XLCellDataContainer.h"

XLCellDataContainer::XLCellDataContainer(int r, int c):
m_Table(r, XLCDRow(c)),
m_iRow(r),
m_iCol(c)
{
}

XLCellDataContainer::~XLCellDataContainer(void)
{
}

XLCellData* XLCellDataContainer::CellData(int r, int c)
{
	if(r < 0 || r >= m_iRow)
		return 0;
	if(c < 0 || c >= m_iCol)
		return 0;

	return &m_Table[r][c];
}


XLCellData* XLCellDataContainer::CellData(CellIdentifier cI)
{
	int r = cI.X();
	int c = cI.Y();
	return CellData(r, c);
}

XLCDRow* XLCellDataContainer::CellRow(int r)
{
	if(r < 0 || r >= m_iRow)
		return 0;

	return &m_Table[r];
}


void XLCellDataContainer::AddRows(int rows) // Add AT last
{
	if(rows <= 0)
	{
		return;
	}
	m_iRow += rows;
	m_Table.resize(m_iRow, XLCDRow(m_iCol));
}
void XLCellDataContainer::AddCols(int cols) // Add AT last
{
	if(cols <= 0)
	{
		return;
	}
	m_iCol += cols;
	for(int i = 0; i < m_iRow; i++)
	{
		m_Table[i].resize(m_iCol);
	}
}

void XLCellDataContainer::ResizeToNew(int rows, int cols)
{
	if((m_iRow >= rows))
	{
	}
	else
	{
		AddRows(rows - m_iRow);
	}
	if(m_iCol >= cols)
	{
	}
	else
	{
		AddCols(cols - m_iCol);
	}
}