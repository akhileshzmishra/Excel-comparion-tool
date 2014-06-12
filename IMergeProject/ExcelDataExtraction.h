#ifndef _EXCELDATAEXTRACTION_H_
#define _EXCELDATAEXTRACTION_H_

#pragma once
#include "XLCellDataContainer.h"
#include "XLCommonHeader.h"

class CExcelDataExtraction
{
	//XLCellDataContainer
public:
	CExcelDataExtraction(void);
public:
	~CExcelDataExtraction(void);
	XLCellDataContainer* GetXLContainer(std::string& filepath, int sheetNum = 0);
	XLCellDataContainer* GetXLContainerExcelFormat(std::string& filepath, int sheetNum = 0);
	bool SaveToExcelFormat(XLCellDataContainer* container, std::string& path, bool isNew = false);
private:
	void SaveStringInCell(const wchar_t* s, XLCellData* data);
	void SaveStringInCell(std::string& s, XLCellData* data);
	void SaveFormulaInCell(const wchar_t* s, XLCellData* data);
	void SaveIntegerInCell(int i, XLCellData* data);
	void SaveDoubleInCell(double d, XLCellData* data);
	void SaveCharInCell(char c, XLCellData* data);

	void FillData(XLCellData* cellData, ExcelFormat::BasicExcelCell* bECell);
	void SaveData(XLCellData* cellData, ExcelFormat::BasicExcelCell* bECell);
};




#endif