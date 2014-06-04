#ifndef _CHISTORY_DATA_H_
#define _CHISTORY_DATA_H_

#pragma once

#include "XLCellData.h"
#include "CommonHeader.h"
#include "PositionManager.h"
#include "LinkedList.h"
#include <stack>


struct HistDataPair
{
	XLCellData      Data;
	CellIdentifier  Cell;
	int             TableID;


	HistDataPair():TableID(0){}
	HistDataPair(const HistDataPair& other)
	{
		Data = other.Data;
		Cell = other.Cell;
		TableID = other.TableID;
	}
	HistDataPair& operator = (const HistDataPair& other)
	{
		if(this == &other)
		{
			return *this;
		}
		Data = other.Data;
		Cell = other.Cell;
		TableID = other.TableID;
		return *this;
	}
};

typedef LinkedList<HistDataPair> HistoryLinkedList;
typedef LinkedListItr<HistDataPair> HistoryLinkedListItr;

class CHistoryData
{
	HistoryLinkedList                   mHistoryData;
	int                                 mHistorySize;
	HistoryLinkedListItr                mCurrentIndex;
	int                                 mBaseIdx;
	const int                           mHistoryMaxSize;
public:
	CHistoryData(void);
	~CHistoryData(void);

	void Insert(HistDataPair& histData);
	HistDataPair* FindRecentAndPop();
	HistDataPair* FindRecent();	
	int Size()                                 { return mHistorySize;} 
	int MaxSize()                              { return mHistoryMaxSize;}

	void Clear();

};




#endif