#ifndef _XLCELLDATA_H_
#define _XLCELLDATA_H_

#pragma once
#include "XLCommonHeader.h"



class XLCellData
{
	XLCellDataType          m_eDataType;
	union Data
	{
		std::string*        m_pstrData;
		std::wstring*       m_pwstrData;
		double              m_dData;
		int                 m_iData;
		char                m_cData;
	};
	Data                    m_vData;
	bool                    m_bFormula;
public:
	XLCellData(void);
	XLCellData(const XLCellData& other);
	XLCellData& operator = (const XLCellData& other);
	bool operator == (XLCellData& other);
	void SetData(int i);
	void SetData(double d);
	void SetData(char c);
	void SetData(char* str);
	void SetData(const char* str);
	void SetData(std::string& str);
	void SetData(wchar_t* str);
	void SetData(const wchar_t* str);
	void SetData(std::wstring& str);
	void SetFormula(std::string& str);

	void SetDataAndIndentifyType(CString& value);

	bool GetData(int& i);
	bool GetData(double& d);
	bool GetData(char& c);
	bool GetData(char*& str);
	bool GetData(std::string& str);
	bool GetData(const wchar_t*& str);
	bool GetData(std::wstring& str);
	bool GetFormula(std::string& str);

	bool ForceCompare(std::wstring& wstr);

	bool IsEmpty()
	{
		return (m_eDataType == InvalidDataType);
	}

	XLCellDataType Type()
	{
		return m_eDataType;
	}
	bool IsFormula()
	{
		return m_bFormula;
	}
	void Reset();
public:
	~XLCellData(void);
};



#endif