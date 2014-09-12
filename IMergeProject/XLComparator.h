#ifndef _XLCOMPARATOR_H_
#define _XLCOMPARATOR_H_

#pragma once
#include "XLCommonHeader.h"
#include "ExcelDataExtraction.h"
#include "XLLCS.h"
#include "CommonHeader.h"
#include "IndexTree.h"


class XLMalleablePairList
{
	typedef std::pair<int, int>                               _IndexPair;
	typedef IndexTreeSpace::CIndexTree<_IndexPair >           _MappedIndex;
	_MappedIndex                                              m_IndexMap;
	_IndexPair                                                m_DummyPair;
public:
	XLMalleablePairList(int initialVal = 0) :
		m_IndexMap(16, 16)
	{
		AddSpace(initialVal);
	}

	void AddSpace(int items)
	{
		for (int i = 0; i < items; i++)
		{
			m_IndexMap.Pushback(_IndexPair(0, 0));
		}
	}

	void AddSpace(int items, _IndexPair p)
	{
		for (int i = 0; i < items; i++)
		{
			m_IndexMap.Pushback(p);
		}
	}

	void ReInitialize(int items)
	{
		m_IndexMap.Clear();
		AddSpace(items);
	}

	void ReInitialize(int items, _IndexPair p)
	{
		m_IndexMap.Clear();
		AddSpace(items, p);
	}

	void Clear()
	{
		m_IndexMap.Clear();
	}

	_IndexPair& operator [] (int index)
	{
		_IndexPair* p;
		if(m_IndexMap.Value(index, p))
		{
			return *p;
		}
		return m_DummyPair;
	}

	bool Insert(int index, _IndexPair p)
	{
		return m_IndexMap.Insert(index, p);
	}

	bool Pushback(_IndexPair p)
	{
		return m_IndexMap.Pushback(p);
	}
	
	bool Delete(int index)
	{
		return m_IndexMap.Delete(index);
	}

	int size()
	{
		return m_IndexMap.Size();
	}

};


class XCellDataComparator
{
public:
	bool operator() (XLCDRow& A, XLCDRow& B)
	{
		return CompareRows(A, B);
	}
private:
	bool CompareCells(XLCellData& one, XLCellData& two);
	bool CompareRows(XLCDRow& A, XLCDRow& B);
};

class XCellDataMaxComparator
{
public:
	bool operator() (XLCDRow& A, XLCDRow& B)
	{
		return CompareRows(A, B);
	}
private:
	bool CompareRows(XLCDRow& A, XLCDRow& B);
};

class XLCellDataProvider
{
	XLCellDataContainer* m_pContainer;
	int m_iR1;
	int m_iR2;
public:
	XLCellDataProvider(XLCellDataContainer* container):
	m_pContainer(container),
	m_iR1(0),
	m_iR2(m_pContainer? m_pContainer->Row() - 1: 0)
	{
	}

	void SetContainer(XLCellDataContainer* container)
	{
		m_pContainer = container;
	}

	void SetRowLimit(int r1, int r2)
	{
		m_iR1 = r1;
		m_iR2 = r2;
	}

	int Size()
	{
		return (m_iR2 - m_iR1 + 1);
	}

	int Span()
	{
		return (m_iR2 - m_iR1 + 1);
	}

	XLCDRow& operator [](int i)
	{
		return *(m_pContainer->CellRow(i + m_iR1));
	}
};


typedef XLLCS<XCellDataComparator, XLCellDataProvider>       XLDataCompareClass;
typedef XLLCS<XCellDataMaxComparator, XLCellDataProvider>    XLDataMaxCompareClass;
typedef XLMalleablePairList                                  XLInsertableLinkedList;






class XLComparator
{
	RETURNTYPE                         m_UnChangedRows;
	RETURNTYPE                         m_ChangedRows;
	std::vector<int>                   m_OnlyInA;
	std::vector<int>                   m_OnlyInB;
	int                                m_R1;
	int                                m_R2;
	std::vector<int>                   m_vUniqueKeys; //identifier key set
	XLComparatorOperation              m_iOperationType;
	XLCellDataContainer*               m_A;
	XLCellDataContainer*               m_B;
public:
	XLComparator(std::vector<int>& keys);
	XLComparator();
	~XLComparator(void);
	bool Compare(XLCellDataContainer* A, XLCellDataContainer* B);
	RETURNTYPE& UnChangedRows()   { return	m_UnChangedRows; }
	RETURNTYPE& ChangedRows()	  { return	m_ChangedRows; }
	void Reset();
	void SetOperationType(XLComparatorOperation op)
	{
		m_iOperationType = op;
	}
	void SetUniqueRows(std::vector<int>& ColNum);
private:
	void m_FindChangedRows(XLCellDataContainer* A, XLCellDataContainer* B);
	void m_RectifyContainers(XLCellDataContainer* A, XLCellDataContainer* B);

	bool m_CompareUniqueKeys(XLCDRow* r1, XLCDRow* r2);

	bool m_CompareBasic();

};



class XLRowElementDataProvider
{
	XLCDRow* m_pContainer;
public:
	  XLRowElementDataProvider(XLCDRow* container):
	  m_pContainer(container)
	  {
	  }
	  void SetContainer(XLCDRow* container)
	  {
		  m_pContainer = container;
	  }
	  XLCellData& operator [](int i)
	  {
		  return ((*m_pContainer)[i]);
	  }
};


class XLRowDataComparator
{
public:
	bool operator() (XLCellData& A, XLCellData& B)
	{
		return CompareCells(A, B);
	}
private:
	bool CompareCells(XLCellData& one, XLCellData& two);
};

class XCellExhaustiveDataComparator
{
public:
	bool operator() (XLCDRow& A, XLCDRow& B)
	{
		return CompareRows(A, B);
	}
private:
	bool CompareRows(XLCDRow& A, XLCDRow& B);
};


typedef XLLCS<XLRowDataComparator, XLRowElementDataProvider>           XLRowCompareClass;
typedef XLLCS<XCellExhaustiveDataComparator, XLCellDataProvider>       XLExhaustiveCompareClass;





inline bool XCellDataComparator::CompareCells(XLCellData& one, XLCellData& two)
{
	return (one == two);
}


inline bool XLRowDataComparator::CompareCells(XLCellData& one, XLCellData& two)
{
	return (one == two);
}








#endif
