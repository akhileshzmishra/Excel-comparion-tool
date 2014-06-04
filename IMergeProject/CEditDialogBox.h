#ifndef _CEDITDIALOGBOX_H_
#define _CEDITDIALOGBOX_H_

#include "Afxwin.h"

class CEditDialogBox : public CDialog
{
  CString m_value;
public:  
  CString GetValue() const {return m_value;}
  void SetValue(const CString& value) {m_value = value;}

  virtual BOOL OnInitDialog();
  virtual BOOL DestroyWindow( );



public:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnBnClickedCancel();
public:
	afx_msg void OnEnChangeEdit1();
};






#endif