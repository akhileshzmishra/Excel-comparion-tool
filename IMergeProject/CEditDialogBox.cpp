#include "stdafx.h"
#include "Resource.h"
#include "CEditDialogBox.h"

BEGIN_MESSAGE_MAP(CEditDialogBox, CDialog)
	ON_BN_CLICKED(IDOK, &CEditDialogBox::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CEditDialogBox::OnBnClickedCancel)
//	ON_EN_CHANGE(IDC_EDITBOX_EBR, &CEditDialogBox::OnEnChangeEdit1)
END_MESSAGE_MAP()

BOOL CEditDialogBox::OnInitDialog()
{
  CDialog::OnInitDialog();

  SetDlgItemText(IDC_EDITBOX_EBR, m_value);

  return TRUE;
}

BOOL CEditDialogBox::DestroyWindow()
{
  GetDlgItemText(IDC_EDITBOX_EBR, m_value);

  return CDialog::DestroyWindow();
}

void CEditDialogBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CEditDialogBox::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CEditDialogBox::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
