// FindItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "FindItemDlg.h"
#include "Settings.h"


// CFindItemDlg dialog

IMPLEMENT_DYNAMIC(CFindItemDlg, CDialog)

CFindItemDlg::CFindItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindItemDlg::IDD, pParent)
	, m_KeywordStringValue(_T(""))
	, m_IgnoreCaps(false)
	, m_FindUp(false)
	, m_FindDown(false)
{

}

CFindItemDlg::~CFindItemDlg()
{
}

std::wstring CFindItemDlg::GetKeyword()
{
	std::wstring wstr(m_KeywordStringValue);

	return wstr; 
}

INT_PTR CFindItemDlg::DoModal()
{
	//CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT_KEYWORD);
	//if(e)
	{
		//e->SetSel(0);
	}
	INT_PTR ptr = CDialog::DoModal();
	return ptr;
}

void CFindItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_KeyWord);
	//DDX_Text(pDX, IDC_EDIT_KEYWORD, m_KeywordStringValue);
}


BEGIN_MESSAGE_MAP(CFindItemDlg, CDialog)
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDC_CHECK3, &CFindItemDlg::OnFindUP)
	ON_BN_CLICKED(IDC_CHECK4, &CFindItemDlg::OnFindDown)
	ON_BN_CLICKED(IDC_CHECK1, &CFindItemDlg::OnMatchExact)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	//ON_BN_CLICKED(IDOK, &CFindItemDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_KEYWORD, &CFindItemDlg::OnEnChangeEditKeyword)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CFindItemDlg message handlers

void CFindItemDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	//GetParent()->PostMessage(WM_MODELESS_CLOSED,0,0);
	//delete this;
	// TODO: Add your message handler code here
}

void CFindItemDlg::OnFindUP()
{
	// TODO: Add your control notification handler code here
	CButton* checkbtn = (CButton*)GetDlgItem(IDC_CHECK4);
	if(checkbtn)
	{
		checkbtn->SetCheck(0);
	}
	CSettingsSM::GetInstance()->SetDefaultSearchDirection(DefaultSearchDir_UP);
}

void CFindItemDlg::OnFindDown()
{
	// TODO: Add your control notification handler code here
	CButton* checkbtn = (CButton*)GetDlgItem(IDC_CHECK3);
	if(checkbtn)
	{
		checkbtn->SetCheck(0);
	}
	CSettingsSM::GetInstance()->SetDefaultSearchDirection(DefaultSearchDir_DOWN);
}

void CFindItemDlg::OnMatchExact()
{
	// TODO: Add your control notification handler code here
	CButton* checkbtn = (CButton*)GetDlgItem(IDC_CHECK1);
	if(checkbtn)
	{
		if(checkbtn->GetCheck() == 1)
		{
			CSettingsSM::GetInstance()->SetMatchExact(true);
		}
		else
		{
			CSettingsSM::GetInstance()->SetMatchExact(false);
		}
	}
	
}

void CFindItemDlg::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);
	CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT_KEYWORD);
	if(e)
	{
		e->SetFocus();
		e->SetSel(0,-1);
		e->SetSel(-1);
	}

	// TODO: Add your message handler code here
}

void CFindItemDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if(CSettingsSM::GetInstance()->GetDefaultSearchDirection() == DefaultSearchDir_UP)
	{
		CButton* checkbtn = (CButton*)GetDlgItem(IDC_CHECK3);
		if(checkbtn)
		{
			checkbtn->SetCheck(1);
		}
	}
	else
	{
		CButton* checkbtn = (CButton*)GetDlgItem(IDC_CHECK4);
		if(checkbtn)
		{
			checkbtn->SetCheck(1);
		}
	}
	CButton* checkbtnMatchEx = (CButton*)GetDlgItem(IDC_CHECK1);
	if(checkbtnMatchEx && CSettingsSM::GetInstance()->MatchExact())
	{
		checkbtnMatchEx->SetCheck(1);
	}
	CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT_KEYWORD);
	if(e)
	{
		std::wstring wstr = CSettingsSM::GetInstance()->GetUserString();
		m_KeywordStringValue.SetString(wstr.c_str());
		e->SetWindowTextW(m_KeywordStringValue);
		e->SetFocus();
		e->SetSel(0,-1);
		e->SetSel(-1);
	}
}

void CFindItemDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
	std::wstring wstr(m_KeywordStringValue);
	CSettingsSM::GetInstance()->SetUserString(wstr);
	
}

void CFindItemDlg::OnEnChangeEditKeyword()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT_KEYWORD);
	if(e)
	{
		e->GetWindowTextW(m_KeywordStringValue);
		std::wstring wstr(m_KeywordStringValue);
		CSettingsSM::GetInstance()->SetUserString(wstr);
	}
}

void CFindItemDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	CEdit* e = (CEdit*)GetDlgItem(IDC_EDIT_KEYWORD);
	if(e)
	{
		std::wstring wstr = CSettingsSM::GetInstance()->GetUserString();
		m_KeywordStringValue.SetString(wstr.c_str());
		e->SetWindowTextW(m_KeywordStringValue);
		e->SetFocus();
		e->SetSel(0,-1);
		e->SetSel(-1);
	}
}
