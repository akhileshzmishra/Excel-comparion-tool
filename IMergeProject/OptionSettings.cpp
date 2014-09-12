// OptionSettings.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "OptionSettings.h"
#include "Settings.h"


// COptionSettings dialog

IMPLEMENT_DYNAMIC(COptionSettings, CDialog)

COptionSettings::COptionSettings(CWnd* pParent /*=NULL*/)
	: CDialog(COptionSettings::IDD, pParent)
{

}

COptionSettings::~COptionSettings()
{
}

void COptionSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COptionSettings, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_PAGE_SIZE, &COptionSettings::OnCbnSelchangeComboPageSize)
	ON_CBN_SELCHANGE(IDC_COMBO_TREE_ORDER, &COptionSettings::OnCbnSelchangeComboTreeOrder)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &COptionSettings::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO_OP_TYPE, &COptionSettings::OnCbnSelchangeComboOpType)
	ON_EN_CHANGE(IDC_MATCH_RATIO, &COptionSettings::OnEnChangeMatchRatio)
	ON_BN_CLICKED(IDC_CHECK_AUTO_COMPARE, &COptionSettings::OnBnClickedCheckAutoCompare)
	ON_EN_CHANGE(IDC_MAX_HISTORY, &COptionSettings::OnEnChangeMaxHistory)
END_MESSAGE_MAP()


// COptionSettings message handlers

void COptionSettings::OnCbnSelchangeComboPageSize()
{
	// TODO: Add your control notification handler code here
	CComboBox* pageSize = (CComboBox*)GetDlgItem(IDC_COMBO_PAGE_SIZE);
	if(pageSize)
	{
		int settings = pageSize->GetCurSel();
		int size = 4<<settings;
		SETTINGS_CLASS->SetDefaultPocketSize(size);
	}
}

void COptionSettings::OnCbnSelchangeComboTreeOrder()
{
	// TODO: Add your control notification handler code here
	CComboBox* pageSize = (CComboBox*)GetDlgItem(IDC_COMBO_TREE_ORDER);
	if(pageSize)
	{
		int settings = pageSize->GetCurSel();
		int size = 4<<settings;
		SETTINGS_CLASS->SetDefaultOrder(size);
	}
}

void COptionSettings::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: Add your message handler code here
	GetDefPageValue();

	GetDefOrderValue();

	GetOperationType();

	GetMatchPercentage();

	EnableMatchPercentageBox();

	GetAutoCompare();

	GetMaxHistory();
	
}

void COptionSettings::GetDefPageValue()
{
	CComboBox* pageSize = (CComboBox*)GetDlgItem(IDC_COMBO_PAGE_SIZE);
	if(pageSize)
	{
		int settings = SETTINGS_CLASS->GetDefaultPocketSize();
		int pageSizeIdx = 0;
		int maxNum = 4;
		while(maxNum < settings)
		{
			maxNum = maxNum<<1;
			pageSizeIdx++;
		}
		pageSize->SetCurSel(pageSizeIdx);
	}
}

void COptionSettings::GetDefOrderValue()
{
	CComboBox* pageSize = (CComboBox*)GetDlgItem(IDC_COMBO_TREE_ORDER);
	if(pageSize)
	{
		int settings = SETTINGS_CLASS->GetDefaultOrder();
		int pageSizeIdx = 0;
		int maxNum = 4;
		while(maxNum < settings)
		{
			maxNum = maxNum<<1;
			pageSizeIdx++;
		}
		pageSize->SetCurSel(pageSizeIdx);
	}
}

void COptionSettings::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void COptionSettings::OnCbnSelchangeComboOpType()
{
	// TODO: Add your control notification handler code here
	CComboBox* OpTypeCombo = (CComboBox*)GetDlgItem(IDC_COMBO_OP_TYPE);
	if(OpTypeCombo)
	{
		int selection = OpTypeCombo->GetCurSel();
		if(selection == 0)
		{
			SETTINGS_CLASS->SetOperationType(XLComparatorOperation_O1);
		}
		else
		{
			SETTINGS_CLASS->SetOperationType(XLComparatorOperation_O2);
		}
	}
	EnableMatchPercentageBox();
}

void COptionSettings::GetOperationType()
{
	// TODO: Add your control notification handler code here
	CComboBox* OpTypeCombo = (CComboBox*)GetDlgItem(IDC_COMBO_OP_TYPE);
	if(OpTypeCombo)
	{
		XLComparatorOperation opType = SETTINGS_CLASS->GetOperationType();
		if(opType == XLComparatorOperation_O1)
		{
			OpTypeCombo->SetCurSel(0);
		}
		else
		{
			OpTypeCombo->SetCurSel(1);
		}
	}
}

void COptionSettings::GetMatchPercentage()
{
	CEdit* matchBox = (CEdit*)GetDlgItem(IDC_MATCH_RATIO);
	if(matchBox)
	{
		double d = SETTINGS_CLASS->GetMatchPercentage();
		CString dString;
		dString.Format(L"%g", d);
		matchBox->SetWindowTextW(dString);
	}
}

void COptionSettings::EnableMatchPercentageBox()
{
	CEdit* matchBox = (CEdit*)GetDlgItem(IDC_MATCH_RATIO);
	if(matchBox)
	{
		XLComparatorOperation opType = SETTINGS_CLASS->GetOperationType();
		if(opType == XLComparatorOperation_O1)
		{
			matchBox->EnableWindow(FALSE);
		}
		else
		{
			matchBox->EnableWindow(TRUE);
		}
	}
}

void COptionSettings::OnEnChangeMatchRatio()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	CEdit* matchBox = (CEdit*)GetDlgItem(IDC_MATCH_RATIO);
	if(matchBox)
	{
		CString thestring;
		matchBox->GetWindowTextW(thestring);
		double d = _tstof(thestring);
		SETTINGS_CLASS->SetMatchPercentage(d);
	}
}

void COptionSettings::OnBnClickedRadioAutoCompare()
{
	// TODO: Add your control notification handler code here
}

void COptionSettings::GetAutoCompare()
{
	CButton* autoCompare = (CButton*)GetDlgItem(IDC_CHECK_AUTO_COMPARE);
	if(autoCompare)
	{
		if(SETTINGS_CLASS->GetAutoCompare())
		{
			autoCompare->SetCheck(1);
		}
		else
		{
			autoCompare->SetCheck(0);
		}
	}
}
void COptionSettings::OnBnClickedCheckAutoCompare()
{
	// TODO: Add your control notification handler code here
	CButton* autoCompare = (CButton*)GetDlgItem(IDC_CHECK_AUTO_COMPARE);
	if(autoCompare)
	{
		if(autoCompare->GetCheck() == 1)
		{
			SETTINGS_CLASS->SetAutoCompare(true);
		}
		else
		{
			SETTINGS_CLASS->SetAutoCompare(false);
		}
	}
}

void COptionSettings::OnEnChangeMaxHistory()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CEdit* matchBox = (CEdit*)GetDlgItem(IDC_MAX_HISTORY);
	if(matchBox)
	{
		CString thestring;
		matchBox->GetWindowTextW(thestring);
		int d = _tstoi(thestring);
		SETTINGS_CLASS->SetMaxHistorySize(d);
	}
}

void COptionSettings::GetMaxHistory()
{
	CEdit* matchBox = (CEdit*)GetDlgItem(IDC_MAX_HISTORY);
	if(matchBox)
	{
		CString thestring;
		thestring.Format(L"%d", SETTINGS_CLASS->GetMaxHistorySize());
		matchBox->SetWindowTextW(thestring);
	}
}