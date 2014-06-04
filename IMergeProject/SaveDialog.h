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
		return (mbLeft   > 0);
	}
	bool IsRight()
	{
		return (m_bRight   > 0);
	}
	bool IsAll()
	{
		return (m_bAll   > 0);
	}

// Dialog Data
	enum { IDD = IDD_SAVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int mbLeft;
	int m_bRight;
	int m_bAll;
};
