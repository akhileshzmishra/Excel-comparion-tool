#ifndef ___CSETTINGS_H____
#define ___CSETTINGS_H____

#define LOAD_FILE_A_FLAG      1
#define LOAD_FILE_B_FLAG      2	
#define COMPARE_START_ENB     4
#define COMPARED_FILE_FLAG    8
#define SHOW_ONLY_DIFF_FLAG   16
#define SHOW_SAME_FLAG        32
#define SHOW_DIFF_ITR_FLAG    64
#define SHOW_ALL_FLAG         128

#include <string>
#include <string.h>
#include "BoyerMoore.h"
#include "CommonHeader.h"
enum
{
	FILE_A_LOADED,
	FILE_B_LOADED,
	COMPARE_DONE,
	SHOW_SAME,
	SHOW_ONLY_DIFF,
	SHOW_ALL
};

#define DEF_POCKET_SIZE 16
#define DEF_ORDER_SIZE 16


class CSettings
{
	int m_Flag;
	static CSettings* m_Instance;
private:
	CSettings(void);
public:
	static CSettings* GetInstance();
	~CSettings(void);
	void SetFlag(int flag);
	void ResetFlag(int flag);
	bool IsFlagSet(int flag);
};



void StoreFlag(int flag);
void RestoreFlag(int flag);
bool GetFlag(int flag);


enum DefaultSearchDir
{
	DefaultSearchDir_UP,
	DefaultSearchDir_DOWN
};


class CSettingsSM
{
	static CSettingsSM*     m_InstancesSM;
	std::wstring            m_UserFindString;
	DefaultSearchDir        m_DefaultSearchDirection;
	bool                    m_bMatchExact;
	BoyerMoore<wchar_t>     m_wstrComparator;
	bool                    m_bSetInit;
	int                     m_iDefPocketSize;
	int                     m_iDefOrder;
	double                  m_dMatchPercentage;
	XLComparatorOperation   m_OpType;
	bool                    m_bAutoCompare;
public:
	~CSettingsSM();
private:
	CSettingsSM();
	CSettingsSM(const CSettingsSM&);
public:
	static CSettingsSM* GetInstance();

	COLORREF GetSameColor();
	COLORREF GetDifferentColor();
	COLORREF GetSameTextColor();
	COLORREF GetDifferentTextColor();

	bool GetAutoCompare()
	{
		return m_bAutoCompare;
	}

	void SetAutoCompare(bool set)
	{
		m_bAutoCompare = set;
	}

	std::wstring GetUserString()
	{
		return m_UserFindString;
	}
	int GetDefaultPocketSize()
	{
		return m_iDefPocketSize;
	}
	void SetDefaultPocketSize(int size)
	{
		if(size > 0)
		{
			m_iDefPocketSize = size;
		}
	}
	int GetDefaultOrder()
	{
		return m_iDefOrder;
	}
	void SetDefaultOrder(int size)
	{
		if(size > 0)
		{
			m_iDefOrder = size;
		}
	}

	double GetMatchPercentage()
	{
		return m_dMatchPercentage;
	}

	void SetMatchPercentage(double percent)
	{
		if((percent > 0.0) && (percent < 1.0))
		{
			m_dMatchPercentage = percent;
		}
	}

	XLComparatorOperation GetOperationType()
	{
		return m_OpType;
	}
	void SetOperationType(XLComparatorOperation type)
	{
		m_OpType = type;
	}

	void SetUserString(std::wstring& str)
	{
		 m_UserFindString = str;
		 m_bSetInit = false;
	}
	void InitUserSearchStringComparator()
	{
		 m_wstrComparator.SetSubString(m_UserFindString.c_str(), (int)m_UserFindString.length());
		 m_bSetInit = true;
	}
	void SetMatchExact(bool set)
	{
		m_bMatchExact = set;
	}

	bool MatchExact()
	{
		return m_bMatchExact;
	}

	bool SubStringExistsInFindString(std::wstring& wstr)
	{
		if(!m_bSetInit)
		{
			return false;
		}
		if(m_wstrComparator.FindIndex(wstr.c_str(), (int)wstr.length()) == BoyerMoore<char>::No_Index)
		{
			return false;
		}
		return true;
	}

	void SetDefaultSearchDirection(DefaultSearchDir dir)
	{
		m_DefaultSearchDirection = dir;
	}
	DefaultSearchDir GetDefaultSearchDirection()
	{
		return m_DefaultSearchDirection;
	}

	void StateAchieved(int StateID);
private:
	void OnFileLoaded();
	void OnCompareDone();
};


#define SETTINGS_CLASS \
	CSettingsSM::GetInstance()


#endif