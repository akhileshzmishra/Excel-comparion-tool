// CurrentDiffViewer.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "CurrentDiffViewer.h"
//#include "IMergeProjectView.h"
#include "GridTableCompareView.h"


// CCurrentDiffViewer dialog

IMPLEMENT_DYNAMIC(CCurrentDiffViewer, CDialog)

CCurrentDiffViewer::CCurrentDiffViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrentDiffViewer::IDD, pParent)
{
	m_pParent = pParent;
}

CCurrentDiffViewer::~CCurrentDiffViewer()
{
}

BOOL CCurrentDiffViewer::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	BOOL retVal = CDialog::Create(nIDTemplate, pParentWnd);

	if(retVal == TRUE)
	{
		if(m_pGrid)
		{
			delete m_pGrid;
			m_pGrid = 0;
		}
		int partitions[3];
		int width;
		XL_UTIL->VerticallyDivideArea(2, partitions, width);

		CRect rect;	
		rect.left = 10;
		rect.top = 10;
		rect.bottom = 100;
		rect.right = width + rect.left;

		int Col = GridTableCompareView::GetInstance()->GetMaxCol();
		m_pGrid = new CGridCtrl(GridTableCompareView::GetInstance(), 2, Col);
		m_pGrid->Create(rect, m_pParentWnd, 3);
		m_pGrid->SetRowCount(2);
		m_pGrid->EnableDragAndDrop(FALSE);
		m_pGrid->SetColumnCount(Col);
	}
	return retVal;
}

void CCurrentDiffViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CCurrentDiffViewer::PostNcDestroy()
{
	CDialog::PostNcDestroy();
	//if(m_pParent)
	{		
		//((CModelessDemoDlg*)m_pParent)->m_pmodeless = NULL;		
	}
	delete this;
}


BEGIN_MESSAGE_MAP(CCurrentDiffViewer, CDialog)
END_MESSAGE_MAP()


// CCurrentDiffViewer message handlers
