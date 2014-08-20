#pragma once


// COptionSettings dialog

class COptionSettings : public CDialog
{
	DECLARE_DYNAMIC(COptionSettings)

public:
	COptionSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionSettings();

// Dialog Data
	enum { IDD = IDD_OPTION_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboPageSize();
	afx_msg void OnCbnSelchangeComboTreeOrder();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
private:
	void GetDefPageValue();
	void GetDefOrderValue();
	void GetOperationType();
	void GetMatchPercentage();
	void EnableMatchPercentageBox();
	void GetAutoCompare();
public:
	afx_msg void OnCbnSelchangeComboOpType();
	afx_msg void OnEnChangeMatchRatio();
	afx_msg void OnBnClickedRadioAutoCompare();
	afx_msg void OnBnClickedCheckAutoCompare();
};
