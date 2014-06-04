#ifndef _CHISTORY_MANAGER_H_
#define _CHISTORY_MANAGER_H_

#include "CommonHeader.h"
#include "HistoryData.h"


void InsertHistory(HistDataPair& histPair);
HistDataPair* RemoveRecentHistory();
void ClearHistory();
void EnableHistory(bool set);

class CHistoryManager
{
	static CHistoryManager* m_ManagerInstance;
	CHistoryData            mHistoryData;
	bool                    mbRecord;
private:
	CHistoryManager(void);
	CHistoryManager(const CHistoryManager& );

public:
	static CHistoryManager* GetInstance(); 
	void RecordHistory(HistDataPair& data); 
	HistDataPair* GetRecentData();

	void Enable(bool set)
	{
		mbRecord = set;
	}

	void Clear();
	~CHistoryManager(void);
};





#endif