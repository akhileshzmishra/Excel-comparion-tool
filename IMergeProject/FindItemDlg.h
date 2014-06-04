#pragma once
#include "afxwin.h"
#include "IProjectInterface.h"


// CFindItemDlg dialog

class CFindItemDlg : public CDialog
{
	DECLARE_DYNAMIC(CFindItemDlg)

public:
	CFindItemDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFindItemDlg();

// Dialog Data
	enum { IDD = IDD_FIND_DLG };

	std::wstring GetKeyword();
	virtual INT_PTR DoModal();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_KeyWord;
	afx_msg void OnNcDestroy();
	CString m_KeywordStringValue;
	bool m_IgnoreCaps;
	bool m_FindUp;
	bool m_FindDown;
	afx_msg void OnFindUP();
	afx_msg void OnFindDown();
	afx_msg void OnMatchExact();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditKeyword();
};
