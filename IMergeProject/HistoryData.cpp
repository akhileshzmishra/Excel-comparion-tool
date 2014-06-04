#include "StdAfx.h"
#include "HistoryData.h"

CHistoryData::CHistoryData(void):
mHistorySize(0),
mCurrentIndex(&mHistoryData),
mBaseIdx(0),
mHistoryMaxSize(MAX_HISTORY_SIZE)
{
}

CHistoryData::~CHistoryData(void)
{
}


void CHistoryData::Insert(HistDataPair& histData)
{
	if(mHistorySize == 0)
	{
		mHistoryData.InsertAtBack(histData);
		mBaseIdx = 1;
		mHistorySize++;
	}
	else if(mHistorySize < mHistoryMaxSize)
	{
		if(mCurrentIndex.AtFirst() && (mBaseIdx == 0))
		{
			mCurrentIndex.Node()->Data() = histData;
			mBaseIdx = 1;
		}
		else if(mCurrentIndex.HasNext())
		{
			mCurrentIndex++;
			mCurrentIndex.Node()->Data() = histData;
			mBaseIdx++;
		}
		else
		{
			mHistoryData.InsertAtBack(histData);
			mCurrentIndex++;
			mHistorySize++;
			mBaseIdx++;
		}
	}
	else
	{
		mHistoryData.PopFrontPushBack();
		mCurrentIndex++;
		mCurrentIndex.Node()->Data() = histData;
	}
}


HistDataPair* CHistoryData::FindRecentAndPop()
{
	if(mHistorySize <= 0)
	{
		return 0;
	}
	HistDataPair* recentElement = mCurrentIndex.Value();
	if(mCurrentIndex.HasPrevious())
	{ 		
		mCurrentIndex--;
		mBaseIdx--;
		
	}
	return recentElement;
	
}

HistDataPair* CHistoryData::FindRecent()
{
	if(mHistorySize <= 0)
	{
		return 0;
	}
	HistDataPair* recentElement = mCurrentIndex.Value();
	return recentElement;
}

void CHistoryData::Clear()
{ 
	mCurrentIndex.Reset();
	mBaseIdx = 0;
}
