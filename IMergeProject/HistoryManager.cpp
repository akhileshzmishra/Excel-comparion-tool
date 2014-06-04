#include "StdAfx.h"
#include "HistoryManager.h"

CHistoryManager* CHistoryManager::m_ManagerInstance	= 0;


void InsertHistory(HistDataPair& histPair)
{
	CHistoryManager::GetInstance()->RecordHistory(histPair);
}
HistDataPair* RemoveRecentHistory()
{
	return CHistoryManager::GetInstance()->GetRecentData();
}

void ClearHistory()
{
	CHistoryManager::GetInstance()->Clear();
}

void EnableHistory(bool set)
{
	CHistoryManager::GetInstance()->Enable(set);
}

CHistoryManager::CHistoryManager(void):
mbRecord(true)
{
}

CHistoryManager::~CHistoryManager(void)
{
}

CHistoryManager* CHistoryManager::GetInstance()
{
	if(!m_ManagerInstance)
	{
		m_ManagerInstance = new CHistoryManager();
	}
	return m_ManagerInstance;
}


void CHistoryManager::RecordHistory(HistDataPair& histPair)
{
	if(mbRecord)
	{
		mHistoryData.Insert(histPair);
	}
}

HistDataPair* CHistoryManager::GetRecentData()
{
	HistDataPair* cellData = mHistoryData.FindRecentAndPop();
	if(cellData)
	{
		return cellData;
	}
	return 0;
}

void CHistoryManager::Clear()
{
	mHistoryData.Clear();
	
}