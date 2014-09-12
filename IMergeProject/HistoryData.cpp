#include "StdAfx.h"
#include "HistoryData.h"

using namespace XLHistoryData;

CHistoryData::CHistoryData(void):
mHistoryMaxSize(MAX_HISTORY_SIZE)
{
}

CHistoryData::~CHistoryData(void)
{
}


void CHistoryData::Insert(HistDataPair& histData)
{
	if(mHistoryData.Size() == mHistoryMaxSize)
	{
		HistDataPair output;
		mHistoryData.RemoveFromBack(output);
	}
	mHistoryData.InsertAtBack(histData);
}


HistoryDataSPtr CHistoryData::FindRecentAndPop()
{
	HistDataPair histData;
	if(mHistoryData.RemoveFromBack(histData))
	{
		HistoryDataSPtr retVal(new HistDataPair(histData));		
		return retVal;
	}
	return HistoryDataSPtr(0);	
}

HistDataPair* CHistoryData::FindRecent()
{
	if(mHistoryData.BackNode())
	{
		return &(mHistoryData.BackNode()->Data());
	}
	return 0;
}

void CHistoryData::Clear()
{ 
	mHistoryData.Clear();
}

void CHistoryData::SetHistoryMaxSize(int size)
{
	if(size > 0)
	{
		mHistoryMaxSize = size;
		HistDataPair histData;
		while(mHistoryData.Size() >= mHistoryMaxSize)
		{
			mHistoryData.RemoveFromBack(histData);
		}
	}
}
