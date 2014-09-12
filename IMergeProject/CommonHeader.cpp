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


void RowComparisonList::Clear()
{
	mFirstRowPos.clear();
	mSecondRowPos.clear();
	mRowList.Clear();
	mFill = 0;
}


RowComparison* RowComparisonList::FindRow1(int r1)
{
	RowItr itr = mFirstRowPos.find(r1);
	if(itr != mFirstRowPos.end())
	{
		return &((itr->second)->Data());
	}
	return 0;
}
RowComparison* RowComparisonList::FindRow2(int r2)
{
	RowItr itr = mSecondRowPos.find(r2);
	if(itr != mSecondRowPos.end())
	{
		return &((itr->second)->Data());
	}
	return 0;
}

RowComparisonList::Node* RowComparisonList::GetRow1Node(int r1)
{
	RowComparisonList::Node* findNode = 0;
	RowItr itr = mFirstRowPos.find(r1);
	if(itr != mFirstRowPos.end())
	{
		return itr->second;
	}
	return 0;
}
RowComparisonList::Node* RowComparisonList::GetRow2Node(int r2)
{
	RowComparisonList::Node* findNode = 0;
	RowItr itr = mSecondRowPos.find(r2);
	if(itr != mSecondRowPos.end())
	{
		return itr->second;
	}
	return 0;
}
RowComparison* RowComparisonList::Insert(int r1, int r2)
{
	try
	{
		RowComparison rc(r1, r2);

		mRowList.InsertAtBack(rc);

		mFirstRowPos.insert(RowPair(r1, mRowList.BackNode()));
		mSecondRowPos.insert(RowPair(r2, mRowList.BackNode()));
		mFill++;
		return &(mRowList.BackNode()->Data());
	}
	catch(...)
	{
	}

	return 0;
}

RowComparison* RowComparisonList::AtLastFill()
{
	if(mRowList.BackNode())
	{
		return &(mRowList.BackNode()->Data());
	}
	return 0;
}

void RowComparisonList::Delete(int r1, int r2)
{
	Node* n = 0;
	RowItr itr = mFirstRowPos.find(r1);
	if(itr != mFirstRowPos.end())
	{
		n = itr->second;
		mRowList.Delete(n);
	}
	mFirstRowPos.erase(r1);
	mSecondRowPos.erase(r2);
}





RowComparisonIterator::RowComparisonIterator(RowComparisonList* mlist):
LinkedListItr<RowComparison>(&(mlist->mRowList)),
mList(mlist)
{
	Reset();
}
RowComparisonIterator::~RowComparisonIterator()
{
}

bool RowComparisonIterator::IsEnd()
{
	return AtEnd();
}
bool RowComparisonIterator::IsBegin()
{
	return AtFirst();
}
bool RowComparisonIterator::IsValid()
{
	if(!mList)
		return false;

	return Value() != 0;
}


void RowComparisonIterator::DeleteCurrValue()
{
	if(mList && Value())
	{
		mList->Delete(Value()->Row1, Value()->Row2);
	}
}

void RowComparisonIterator::ResetToFront()
{
	SetToBack();
}
void RowComparisonIterator::ResetToBack()
{
	SetToFront();
}

bool RowComparisonIterator::MoveToRow(int row, bool firstIndex)
{
	if(!mList)
	{
		return false;
	}
	RowComparisonList::Node* findValue = 0;
	if(firstIndex)
	{
		findValue = mList->GetRow1Node(row);
	}
	else
	{
		findValue = mList->GetRow2Node(row);
	}
	if(findValue)
	{
		__SetItrAT(findValue);
		return true;
	}
	return false;
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
