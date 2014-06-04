#include "StdAfx.h"
#include "CommonHeader.h"

XLUtility* XLUtility::m_Instance = NULL;


void RowComparison::Insert(int c1, int c2)
{
	mColMap.Insert(c1, c2);
}

bool RowComparison::FindC2(int c2, int& ret)
{
	ret = 0;
	if(!mColMap.FindFirst(c2, ret))
	{
		return false;
	}

	return true;
}

bool RowComparison::FindC1(int c1, int& ret)
{
	ret = 0;
	if(!mColMap.FindSecond(c1, ret))		
	{
		return false;
	}

	return true;
}

void RowComparison::Erase(int c1, int c2)
{
	mColMap.Delete(c2, c1);
}

RowComparisonList::RowComparisonList():
mFill(0)
{
}
RowComparisonList::~RowComparisonList()
{
}
void RowComparisonList::resize(int size)
{
	clear();
	mRowList.resize(size);
}
void RowComparisonList::clear()
{
	mFirstRowPos.clear();
	mSecondRowPos.clear();
	mRowList.clear();
	mFill = 0;
}


RowComparison* RowComparisonList::FindRow1(int r1)
{
	RowItr itr = mFirstRowPos.find(r1);
	if(itr != mFirstRowPos.end())
	{
		int index = (itr->second);
		return &mRowList[index];
	}
	return 0;
}
RowComparison* RowComparisonList::FindRow2(int r2)
{
	RowItr itr = mSecondRowPos.find(r2);
	if(itr != mSecondRowPos.end())
	{
		int index = (itr->second);
		return &mRowList[index];
	}
	return 0;
}
void RowComparisonList::InsertAtLast(int r1, int r2)
{
	if(mFill >= mRowList.size())
	{
		mRowList.resize(2*mFill);
	}

	mRowList[mFill].Row1 = r1;
	mRowList[mFill].Row2 = r2;

	mFirstRowPos.insert(RowPair(r1, mFill));
	mSecondRowPos.insert(RowPair(r2, mFill));
	mFill++;
}

RowComparison* RowComparisonList::AtLastFill()
{
	if(mFill < 0 || mFill >= mRowList.size())
	{
		return 0;
	}
	return (&mRowList[mFill - 1]);
}

void RowComparisonList::Delete(int r1, int r2)
{
	mFirstRowPos.erase(r1);
	mSecondRowPos.erase(r2);
}

int RowComparisonList::FindIndex1(int r1)
{
	RowItr itr = mFirstRowPos.find(r1);
	if(itr != mFirstRowPos.end())
	{
		int index = (itr->second);
		return index;
	}
	return -1;
}
int RowComparisonList::FindIndex2(int r2)
{
	RowItr itr = mSecondRowPos.find(r2);
	if(itr != mSecondRowPos.end())
	{
		int index = (itr->second);
		return index;
	}
	return -1;
}





RowComparisonIterator::RowComparisonIterator(RowComparisonList* mlist):
mitr(0),
mList(mlist),
mThisValue(0)
{
	Reset();
}
RowComparisonIterator::~RowComparisonIterator()
{
}

bool RowComparisonIterator::IsEnd()
{
	if(!mList)
		return false;
	return ((mitr == mList->mFill));
}
bool RowComparisonIterator::IsBegin()
{
	if(!mList)
		return false;

	return ((mitr < 0));
}
bool RowComparisonIterator::IsValid()
{
	if(!mList)
		return false;

	return ((mitr >= 0) && (mitr < mList->mFill));
}
void RowComparisonIterator::Reset(bool toLast)
{
	if(!mList)
		return;
	
	RowComparison* comp1 = 0, *comp2 = 0;
	mThisValue = 0;
	if(toLast)
	{
		mitr =	mList->mFill - 1;
		while(!comp1 && !comp2 && IsValid())
		{
			comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
			comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
			if(!comp1 && !comp2)
			{
				mitr--;
			}
		}
		mThisValue = comp1; 
	}
	else
	{
		mitr = 0;
		while(!comp1 && !comp2 && IsValid())
		{
			comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
			comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
			if(!comp1 && !comp2)
			{
				mitr++;
			}
		}
		mThisValue = comp1; 
	}
}
void RowComparisonIterator::operator ++()
{
	if(!mList)
		return;
	RowComparison* comp1 = 0, *comp2 = 0;
	mThisValue = 0;
	mitr++;
	while(!comp1 && !comp2 && IsValid())
	{ 		
		comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
		comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
		if(!comp1 && !comp2)
		{
			mitr++;
		}
		
	}
	if(comp1 && comp2)
	{
		mThisValue = comp1; 
	}
}
void RowComparisonIterator::operator ++(int)
{
	if(!mList)
		return;

	RowComparison* comp1 = 0, *comp2 = 0;
	mThisValue = 0;
	mitr++;
	while(!comp1 && !comp2 && IsValid())
	{
		comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
		comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
		if(!comp1 && !comp2)
		{
			mitr++;
		}
		
	}
	if(comp1 && comp2)
	{
		mThisValue = comp1; 
	}
}


void RowComparisonIterator::operator --()
{
	if(!mList)
		return;
	RowComparison* comp1 = 0, *comp2 = 0;
	mitr--;
	mThisValue = 0;
	while(!comp1 && !comp2 && IsValid())
	{
		comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
		comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
		if(!comp1 && !comp2)
		{
			mitr--;
		}
		
	}
	if(comp1 && comp2)
	{
		mThisValue = comp1; 
	}
}
void RowComparisonIterator::operator --(int)
{
	if(!mList)
		return;

	RowComparison* comp1 = 0, *comp2 = 0;
	mThisValue = 0;
	mitr--;
	while(!comp1 && !comp2 && IsValid())
	{
		comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
		comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
		if(!comp1 && !comp2)
		{
			mitr--;
		}
		
	}
	if(comp1 && comp2)
	{
		mThisValue = comp1; 
	}
}
void RowComparisonIterator::DeleteCurrValue()
{
	if(!mList)
		return;
	if(!IsValid())
		return;

	if(!GetValue())
		return;

	mList->Delete(GetValue()->Row1, GetValue()->Row2);
}



void RowComparisonIterator::SetAtPlace(int placeIdx)
{
	int nextPlace = placeIdx;
	if ((nextPlace >= 0) && (nextPlace < mList->mFill))
	{
		mitr = nextPlace;
	}
	RowComparison* comp1 = 0, *comp2 = 0;
	if(!comp1 && !comp2 && IsValid())
	{
		comp1 = mList->FindRow1(mList->mRowList[mitr].Row1);
		comp2 = mList->FindRow2(mList->mRowList[mitr].Row2);
	}
	if(comp1 && comp2 && IsValid())
	{
		mThisValue = comp1; 
	}
}



XLUtility::XLUtility()   :
m_Parent(NULL),
m_bVerticalInterlocked(true)
{
}

XLUtility::~XLUtility()
{
}

XLUtility* XLUtility::GetInstance(CView* view)
{
	if(!m_Instance)
	{
		m_Instance = new XLUtility();
		m_Instance->Initialize(view);
	}
	return m_Instance;
}

void XLUtility::Initialize(CView* view)
{
	m_Parent = view;
}

void XLUtility::VerticallyDivideArea(int num, int* partitions, int& width)
{
	if(m_Parent && partitions)
	{
		CRect WRect;
		m_Parent->GetDesktopWindow()->GetWindowRect(&WRect);

		int tableWidth = (WRect.Width() - FIRST_TABLE_X - LEFT_GAP)/num;

		for(int i = 0; i < num; i++)
		{
			partitions[i] = (tableWidth + TABLE_GAP)*i + FIRST_TABLE_X;
		}

		width = tableWidth;

	}
}

int XLUtility::GetWindowHeight()
{
	if(m_Parent)
	{
		RECT WRect;
		m_Parent->GetDesktopWindow()->GetWindowRect(&WRect);
		return (WRect.bottom - WRect.top);
	}
	return 300;
}

int XLUtility::GetTableY()
{
	return FIRST_TABLE_Y;
}


bool XLUtility::IsVerticalScrollInterlocked()
{
	return m_bVerticalInterlocked;
}
bool XLUtility::IsHorizontalScrollInterlocked()
{
	return true;
}