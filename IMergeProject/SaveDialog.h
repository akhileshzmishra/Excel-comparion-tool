#pragma once


// CSaveDialog dialog

class CSaveDialog : public CDialog
{
	DECLARE_DYNAMIC(CSaveDialog)

public:
	CSaveDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveDialog();
	bool IsLeft()
	{
		return (m_bLeft);
	}
	bool IsRight()
	{
		return (m_bRight);
	}
	bool IsAll()
	{
		return (m_bAll);
	}

// Dialog Data
	enum { IDD = IDD_SAVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	bool m_bLeft;
	bool m_bRight;
	bool m_bAll;
	afx_msg void OnBnClickedRadioAll();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedRadioLeft();
	afx_msg void OnBnClickedRadioRight();
	afx_msg void OnBnClickedOk();
};
