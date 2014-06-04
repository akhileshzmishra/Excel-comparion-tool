// IMergeProjectDoc.h : interface of the IMergeProjectDoc class
//


#pragma once


class IMergeProjectDoc : public CDocument
{
protected: // create from serialization only
	IMergeProjectDoc();
	DECLARE_DYNCREATE(IMergeProjectDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~IMergeProjectDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


