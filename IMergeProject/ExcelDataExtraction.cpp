#include "StdAfx.h"
#include "BasicExcel.hpp"
#include "ExcelDataExtraction.h"
#include "XLCommonHeader.h"
#include "ExcelFormat.h"

using namespace ExcelFormat;

CExcelDataExtraction::CExcelDataExtraction(void)
{
}

CExcelDataExtraction::~CExcelDataExtraction(void)
{
}

XLCellDataContainer* CExcelDataExtraction::GetXLContainer(std::string& filepath, int sheetNum)
{
	return 0;
}

void CExcelDataExtraction::SaveStringInCell(const wchar_t* w, XLCellData* data)
{
	wstring str(w);
	string s(str.begin(), str.end());
	data->SetData(s);
}

void CExcelDataExtraction::SaveStringInCell(std::string& s, XLCellData* data)
{
	data->SetData(s);
}

void CExcelDataExtraction::SaveFormulaInCell(const wchar_t* w, XLCellData* data)
{
	wstring str(w);
	string s(str.begin(), str.end());
	data->SetFormula(s);
}

void CExcelDataExtraction::SaveIntegerInCell(int i, XLCellData* data)
{
	data->SetData(i);
}
void CExcelDataExtraction::SaveDoubleInCell(double d, XLCellData* data)
{
	data->SetData(d);
}
void CExcelDataExtraction::SaveCharInCell(char c, XLCellData* data)
{
	data->SetData(c);
}


XLCellDataContainer* CExcelDataExtraction::GetXLContainerExcelFormat
(std::string& filepath,  int sheetNum)
{
	BasicExcel XLLoader;
	XLLoader.New(1);
	if(XLLoader.Load(filepath.c_str()))
	{
		BasicExcelWorksheet* workSheet = XLLoader.GetWorksheet(sheetNum);
		if(!workSheet)
		{
			return 0;
		}
		int totalRows = workSheet->GetTotalRows();
		int totalCols = workSheet->GetTotalCols();
		XLCellDataContainer* container = new XLCellDataContainer(totalRows, totalCols);
		BasicExcelCell* cell = 0;
		for(int i = 0; i < totalRows; i++)
		{
			for(int j = 0; j < totalRows; j++)
			{
				cell = workSheet->Cell(i, j);
				XLCellData* dataCell = container->CellData(i, j);
				FillData(dataCell, cell);
			}
		}
		return container;
	}
	return 0;

}

void CExcelDataExtraction::FillData(XLCellData* cellData, ExcelFormat::BasicExcelCell* bECell)
{
	if(!bECell || !cellData)
		return;
	switch(bECell->Type())
	{
	case BasicExcelCell::INT:
		cellData->SetData(bECell->GetInteger());
		break;
	case BasicExcelCell::DOUBLE:
		cellData->SetData(bECell->GetDouble());
		break;
	case BasicExcelCell::STRING:
		cellData->SetData(bECell->GetString());
		break;
	case BasicExcelCell::WSTRING:
		cellData->SetData(bECell->GetWString());
		break;
	case BasicExcelCell::FORMULA:
		{
			//Worksheet::CellTable::RowBlock::CellBlock* pCell = 0;
			//bECell->get_formula(pCell);
		}
		break;
	case BasicExcelCell::UNDEFINED:
	default:
		break;
	}
}
void CExcelDataExtraction::SaveData(XLCellData* cellData, ExcelFormat::BasicExcelCell* bECell)
{
	if(!bECell || !cellData)
		return;
	switch(cellData->Type())
	{
	case Integer:
		{
			int data;
			cellData->GetData(data);
			bECell->SetInteger(data);
		}
		break;
	case Double:
		{
			double data;
			cellData->GetData(data);
			bECell->SetDouble(data);
		}
		break;
	case Char:
		{
			char data = 0;
			char strdata[2] = {data, 0};
			cellData->GetData(data);
			bECell->SetString(strdata);
		}
		break;
	case String:
		{
			std::string data;
			cellData->GetData(data);
			bECell->SetString(data.c_str());
		}
		break;
	case WString:
		{
			std::wstring data;
			cellData->GetData(data);
			bECell->SetWString(data.c_str());
		}
		break;
	default:
		break;
	}
}

bool CExcelDataExtraction::SaveToExcelFormat(XLCellDataContainer* container, std::string& path, bool isNew)
{
	if(!container)
		return false;
	
	try
	{
		BasicExcel XLLoader;
		bool loaded = false;
		XLLoader.New(1);
		if(!isNew)
		{
			if(XLLoader.Load(path.c_str()))
			{
				loaded = true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			XLLoader.New(1);
		}
		//BasicExcel xls;
		//xls.New(1);
		//BasicExcelWorksheet* sheet = xls.GetWorksheet(0);

		BasicExcelWorksheet* sheet = XLLoader.GetWorksheet(0);
		if(!sheet)
		{
			return false;
		}
		int r = container->Row();
		int c = container->Col();
		BasicExcelCell* cell = 0;

		int ritr = 0;
		int sheetRow = 0;
		while(ritr < r)
		{
			int empty = 0;
			for(int j = 0; j < c; j++)
			{
				cell = sheet->Cell(sheetRow, j);
				XLCellData* cellData = container->CellData(ritr, j);
				SaveData(cellData, cell);
				if(cellData)
				{
					if(cellData->IsEmpty())
					{
						empty++;
					}
				}
			}
			if(empty < c)
			{
				sheetRow++;
			}
			ritr++;
		}
		if(!XLLoader.SaveAs(path.c_str()))
		{
			return false;
		}
	}
	catch(...)
	{
	}
	return true;
}