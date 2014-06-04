#include "StdAfx.h"
#include "XLComparator.h"
#include "CommonHeader.h"

XLComparator::XLComparator(int baseComparator):
m_iBaseCellColIndex(baseComparator)
{
}

XLComparator::~XLComparator(void)
{
}

void XLComparator::Reset()
{
	m_UnChangedRows.clear();
	m_ChangedRows.clear();
}


bool XLComparator::Compare(XLCellDataContainer* A, XLCellDataContainer* B)
{
	if(!A || !B)
		return false;
	Reset();
	XLCellDataProvider rowProvider1(m_iBaseCellColIndex, A);
	XLCellDataProvider rowProvider2(m_iBaseCellColIndex, B);
	m_R1 = A->Row();
	m_R2 = B->Row();
	XLDataCompareClass xlcomp(A->Row(), B->Row(), rowProvider1, rowProvider2);
	int length = 0;

	try
	{
		if(!xlcomp.Compare(m_UnChangedRows))
		{
			return false;
		}
		m_FindChangedRows();
	}
	catch(...)
	{
	}
	return true;
}

void XLComparator::m_FindChangedRows()
{
	int R1 = m_R1;
	int R2 = m_R2;
	int TableMaxSize = R1;	 
	if(TableMaxSize < R2)
	{
		TableMaxSize = R2;
	}
	//TableMaxSize += XLUtility::GetInstance()->GetDefaultEmptyRows();

	int length = m_UnChangedRows.size();
	if(length == 0)
	{
		m_ChangedRows.clear();
		m_ChangedRows.resize(TableMaxSize);
		for(int i = 0; i < R1; i++)
		{
			m_ChangedRows[i].first = i;
		}
		for(int i = 0; i < R2; i++)
		{
			m_ChangedRows[i].second = i;
		}
	}
	else
	{
		m_ChangedRows.clear(); 
		m_ChangedRows.resize((TableMaxSize - length), ComparedDataPair(INVALIDROW, INVALIDROW));

		int i = 0;
		int j1 = 0;
		int j2 = 0;
		int k1 = 0;
		int k2 = 0;

		int unchangedRowsSize = m_UnChangedRows.size() - 1;
		//Starting
		int jd = 0;
		while(jd < m_UnChangedRows[0].first)
		{
			if(j1 < m_ChangedRows.size())
			{
				m_ChangedRows[j1].first = jd;
				j1++;
			}
			jd++;
		}
		jd = 0;
		while(jd < m_UnChangedRows[0].second)
		 {
			 if(j2 < m_ChangedRows.size())
			 {
				m_ChangedRows[j2].second = jd;
				j2++;
			 }
			 jd++;
		 }

		//For first
		for(i = 1; i < unchangedRowsSize; i++)
		{
			 if(m_UnChangedRows[i].first !=  (m_UnChangedRows[i - 1].first + 1))
			 {
				 int j = m_UnChangedRows[i - 1].first + 1;	
				 while(j < m_UnChangedRows[i].first)
				 {
					 if(j1 < m_ChangedRows.size())
					 {
						m_ChangedRows[j1].first = j;
						j1++;
					 }
					 j++;
				 }
			 }

			 if(m_UnChangedRows[i].second !=  (m_UnChangedRows[i - 1].second + 1))
			 {
				 int j = m_UnChangedRows[i - 1].second + 1;
				 while(j < m_UnChangedRows[i].second)
				 {
					 if(j2 < m_ChangedRows.size())
					 {
						m_ChangedRows[j2].second = j;
						j2++;
					 }
					 j++;
				 }
			 }
		}

		//Ending
		jd = m_UnChangedRows[unchangedRowsSize].first + 1;
		while(jd < R1)
		 {
			 if(j1 < m_ChangedRows.size())
			 {
				m_ChangedRows[j1].first = jd;
				j1++;
			 }
			 jd++;
		 }
		jd = m_UnChangedRows[unchangedRowsSize].second + 1;
		while(jd < R2)
		 {
			 if(j2 < m_ChangedRows.size())
			 {
				m_ChangedRows[j2].second = jd;
				j2++;
			 }
			 jd++;
		 }
	}
}
bool XCellDataComparator::CompareRows(XLCDRow& A, XLCDRow& B)
{
	//return (A[0] == B[0]);
	if(A.size() != B.size())
	{
		return false;
	}
	for(int i = 0; i < A.size(); i++)
	{
		if(A[i] == B[i])
		{
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool XCellExhaustiveDataComparator::CompareRows(XLCDRow& A, XLCDRow& B)
{
	XLRowElementDataProvider rowProvider1(&A);
	XLRowElementDataProvider rowProvider2(&B);
	XLRowCompareClass comparator(A.size(), B.size(), rowProvider1, rowProvider2);
	RETURNTYPE retval;
	if(comparator.Compare(retval))
	{
		if(retval.size() > 0)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	return false;
}




XLDifferenceData::XLDifferenceData():
mRow1(0),
mRow2(0),
mNext(NULL),
mPrevious(NULL),
mIteratorStarted(false)
{
}


void XLDifferenceData::InsertDifference(int first, int second)
{
	mColList.push_back(XLDifferenceData::Column(first, second));
}
XLDifferenceData::Column* XLDifferenceData::GetNextDifference()
{
	XLDifferenceData::Column* retVal = NULL;
	if(!mIteratorStarted)
	{
		mItr = mColList.begin();
		mIteratorStarted = true;
	}
	if(mItr != mColList.end())
	{
		retVal = &(*mItr);
		mItr++;
	}
	return retVal;
}
void XLDifferenceData::RemovePresentColumn()
{
	mColList.erase(mItr);
}

void XLDifferenceData::Reset()
{
	mItr = mColList.begin();
	mIteratorStarted = false;
}


XLDifferenceDataList::XLDifferenceDataList(int intialCap ):
mList(intialCap, NULL),
mItr(0),
mInitialized(false),
mCapacity(intialCap),
mSize(0),
mPos(0)
{
}

XLDifferenceData*  XLDifferenceDataList::GetNextDifference()
{
	XLDifferenceData* retVal = NULL;

	if(mItr < mSize)
	{
		//retVal = (*mItr);
	}
	return retVal;
}
void XLDifferenceDataList::Reset()
{
	mInitialized = false;
}
void XLDifferenceDataList::RemovePresentRow()
{
	if(mList[mItr])
	{
		//if(
	}
}

void XLDifferenceDataList::Insert(XLDifferenceData* data)
{
	if(mSize == mCapacity)
	{
		mCapacity *= 2;
		mList.resize(mCapacity, NULL);
	}
	mList[mPos] = data;
	int i = mSize - 1;
	while(i >= 0)
	{
		if(mList[i])
		{
			mList[i]->SetNext(mSize);
			mList[mSize]->SetPrevious(i);
			break;
		}
		i--;
	}
	mPos++;
	mSize++;
}

void XLDifferenceDataList::Clear()
{
	Reset();
	mList.clear();
}
