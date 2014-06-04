#pragma once
#include "afxwin.h"
#include "Resource.h"
#include "XLSimpleThreads.h"


// CXLEvisMsgBox dialog

class CXLEvisMsgBox : public CDialog , public IThreadFunc
{
	DECLARE_DYNAMIC(CXLEvisMsgBox)

public:
	CXLEvisMsgBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CXLEvisMsgBox();

// Dialog Data
	enum { IDD = IDD_DIALOG3 };

	BOOL CreateDlg(CWnd* parent);


	//IThreadFunc Member
	void RunProgram();

	void SetDesc(std::string& desc);

	void StartWait();

	void Destroy();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual void t_Run();

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_StaticName;
	afx_msg void OnStnClickedMsgEvis();
//	afx_msg void OnStnEnableMsgEvis() const;
};
