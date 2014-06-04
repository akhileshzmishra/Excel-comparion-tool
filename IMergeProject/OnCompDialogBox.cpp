#include "StdAfx.h"
#include "OnCompDialogBox.h"
#include "GridTableCompareView.h"

OnCompDialogBox::OnCompDialogBox(CWnd* pParent):
CXLEvisMsgBox(pParent)
{
	//Create(pParent);
}

OnCompDialogBox::~OnCompDialogBox(void)
{
}

void OnCompDialogBox::t_Run()
{
	std::string loading("Loading Files and comparing. Please Wait");
	SetDesc(loading);
	//GridTableCompareView::GetInstance()->Clear();
	GridTableCompareView::GetInstance()->Load();
	GridTableCompareView::GetInstance()->CreateTable();
	GridTableCompareView::GetInstance()->Compare();
	//this->
	
}
