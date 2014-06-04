// SaveDialog.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "SaveDialog.h"


// CSaveDialog dialog

IMPLEMENT_DYNAMIC(CSaveDialog, CDialog)

CSaveDialog::CSaveDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSaveDialog::IDD, pParent)
	, mbLeft(0)
	, m_bRight(0)
	, m_bAll(0)
{

}

CSaveDialog::~CSaveDialog()
{
}

void CSaveDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_RADIO_LEFT, mbLeft);
    DDX_Check(pDX, IDC_RADIO_RIGHT, m_bRight);
	DDX_Check(pDX, IDC_RADIO_ALL, m_bAll);
}


BEGIN_MESSAGE_MAP(CSaveDialog, CDialog)
END_MESSAGE_MAP()


// CSaveDialog message handlers
