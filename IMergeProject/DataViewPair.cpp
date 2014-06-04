#include "StdAfx.h"
#include "DataViewPair.h"
#include "ExcelDataExtraction.h"


DataViewPair::DataViewPair(void):
m_pGrid(0),
m_bLoad(false),
m_Container(0)
{
}


DataViewPair::~DataViewPair(void)
{
	DestroyGrid();
	DestroyXLContainer();
}



void DataViewPair::SetPath(std::string str)
{
	m_strPath = str;
}
void DataViewPair::Load()
{
	DestroyXLContainer();
	CExcelDataExtraction e;
	m_Container = e.GetXLContainerExcelFormat(m_strPath);
	if(m_Container)
	{
		m_bLoad = true;
	}
}
CGridCtrl* DataViewPair::CreateGrid(RECT rect, CWnd* parent, UINT uint)
{
	if(m_bLoad && m_Container)
	{
		m_pGrid = new CGridCtrl(m_Container->Row(), m_Container->Col());
	}
	m_pGrid->Create(rect, parent, uint);
	return m_pGrid;
}

void DataViewPair::DestroyXLContainer()
{
	if(m_Container)
	{
		delete m_Container;
		m_Container = 0;
		m_bLoad = false;
	}
}

void DataViewPair::DestroyGrid()
{
	if(m_pGrid)
	{
		delete m_pGrid;
		m_pGrid = 0;
	}
}
XLCellDataContainer* DataViewPair::Container()
{
	return m_Container;
}
void DataViewPair::ResetContainer()
{
	delete m_Container;
	m_Container = 0;
}
CGridCtrl* DataViewPair::Grid()
{
	return m_pGrid;
}
bool DataViewPair::IsReady()
{
	return m_bLoad;
}
void DataViewPair::CreateEmptyContainer(int r, int c)
{
	m_Container = new XLCellDataContainer(r, c);
}
