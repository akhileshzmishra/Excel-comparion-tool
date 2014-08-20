#include "StdAfx.h"
#include "XLCellDataContainer.h"
#include "Settings.h"


bool XLCDRow::operator == (XLCDRow& other)
{
	if(size() != other.size())
	{
		return false;
	}

	for(int i = 0; i < size(); i++)
	{
		if((*this)[i] == other[i])
		{
			continue;
		}
		return false;
	}
	return true;
}

int XLCDRow::GetMatchNumber(XLCDRow& other)
{
	int r1 = size();
	int r2 = other.size();

	int minElem = min(r1, r2);

	int match = 0;
	for(int i = 0; i < minElem; i++)
	{
		if((*this)[i] == other[i])
		{
			match++;
		}
	}
	return match;
}


XLCellDataContainer::XLCellDataContainer(int r, int c):
m_Table(SETTINGS_CLASS->GetDefaultPocketSize(), SETTINGS_CLASS->GetDefaultOrder()),
m_iRow(r),
m_iCol(c)
{
	for(int i = 0; i < m_iRow; i++)
	{
		m_Table.Pushback(new XLCDRow(c));
	}
}

XLCellDataContainer::~XLCellDataContainer(void)
{
	for(int i = 0; i < m_iRow; i++)
	{
		XLCDRow** rowPtr = 0;
		if(m_Table.Value(i, rowPtr))
		{
			delete *rowPtr;
		}
	}
}

XLCellData* XLCellDataContainer::CellData(int r, int c)
{
	if(r < 0 || r >= m_iRow)
		return 0;
	if(c < 0 || c >= m_iCol)
		return 0;

	XLCDRow** rowPtr = 0;
	if(m_Table.Value(r, rowPtr))
	{
		return &(**rowPtr)[c];
	}
	return 0;
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

	XLCDRow** rowPtr = 0;
	if(m_Table.Value(r, rowPtr))
	{
		return *rowPtr;
	}
	return 0;
}


void XLCellDataContainer::AddRows(int rows) // Add AT last
{
	if(rows <= 0)
	{
		return;
	}
	for(int i = 0; i < rows; i++)
	{
		if(m_Table.Pushback(new XLCDRow(m_iCol)))
		{
			m_iRow++;
		}
	}
}
void XLCellDataContainer::AddCols(int cols) // Add AT last
{
	if(cols <= 0)
	{
		return;
	}
	m_iCol += cols;
	XLCDRow** rowPtr = 0;
	for(int i = 0; i < m_iRow; i++)
	{
		if(m_Table.Value(i, rowPtr))
		{
			(*rowPtr)->resize(m_iCol);
		}
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
void XLCellDataContainer::InsertRowAt(int row, bool dummyRow)
{
	InsertRowsAt(row, 1, dummyRow);
}

void XLCellDataContainer::InsertRowsAt(int startRow, int numRows, bool dummyRow)
{
	if(numRows <= 0)
	{
		return;
	}
	if((startRow <= 0) || (m_iRow <= startRow))
	{
		return;
	}

	for(int i = 0; i < numRows; i++)
	{
		if(m_Table.Insert(startRow, new XLCDRow(m_iCol, dummyRow)))
		{
			m_iRow++;
			startRow++;
		}
	}
}
XLCDRow* XLCellDataContainer::FindInRow(int stRow, int endRow, int* colIdx, int numCols, WStrType& str)
{
	XLCellData* cellData = 0;
	bool Found = false;
	int i = stRow;
	for(; i <= endRow; i++)
	{
		for(int j = 0; j < numCols; j++)
		{
			cellData = CellData(i, colIdx[j]);
			if(cellData)
			{
				if(cellData->ExactCompare(str))
				{
					Found = true;
					break;
				}
			}
		}
		if(Found)
		{
			break;
		}
		
	}
	return CellRow(i);
}
int XLCellDataContainer::GetBestMatch(int r1, int r2, XLCDRow* row)
{
	if(!mGoodRow(r1) || !mGoodRow(r2) || !row)
	{
		return INVALID_ROW;
	}

	XLCDRow* cellRow = 0;
	bool Found = false;
	int i = r1;
	int bestMatchIdx = r1;
	int currentMatch = 0;
	int bestMatch = 0;
	for(; i <= r2; i++)
	{
		cellRow = CellRow(i);
		if(cellRow)
		{
			currentMatch = cellRow->GetMatchNumber(*row);
			if(currentMatch > bestMatch)
			{
				bestMatch = currentMatch;
				bestMatchIdx = i;
			}
		}

		
	}
	return bestMatchIdx;
}

bool XLCellDataContainer::IsDummy(int row)
{
	XLCDRow* rowPtr = CellRow(row);
	if(!rowPtr)
	{
		return true;
	}
	return rowPtr->IsDummy();
}

bool XLCellDataContainer::IsEmpty(int row)
{
	XLCDRow* rowPtr = CellRow(row);
	if(!rowPtr)
	{
		return true;
	}
	for(int i = 0; i < rowPtr->size(); i++)
	{
		if(!(*rowPtr)[i].IsEmpty())
		{
			return false;
		}
	}
	return true;
}