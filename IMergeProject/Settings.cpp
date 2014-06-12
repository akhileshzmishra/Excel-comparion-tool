#include "StdAfx.h"
#include "Settings.h"
#include "CommonHeader.h"
CSettings* CSettings::m_Instance = NULL;
CSettingsSM* CSettingsSM::m_InstancesSM = NULL;




void StoreFlag(int flag)
{
	CSettings::GetInstance()->SetFlag(flag);
}
void RestoreFlag(int flag)
{
	CSettings::GetInstance()->ResetFlag(flag);
}
bool GetFlag(int flag)
{
	return CSettings::GetInstance()->IsFlagSet(flag);
}




CSettings::CSettings(void):
m_Flag(0)
{
}

CSettings::~CSettings(void)
{
}


CSettings* CSettings::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new CSettings();
	}
	return m_Instance;
}


void CSettings::SetFlag(int flag)
{
	m_Flag = m_Flag | flag;
}
void CSettings::ResetFlag(int flag)
{
	m_Flag &= (~flag);
}
bool CSettings::IsFlagSet(int flag)
{
	return ((m_Flag & flag) > 0);
}



CSettingsSM::~CSettingsSM()
{
}

CSettingsSM::CSettingsSM():
m_DefaultSearchDirection(DefaultSearchDir_DOWN),
m_bMatchExact(false),
m_bSetInit(false)
{
}
CSettingsSM* CSettingsSM::GetInstance()
{
	if(!m_InstancesSM)
	{
		m_InstancesSM = new CSettingsSM();
	}
	return m_InstancesSM;
}

void CSettingsSM::StateAchieved(int StateID)
{
	switch(StateID)
	{
	case FILE_A_LOADED:
		StoreFlag(LOAD_FILE_A_FLAG);
		OnFileLoaded();
		break;
	case FILE_B_LOADED:
		StoreFlag(LOAD_FILE_B_FLAG);
		OnFileLoaded();
		break;
	case COMPARE_DONE:
		StoreFlag(COMPARED_FILE_FLAG);
		OnCompareDone();
		break;
	case SHOW_SAME:
		RestoreFlag(SHOW_DIFF_ITR_FLAG);
		break;
	case SHOW_ONLY_DIFF:
		StoreFlag(SHOW_DIFF_ITR_FLAG);
		break;
	case SHOW_ALL:
		StoreFlag(SHOW_DIFF_ITR_FLAG);
		break;
	default:
		break;
	}
}


void CSettingsSM::OnFileLoaded()
{
	//SHOW_ALL_FLAG
	RestoreFlag(COMPARE_START_ENB);
	RestoreFlag(COMPARED_FILE_FLAG);
	RestoreFlag(SHOW_DIFF_ITR_FLAG);
	RestoreFlag(SHOW_SAME_FLAG);
	RestoreFlag(SHOW_ONLY_DIFF_FLAG);
	RestoreFlag(SHOW_ALL_FLAG);
	if(GetFlag(LOAD_FILE_A_FLAG) &&	 GetFlag(LOAD_FILE_B_FLAG))
	{
		StoreFlag(COMPARE_START_ENB);
	}
}


void CSettingsSM::OnCompareDone()
{
	StoreFlag(SHOW_DIFF_ITR_FLAG);
	StoreFlag(SHOW_SAME_FLAG);
	StoreFlag(SHOW_ONLY_DIFF_FLAG);
	StoreFlag(SHOW_ALL_FLAG);
}



COLORREF CSettingsSM::GetSameColor()
{
	return MEDAQUAMARCOL;
}
COLORREF CSettingsSM::GetDifferentColor()
{
	return DARKREDCOL;
}
COLORREF CSettingsSM::GetSameTextColor()
{
	return BLUECOL;
}
COLORREF CSettingsSM::GetDifferentTextColor()
{
	return DARKSLATEGRAYCOL;
}