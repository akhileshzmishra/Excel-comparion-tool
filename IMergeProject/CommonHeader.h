#ifndef _COMMONHEADER_H1g_
#define _COMMONHEADER_H1g_

#include <stdio.h>
#include "XLCommonHeader.h"
#include "InternalSmartPointer.h"
#include <map>
#include <list>
#include "IDualMap.h"
#include "LinkedList.h"

#define ZERO_TABLE_X1 09
#define ZERO_TABLE_X2 30

#define TABLE_WIDTH 300
#define TABLE_GAP 0
#define LEFT_GAP 30
#define FIRST_TABLE_X 40
#define FIRST_TABLE_Y 20
#define TABLE_HEIGHT 400
#define CELL_HEIGHT 30
#define GAP_DEFAULT 20
#define LOAD_BTN_W 30
#define LOAD_BTN_H	FIRST_TABLE_Y

#define DIFF_POINTER_BUTTON_X  20
#define DIFF_POINTER_BUTTON_Y  20
#define DIFF_POINTER_BUTTON_W  20
#define DIFF_POINTER_BUTTON_H  20

#ifndef BLUECOL
#define BLUECOL RGB(0,0,255)
#endif

#ifndef REDCOL
#define REDCOL RGB(255,0,0)
#endif


#ifndef YELLOWCOL
#define YELLOWCOL RGB(125,125,0)
#endif

#ifndef ALICEBLUE
#define ALICEBLUE RGB(240,248,255)
#endif

#ifndef LEVENDERBLUSHCOL
#define LEVENDERBLUSHCOL RGB(255, 240, 245)
#endif

#ifndef MEDAQUAMARCOL
#define MEDAQUAMARCOL RGB(102, 205, 170)
#endif

#ifndef DARKSLATEGRAYCOL
#define DARKSLATEGRAYCOL RGB(49, 79, 79)
#endif

#ifndef LIGHTGRAYCOLOR
#define LIGHTGRAYCOLOR RGB(149, 149, 149)
#endif

#ifndef DARKREDCOL
#define DARKREDCOL RGB(255, 100, 100)
#endif

#ifndef WHITECOL
#define WHITECOL RGB(255,225,255)
#endif

#ifndef BLACKCOL
#define BLACKCOL RGB(0,0,0)
#endif

#define INVALIDROW	 -1
#define INVALIDCOL   -1

#define MAX_HISTORY_SIZE 200

typedef pair<int, int>    ComparedDataPair;	 
typedef	vector<ComparedDataPair >  CompareListType;

enum XLComparatorOperation
{
	XLComparatorOperation_O1, // Leave all the rows where any row is different
	XLComparatorOperation_O2, // Leave all the rows where identifier key is same and create new rows when they are different


	XLComparatorOperation_Max

};


class RowComparisonComparator
{
public:
	bool operator ()(int a, int b)	const
	{
		return (a < b);
	}
};

class RowComparison
{
public:
	int Row1;
	int Row2;
	
	DualMap<int, int, RowComparisonComparator, RowComparisonComparator>  mColMap;
	friend class ColCompareItr;

	RowComparison(): Row1(0), Row2(0){}
	RowComparison(int r1, int r2): Row1(r1), Row2(r2){}
	void Insert(int c1, int c2);
	bool FindC1(int c2, int& ret);
	bool FindC2(int c1, int& ret);
	int SizeCol() { return mColMap.Size();}
	void Erase(int c1, int c2);
};

typedef DualMapItr<int , int, RowComparisonComparator, RowComparisonComparator> IColCompareItr;

class ColCompareItr: public IColCompareItr
{
public:
	ColCompareItr(RowComparison* rowc, bool forfirst):
	  IColCompareItr(&(rowc->mColMap), forfirst) 
	  {
	  }
};

class RowComparisonList
{
	typedef LListNode<RowComparison>     Node;
	typedef std::map<int, Node*>         RowPos;
	typedef RowPos::iterator             RowItr;
	typedef std::pair<int, Node*>        RowPair; 
	RowPos                               mFirstRowPos;
	RowPos                               mSecondRowPos;
	typedef LinkedList<RowComparison>    RowCompList;
	RowCompList                          mRowList;
	int                                  mFill;
	friend class RowComparisonIterator;
public:
	RowComparisonList();
	~RowComparisonList();
	int size()              {return (int)mRowList.Size();}


	RowComparison* Insert(int r1, int r2);
	RowComparison* AtLastFill();

	void Delete(int r1, int r2);

	RowComparison* FindRow1(int r1);
	RowComparison* FindRow2(int r2);

	void Clear();
private:
	Node* GetRow1Node(int r1);
	Node* GetRow2Node(int r2);

};

class RowComparisonIterator: public LinkedListItr<RowComparison>
{
	RowComparisonList* mList;
public:
	RowComparisonIterator(RowComparisonList* mlist = 0);
	~RowComparisonIterator();
	void SetList(RowComparisonList* mlist)   {mList = mlist;}
	bool IsEnd();
	bool IsBegin();
	bool IsValid();

	RowComparison* GetValue()   
	{
		return Value();
	}

	void DeleteCurrValue();
	bool MoveToRow(int row, bool firstIndex);

	void ResetToFront();
	void ResetToBack();

	bool Next()
	{
		return HasNext();
	}

	bool Previous()
	{
		return HasPrevious();
	}
};


enum XLTableParam
{
	XLTableParamReloadTable,
	XLTableParamFullTable,
	XLTableParamDiffTable,
	XLTableParamSameTable,
	XLTableParamNoTable
};

class XLUtility
{
	CView* m_Parent;
	int    m_WindowDim[2];
	static XLUtility* m_Instance;
	bool   m_bVerticalInterlocked;
private:
	XLUtility();
	XLUtility(const XLUtility&);
public:
	static XLUtility* GetInstance(CView* view = 0);
	~XLUtility();
	void Initialize(CView* view);
	void VerticallyDivideArea(int num, int* partitions, int& width);
	int GetWindowHeight();
	void SetWindowDim(int W, int H)
	{
		m_WindowDim[0] = W;
		m_WindowDim[1] = H;
	}

	void SetVerticalInterlocked(bool set)
	{
		m_bVerticalInterlocked = set;
	}
	int GetTableY();

	bool IsVerticalScrollInterlocked();
	bool IsHorizontalScrollInterlocked();

	int GetDefaultEmptyRows()
	{
		return 10;
	}
};


#define XL_UTIL \
	XLUtility::GetInstance()




#endif
