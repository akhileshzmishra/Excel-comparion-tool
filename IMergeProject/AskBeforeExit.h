#pragma once


// CAskBeforeExit dialog

class CAskBeforeExit : public CDialog
{
	DECLARE_DYNAMIC(CAskBeforeExit)

public:
	CAskBeforeExit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAskBeforeExit();

// Dialog Data
	enum { IDD = IDD_DIALOG_ASK_EXIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
