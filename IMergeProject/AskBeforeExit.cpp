// AskBeforeExit.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "AskBeforeExit.h"


// CAskBeforeExit dialog

IMPLEMENT_DYNAMIC(CAskBeforeExit, CDialog)

CAskBeforeExit::CAskBeforeExit(CWnd* pParent /*=NULL*/)
	: CDialog(CAskBeforeExit::IDD, pParent)
{

}

CAskBeforeExit::~CAskBeforeExit()
{
}

void CAskBeforeExit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAskBeforeExit, CDialog)
END_MESSAGE_MAP()


// CAskBeforeExit message handlers
