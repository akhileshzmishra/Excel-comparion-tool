#include "StdAfx.h"
#include "XLCellData.h"
#include "Settings.h"

static char ConvertingPlace[2048];

XLCellData::XLCellData(void): 
m_eDataType(InvalidDataType),
m_bFormula(false)
{
	memset(&m_vData, 0, sizeof(Data));
}

XLCellData::~XLCellData(void)
{
	Reset();
}


void XLCellData::SetData(int i)
{
	m_eDataType = Integer;
	m_vData.m_iData = i;
}
void XLCellData::SetData(double d)
{
	m_eDataType = Double;
	m_vData.m_dData = d;
}
void XLCellData::SetData(char c)
{
	m_eDataType = Char;
	m_vData.m_cData = c;
}
void XLCellData::SetData(char* str)
{
	Reset();
	m_eDataType = String;
	if(str)
	{
		m_vData.m_pstrData = new std::string(str);
	}
	else
	{
		m_vData.m_pstrData = new std::string(" ");
	}
}
void XLCellData::SetData(std::string& str)
{	
	Reset();
	m_eDataType = String;
	m_vData.m_pstrData = new std::string(str);
}

void XLCellData::SetFormula(std::string& str)
{
	Reset();
	m_bFormula = true;
	SetData(str);
}

bool XLCellData::GetData(int& i)
{
	if(m_eDataType != Integer)
		return false;
	i = m_vData.m_iData;
	return true;
}
bool XLCellData::GetData(double& d)
{
	if(m_eDataType != Double)
		return false;
	d = m_vData.m_dData;
	return true;
}
bool XLCellData::GetData(char& c)
{
	if(m_eDataType != Char)
		return false;
	c = m_vData.m_cData;
	return true;
}
bool XLCellData::GetData(char*& str)
{
	if(m_eDataType != String)
		return false;
	str = new char[m_vData.m_pstrData->size() + 1];
	memcpy(str, m_vData.m_pstrData->c_str(), m_vData.m_pstrData->size());
	str[m_vData.m_pstrData->size()] = 0;
	return true;
}
bool XLCellData::GetData(std::string& str)
{
	if(m_eDataType != String)
		return false;
	str = *(m_vData.m_pstrData);
	return true;
}

bool XLCellData::GetFormula(std::string& str)
{
	if(IsFormula())
	{
		return GetData(str);
	}
	return false;
}


XLCellData::XLCellData(const XLCellData& other):
m_eDataType(other.m_eDataType),
m_bFormula(other.m_bFormula)
{
	switch(m_eDataType)
	{
	case Integer:
		m_vData.m_iData = other.m_vData.m_iData;
		break;
	case Double:
		m_vData.m_dData = other.m_vData.m_dData;
		break;
	case Char:
		m_vData.m_cData = other.m_vData.m_cData;
		break;
	case String:
		{
			m_vData.m_pstrData = new std::string(*other.m_vData.m_pstrData);
		}
		break;
	case WString:
		{
			m_vData.m_pwstrData = new std::wstring(*other.m_vData.m_pwstrData);
		}
		break;
	default:
		break;
	}
}
XLCellData& XLCellData::operator = (const XLCellData& other)
{
	if(this == &other)
		return *this;
	if(m_eDataType == String)
	{
		delete m_vData.m_pstrData;
		m_vData.m_pstrData = 0;
	}
	else if(m_eDataType == WString)
	{
		delete m_vData.m_pwstrData;
		m_vData.m_pwstrData = 0;
	}
	m_eDataType = other.m_eDataType;
	m_bFormula = other.m_bFormula;
	switch(m_eDataType)
	{
	case Integer:
		m_vData.m_iData = other.m_vData.m_iData;
		break;
	case Double:
		m_vData.m_dData = other.m_vData.m_dData;
		break;
	case Char:
		m_vData.m_cData = other.m_vData.m_cData;
		break;
	case String:
		{
			m_vData.m_pstrData = new std::string(*other.m_vData.m_pstrData);
		}
		break;
	case WString:
		{
			m_vData.m_pwstrData = new std::wstring(*other.m_vData.m_pwstrData);
		}
		break;
	default:
		break;
	}
	return *this;
}

void XLCellData::SetData(wchar_t* str)
{
	Reset();
	m_eDataType = WString;
	if(str)
	{
		m_vData.m_pwstrData = new std::wstring(str);
	}
	else
	{
		std::string Empty("Empty");
		m_vData.m_pwstrData = new std::wstring(Empty.begin(), Empty.end());
	}
}

void XLCellData::SetData(std::wstring& str)
{
	Reset();
	m_eDataType = WString;
	m_vData.m_pwstrData = new std::wstring(str);
}

void XLCellData::SetData(const wchar_t* str)
{
	Reset();
	m_eDataType = WString;
	if(str)
	{
		m_vData.m_pwstrData = new std::wstring(str);
	}
	else
	{
		std::string Empty("Empty");
		m_vData.m_pwstrData = new std::wstring(Empty.begin(), Empty.end());
	}
}
void XLCellData::SetData(const char* str)
{
	Reset();
	m_eDataType = String;
	if(str)
	{
		m_vData.m_pstrData = new std::string(str);
	}
	else
	{
		m_vData.m_pstrData = new std::string(" ");
	}
}

bool XLCellData::GetData(const wchar_t*& str)
{
	if(m_eDataType == WString)
	{
		str =  m_vData.m_pwstrData->c_str();
		return true;
	}
	return false;
}
bool XLCellData::GetData(std::wstring& str)
{
	if(m_eDataType == WString)
	{
		str =  *m_vData.m_pwstrData;
		return true;
	}
	return false;
}

void XLCellData::SetDataAndIndentifyType(CString& value)
{
	std::wstring data1(value);

	SetData(data1);
}

void XLCellData::Reset()
{
	switch(Type())
	{
	case Integer:
		{
			int x = 0;
			SetData(x);
		}
		break;
	case Double:
		{
			double x = 0.0;
			SetData(x);
		}
		break;
	case String:
		{
			if(m_vData.m_pstrData)
			{
				delete m_vData.m_pstrData;
				m_vData.m_pstrData = 0;
			}
		}
		break;
	case Char:
		{
			char x = 0;
			SetData(x);
		}		
		break;

	case WString:
		{
			if(m_vData.m_pwstrData)
			{
				delete m_vData.m_pwstrData;
				m_vData.m_pwstrData = 0;
			}
		}		
		break;
	case InvalidDataType:
		break;
	default:
		break;
	}
}

bool XLCellData::ForceCompare(std::wstring& wstr)
{
	//memset(ConvertingPlace,0, sizeof(ConvertingPlace));
	bool isEqual = false;
	switch(Type())
	{
	case Integer:
		{
			std::sprintf(ConvertingPlace, "%d", m_vData.m_iData);
			string str((const char*)ConvertingPlace);
			wstring datastr(str.begin(), str.end());
			if(datastr.compare(wstr) == 0)
			{
				isEqual = true;
			}
		}
		break;
	case Double:
		{
			std::sprintf(ConvertingPlace, "%f", m_vData.m_dData);
			string str((const char*)ConvertingPlace);
			wstring datastr(str.begin(), str.end());
			if(datastr.compare(wstr) == 0)
			{
				isEqual = true;
			}
		}
		break;
	case String:
		{
			wstring datastr(m_vData.m_pstrData->begin(), m_vData.m_pstrData->end());
			if(CSettingsSM::GetInstance()->MatchExact())
			{
				if(datastr.compare(wstr) == 0)
				{
					isEqual = true;
				}
			}
			else
			{
				isEqual = CSettingsSM::GetInstance()->SubStringExistsInFindString(datastr);
			}
		}
		break;
	case Char:
		{
			ConvertingPlace[0] = m_vData.m_cData;
			ConvertingPlace[1] = 0;
			string str((const char*)ConvertingPlace);
			wstring datastr(str.begin(), str.end());
			if(datastr.compare(wstr) == 0)
			{
				isEqual = true;
			}
		}		
		break;

	case WString:
		{
			if(CSettingsSM::GetInstance()->MatchExact())
			{
				if(m_vData.m_pwstrData->compare(wstr) == 0)
				{
					isEqual = true;
				}
			}
			else
			{
				isEqual = CSettingsSM::GetInstance()->SubStringExistsInFindString(*m_vData.m_pwstrData);
			}
		}		
		break;
	case InvalidDataType:
		break;
	default:
		break;
	}

	return isEqual;
}


bool XLCellData::operator == (XLCellData& other)
{
	if(Type() != other.Type())
	{
		return false;
	}
	bool bReturn = false;
	switch(Type())
	{
	case Integer:
		{
			int data1 = 0;
			int data2 = 0;
			GetData(data1);
			other.GetData(data2);
			if(data1 == data2)
			{
				bReturn = true;
			}
		}
		break;
	case Double:
		{
			double data1 = 0;
			double data2 = 0;
			GetData(data1);
			other.GetData(data2);
			if(data1 == data2)
			{
				bReturn = true;
			}
		}
		break;
	case String:
		{
			std::string data1;
			std::string data2;
			GetData(data1);
			other.GetData(data2);
			if(strcmp(data1.c_str(),data2.c_str()) == 0)
			{
				bReturn = true;
			}
		}
		break;
	case Char:
		{
			char data1 = 0;
			char data2 = 0;
			GetData(data1);
			other.GetData(data2);
			if(data1 == data2)
			{
				bReturn = true;
			}
		}		
		break;

	case WString:
		{
			std::wstring data1;
			std::wstring data2;
			GetData(data1);
			other.GetData(data2);
			if(data1.compare(data2) == 0)
			{
				bReturn = true;
			}
		}		
		break;
	case InvalidDataType:
		bReturn = true;
		break;
	default:
		break;
	}
	return bReturn;
}
