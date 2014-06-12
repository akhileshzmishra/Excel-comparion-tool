#ifndef _COMMONHEADER_H1_
#define _COMMONHEADER_H1_

#include <stdio.h>
#include "XLCommonHeader.h"
#include "InternalSmartPointer.h"
#include <map>
#include <list>
#include "IDualMap.h"

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

#ifndef LEVENDERBLUSHCOL
#define LEVENDERBLUSHCOL RGB(255, 240, 245)
#endif

#ifndef MEDAQUAMARCOL
#define MEDAQUAMARCOL RGB(102, 205, 170)
#endif

#ifndef DARKSLATEGRAYCOL
#define DARKSLATEGRAYCOL RGB(49, 79, 79)
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

#define MAX_HISTORY_SIZE 100

typedef pair<int, int>    ComparedDataPair;	 
typedef	vector<ComparedDataPair >  CompareListType;


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
	typedef std::map<int, int> RowPos;
	typedef RowPos::iterator   RowItr;
	typedef std::pair<int, int> RowPair; 
	RowPos                     mFirstRowPos;
	RowPos                     mSecondRowPos;
	typedef std::vector<RowComparison> RowCompList;
	RowCompList                mRowList;
	int                        mFill;
	friend class RowComparisonIterator;
public:
	RowComparisonList();
	~RowComparisonList();
	int size()              {return (int)mRowList.size();}
	void resize(int size);

	RowComparison* Insert(int r1, int r2);
	RowComparison* AtLastFill();

	void Delete(int r1, int r2);

	RowComparison* FindRow1(int r1);
	RowComparison* FindRow2(int r2);

	int FindIndex1(int r1);
	int FindIndex2(int r2);

	void clear();

};

class RowComparisonIterator
{
	int mitr;
	RowComparisonList* mList;
	RowComparison*     mThisValue;
public:
	RowComparisonIterator(RowComparisonList* mlist = 0);
	~RowComparisonIterator();
	void SetList(RowComparisonList* mlist)   {mList = mlist;}
	bool IsEnd();
	bool IsBegin();
	bool IsValid();
	void Reset(bool toLast = false);
	void operator ++();
	void operator ++(int);
	void operator --();
	void operator --(int);
	RowComparison* GetValue()   {return mThisValue;}
	void DeleteCurrValue();
	void MoveToIndex(int placeIdx);

	bool Next()                     {return ((mitr + 1) < mList->mFill);}
	bool Previous()                 {return ((mitr - 1) >= 0);}
};


enum XLTableParam
{
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