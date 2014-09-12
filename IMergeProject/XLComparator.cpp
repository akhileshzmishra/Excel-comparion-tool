#include "StdAfx.h"
#include "XLComparator.h"
#include "CommonHeader.h"
#include "Settings.h"



XLComparator::XLComparator(std::vector<int>& keys):
m_vUniqueKeys(keys),
m_iOperationType(XLComparatorOperation_O1),
m_A(0),
m_B(0)
{
}

XLComparator::XLComparator():
m_iOperationType(XLComparatorOperation_O1),
m_A(0),
m_B(0)
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

void XLComparator::SetUniqueRows(std::vector<int>& ColNum)
{
	m_vUniqueKeys.clear();
	m_vUniqueKeys = ColNum;
	if((m_vUniqueKeys.size() == 0))
	{
		m_vUniqueKeys.push_back(0); //atleast the first col should become the key
	}
}


bool XLComparator::Compare(XLCellDataContainer* A, XLCellDataContainer* B)
{
	if(!A || !B)
		return false;

	m_A = A;
	m_B = B;
	m_R1 = m_A->Row();
	m_R2 = m_B->Row();

	try
	{
		if(!m_CompareBasic())
		{
			return false;
		}		
		if(SETTINGS_CLASS->GetOperationType() == XLComparatorOperation_O2)
		{
			m_RectifyContainers(A, B);
		}
		m_FindChangedRows(A, B);
	}
	catch(...)
	{
	}
	return true;
}


bool XLComparator::m_CompareBasic()
{
	Reset();
	XLCellDataProvider rowProvider1(m_A);
	XLCellDataProvider rowProvider2(m_B);	
	XLDataCompareClass xlcomp(rowProvider1, rowProvider2);

	if (xlcomp.Compare(m_UnChangedRows))
	{
		return true;
	}

	return false;
}


void XLComparator::m_RectifyContainers(XLCellDataContainer* A, XLCellDataContainer* B)
{
	if(!A || !B || (m_UnChangedRows.size() == 0))
	{
		return;
	}

	XLCellDataProvider rowProviderA(A);
	XLCellDataProvider rowProviderB(B);	
	XLDataMaxCompareClass xlcomp(rowProviderA, rowProviderB);

	//We will divide the xcel sheet into sections with consecutive
	//numbers from m_UnChangedRows
	int ARowDiff = 0;
	int BRowDiff = 0;

	int lowerBase = 0;
	for(int i = 0; i < m_UnChangedRows.size(); i++)
	{
		m_UnChangedRows[i].first += ARowDiff;
		m_UnChangedRows[i].second += BRowDiff;
		rowProviderA.SetRowLimit(lowerBase, m_UnChangedRows[i].first);
		rowProviderB.SetRowLimit(lowerBase, m_UnChangedRows[i].second);

		int lengthA = rowProviderA.Span();
		int lengthB = rowProviderB.Span();

		if(lengthA + lowerBase >= A->Row())
		{
			lengthA = A->Row() - lowerBase;
		}
		if(lengthB + lowerBase >= B->Row())
		{
			lengthB = B->Row() - lowerBase;
		}
		RETURNTYPE r;
		int minChangeA = 0;
		int minChangeB = 0;
		if(xlcomp.CompareAPart(r, lengthA, lengthB))
		{
			int diffItr = 0;
			while(diffItr < r.size())
			{
				r[diffItr].first += minChangeA;
				r[diffItr].second += minChangeB;
				int RA = r[diffItr].first;// - lowerBase;
				int RB = r[diffItr].second;// - lowerBase;
				if(RA < RB)
				{
					int numRows = RB - RA;
					A->InsertRowsAt(r[diffItr].first + lowerBase, numRows, true);
					minChangeA += numRows;
				}
				else if(RA > RB)
				{
					int numRows = RA - RB;
					B->InsertRowsAt(r[diffItr].second + lowerBase, numRows, true);
					minChangeB += numRows;
				}
				diffItr++;
			}

			m_UnChangedRows[i].first += minChangeA;
			m_UnChangedRows[i].second += minChangeB;

			if(m_UnChangedRows[i].first < m_UnChangedRows[i].second)
			{
				int numRows = m_UnChangedRows[i].second - m_UnChangedRows[i].first;
				A->InsertRowsAt(m_UnChangedRows[i].first, numRows, true);
				m_UnChangedRows[i].first = m_UnChangedRows[i].second;
				minChangeA += numRows;
			}
			else if(m_UnChangedRows[i].first > m_UnChangedRows[i].second)
			{
				int numRows = m_UnChangedRows[i].first - m_UnChangedRows[i].second;
				B->InsertRowsAt(m_UnChangedRows[i].second, numRows, true);
				m_UnChangedRows[i].second = m_UnChangedRows[i].first;
				minChangeB += numRows;
			}			
		}
		//m_UnChangedRows[i].first += minChangeA;
		//m_UnChangedRows[i].second += minChangeB;
		ARowDiff += minChangeA;
		BRowDiff += minChangeB;
		lowerBase = m_UnChangedRows[i].first;
	}
}


bool XLComparator::m_CompareUniqueKeys(XLCDRow* r1, XLCDRow* r2)
{
	if((m_vUniqueKeys.size() == 0) || !r1 || !r2)
	{
		return true;
	}

	if((m_vUniqueKeys.size() > r1->size()) || (m_vUniqueKeys.size() > r2->size()))
	{
		return false;
	}

	bool Equal = true;

	for(int i = 0; i < m_vUniqueKeys.size(); i++)
	{
		if((*r1)[m_vUniqueKeys[i]] == (*r2)[m_vUniqueKeys[i]])
		{
			continue;
		}
		Equal = false;
		break;
	}

	return Equal;

}

void XLComparator::m_FindChangedRows(XLCellDataContainer* A, XLCellDataContainer* B)
{
	int R1 = A->Row();
	int R2 = B->Row();
	int TableMaxSize = R1;	 
	if(TableMaxSize < R2)
	{
		TableMaxSize = R2;
	}
	//TableMaxSize += XLUtility::GetInstance()->GetDefaultEmptyRows();

	int length = m_UnChangedRows.size();
	if(length == 0)
	{
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
	//If any dummy row is in comparison, we say that it cannot be compared.
	//Part of the logic being that we do not want dummy/empty rows in the unchanged  list
	if(A.IsDummy() || B.IsDummy())
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


bool XCellDataMaxComparator::CompareRows(XLCDRow& A, XLCDRow& B)
{
	//return (A[0] == B[0]);
	if(A.size() != B.size())
	{
		return false;
	}
	if(A.IsDummy() || B.IsDummy())
	{
		return true;
	}
	int matcher = 0;
	for(int i = 0; i < A.size(); i++)
	{
		if(A[i] == B[i])
		{
			matcher++;
		}
	}

	double percentage = ((double)matcher/((double)A.size() + 1.0));
	if(percentage >= SETTINGS_CLASS->GetMatchPercentage())
	{
		return true;
	}

	return false;
}



