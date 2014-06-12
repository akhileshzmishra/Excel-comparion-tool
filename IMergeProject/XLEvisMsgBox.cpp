// XLEvisMsgBox.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h  "
#include "XLEvisMsgBox.h"
#include "GridTableCompareView.h"


// CXLEvisMsgBox dialog

IMPLEMENT_DYNAMIC(CXLEvisMsgBox, CDialog)

CXLEvisMsgBox::CXLEvisMsgBox(CWnd* pParent /*=NULL*/)
	: CDialog(CXLEvisMsgBox::IDD, pParent)
{

}

CXLEvisMsgBox::~CXLEvisMsgBox()
{
}

BOOL CXLEvisMsgBox::CreateDlg(CWnd* parent)
{
	BOOL bRet = FALSE;
	try
	{
		bRet = CDialog::Create(IDD_DIALOG3, parent);
		if(bRet == TRUE)
		{
			SetWindowPos(&this->wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
		}
	}
	catch(...)
	{
	}

	return bRet;
}

void CXLEvisMsgBox::Destroy()
{
	CDialog::ShowWindow(SW_HIDE);
	//CDialog::DestroyWindow();
}

void CXLEvisMsgBox::t_Run()
{
	std::string loading("Loading Files and comparing. Please Wait");
	SetDesc(loading);
	//GridTableCompareView::GetInstance()->Clear();
	GridTableCompareView::GetInstance()->Load();
	GridTableCompareView::GetInstance()->CreateTable();
	GridTableCompareView::GetInstance()->Compare();
}

void CXLEvisMsgBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSG_EVIS, m_StaticName);
}


BEGIN_MESSAGE_MAP(CXLEvisMsgBox, CDialog)
	ON_STN_CLICKED(IDC_MSG_EVIS, &CXLEvisMsgBox::OnStnClickedMsgEvis)
//	ON_STN_ENABLE(IDC_MSG_EVIS, &CXLEvisMsgBox::OnStnEnableMsgEvis)
END_MESSAGE_MAP()


// CXLEvisMsgBox message handlers


//IThreadFunc Member
void CXLEvisMsgBox::RunProgram()
{
	t_Run();
	Destroy();
}


void CXLEvisMsgBox::StartWait()
{
	//XLSimpleThreads::RunSimpleThread(this);
}


void CXLEvisMsgBox::SetDesc(std::string& desc)
{
	CString cstring(desc.c_str());
	m_StaticName.SetWindowTextW((LPCTSTR)cstring);
}
void CXLEvisMsgBox::OnStnClickedMsgEvis()
{
	// TODO: Add your control notification handler code here
}

//void CXLEvisMsgBox::OnStnEnableMsgEvis() const
//{
//	// TODO: Add your control notification handler code here
//}
