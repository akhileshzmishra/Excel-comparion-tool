// DeepikaProjectDoc.cpp : implementation of the IMergeProjectDoc class
//

#include "stdafx.h"
#include "IMergeProject.h"

#include "IMergeProjectDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// IMergeProjectDoc

IMPLEMENT_DYNCREATE(IMergeProjectDoc, CDocument)

BEGIN_MESSAGE_MAP(IMergeProjectDoc, CDocument)
END_MESSAGE_MAP()


// IMergeProjectDoc construction/destruction

IMergeProjectDoc::IMergeProjectDoc()
{
	// TODO: add one-time construction code here

}

IMergeProjectDoc::~IMergeProjectDoc()
{
}

BOOL IMergeProjectDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// IMergeProjectDoc serialization

void IMergeProjectDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// IMergeProjectDoc diagnostics

#ifdef _DEBUG
void IMergeProjectDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void IMergeProjectDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// IMergeProjectDoc commands
