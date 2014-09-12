#include "StdAfx.h"
#include "HistoryManager.h"

using namespace XLHistory;

CHistoryManager* CHistoryManager::m_ManagerInstance	= 0;


void XLHistory::InsertHistory(XLHistoryData::HistDataPair& histPair)
{
	CHistoryManager::GetInstance()->RecordHistory(histPair);
}
XLHistoryData::HistoryDataSPtr XLHistory::RemoveRecentHistory()
{
	return CHistoryManager::GetInstance()->GetRecentData();
}

void XLHistory::ClearHistory()
{
	CHistoryManager::GetInstance()->Clear();
}

int XLHistory::RemainingHistory()
{
	return CHistoryManager::GetInstance()->CurrentHistorySize();
}

void XLHistory::EnableHistory(bool set)
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


int CHistoryManager::CurrentHistorySize()
{
	return mHistoryData.Size();
}


void CHistoryManager::RecordHistory(XLHistoryData::HistDataPair& histPair)
{
	if(mbRecord)
	{
		mHistoryData.Insert(histPair);
	}
}

XLHistoryData::HistoryDataSPtr CHistoryManager::GetRecentData()
{
	XLHistoryData::HistoryDataSPtr cellData = mHistoryData.FindRecentAndPop();
	return cellData;
}

void CHistoryManager::Clear()
{
	mHistoryData.Clear();
	
}
