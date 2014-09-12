#ifndef _CHISTORY_MANAGER_H_
#define _CHISTORY_MANAGER_H_

#include "CommonHeader.h"
#include "HistoryData.h"

namespace XLHistory
{

void InsertHistory(XLHistoryData::HistDataPair& histPair);
XLHistoryData::HistoryDataSPtr RemoveRecentHistory();
void ClearHistory();
void EnableHistory(bool set);
int RemainingHistory();


class CHistoryManager
{
	static CHistoryManager*                m_ManagerInstance;
	XLHistoryData::CHistoryData            mHistoryData;
	bool                                   mbRecord;
private:
	CHistoryManager(void);
	CHistoryManager(const CHistoryManager& );

public:
	static CHistoryManager* GetInstance(); 
	void RecordHistory(XLHistoryData::HistDataPair& data); 
	XLHistoryData::HistoryDataSPtr GetRecentData();

	void Enable(bool set)
	{
		mbRecord = set;
	}
	void SetMaxHistory(int size)
	{
		mHistoryData.SetHistoryMaxSize(size);
	}

	int CurrentHistorySize();

	void Clear();
	~CHistoryManager(void);
};

}; //namespace XLHistory




#endif
