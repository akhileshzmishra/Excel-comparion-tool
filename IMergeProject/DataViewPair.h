#ifndef _DATAVIEWPAIR_H_
#define _DATAVIEWPAIR_H_


#pragma once
#include "Grid/GridCtrl.h"
#include "XLCommonHeader.h"
#include "CommonHeader.h"
#include "XLCellDataContainer.h"



class DataViewPair
{
	CGridCtrl*             m_pGrid;
	std::string            m_strPath;
	bool                   m_bLoad;
	XLCellDataContainer*   m_Container;
public:
	DataViewPair(void);
	~DataViewPair(void);
	void SetPath(std::string str);
	void Load();
	CGridCtrl* CreateGrid(RECT rect, CWnd* parent, UINT uint);
	XLCellDataContainer* Container();
	void ResetContainer();
	CGridCtrl* Grid();
	bool IsReady();
	void CreateEmptyContainer(int r, int c);
private:
	void DestroyXLContainer();
	void DestroyGrid();
};





#endif