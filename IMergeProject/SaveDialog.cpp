// SaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "SaveDialog.h"
#include "Settings.h"


// CSaveDialog dialog

IMPLEMENT_DYNAMIC(CSaveDialog, CDialog)

CSaveDialog::CSaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDialog::IDD, pParent)
	, m_bLeft(SETTINGS_CLASS->GetFileAChanged())
	, m_bRight(SETTINGS_CLASS->GetFileBChanged())
	, m_bAll(m_bLeft && m_bRight)
{

}

CSaveDialog::~CSaveDialog()
{
}

void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Check(pDX, IDC_RADIO_LEFT, mbLeft);
    //DDX_Check(pDX, IDC_RADIO_RIGHT, m_bRight);
	//DDX_Check(pDX, IDC_RADIO_ALL, m_bAll);
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
	ON_BN_CLICKED(IDC_RADIO_ALL, &CSaveDialog::OnBnClickedRadioAll)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDC_RADIO_LEFT, &CSaveDialog::OnBnClickedRadioLeft)
	ON_BN_CLICKED(IDC_RADIO_RIGHT, &CSaveDialog::OnBnClickedRadioRight)
	ON_BN_CLICKED(IDOK, &CSaveDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CSaveDialog message handlers


void CSaveDialog::OnBnClickedRadioAll()
{
	// TODO: Add your control notification handler code here
	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO_ALL);
	if(btn)
	{
		if(btn->GetCheck() == 1)
		{
			m_bAll = true;
		}
		else
		{
			m_bAll = false;
		}
	}
}


void CSaveDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	bool leftChanged = SETTINGS_CLASS->GetFileAChanged();
	bool rightChanged = SETTINGS_CLASS->GetFileBChanged();


	// TODO: Add your message handler code here
	if(leftChanged && rightChanged)
	{
		CButton* btnAll = (CButton*)GetDlgItem(IDC_RADIO_ALL);
		if(btnAll)
		{
			btnAll->SetCheck(1);
		}
	}
	else if(leftChanged)
	{
		CButton* btnLeft = (CButton*)GetDlgItem(IDC_RADIO_LEFT);
		if(btnLeft)
		{
			btnLeft->SetCheck(1);
		}
	}
	else if(rightChanged)
	{
		CButton* btnRight = (CButton*)GetDlgItem(IDC_RADIO_RIGHT);
		if(btnRight)
		{
			btnRight->SetCheck(1);
		}
	}
	
}


void CSaveDialog::OnBnClickedRadioLeft()
{
	// TODO: Add your control notification handler code here
	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO_LEFT);
	if(btn)
	{
		if(btn->GetCheck() == 1)
		{
			m_bLeft = true;
		}
		else
		{
			m_bLeft = false;
		}
	}
}


void CSaveDialog::OnBnClickedRadioRight()
{
	// TODO: Add your control notification handler code here
	CButton* btn = (CButton*)GetDlgItem(IDC_RADIO_LEFT);
	if(btn)
	{
		if(btn->GetCheck() == 1)
		{
			m_bRight = true;
		}
		else
		{
			m_bRight = false;
		}
	}
}


void CSaveDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}
