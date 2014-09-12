#include "StdAfx.h"
#include "GridViewHelper.h"
#include "GridTableCompareView.h"
#include "XLSimpleThreads.h"
GridViewHelper* GridViewHelper::m_Instance = 0;


GridViewHelper::GridViewHelper(void):
mpDbParam(0)
{
}

GridViewHelper::~GridViewHelper(void)
{
}

GridViewHelper* GridViewHelper::GetInstance()
{
	if(!m_Instance)
	{
		m_Instance = new GridViewHelper();
	}
	return m_Instance;
}

void GridViewHelper::RunProgramAsync(AsyncProgramType type)
{
	mType = type;
	//XLSimpleThreads::RunSimpleThread(this);
	
}

void GridViewHelper::SetGridView(GridTableCompareView* view)
{
	m_Helped = view;
}

void GridViewHelper::RunProgram()
{
	switch(mType)
	{
	case AsyncProgramType_SaveToDB:
		m_RunSaveDB();
		break;

	default:
		break;
	}
	
}

void GridViewHelper::m_RunSaveDB()
{
	if(!mpDbParam || !m_Helped)
		return;

	//m_Helped->mSaveToDataBase(mpDbParam->r, mpDbParam->c, mpDbParam->value);
//	delete mpDbParam;
}


