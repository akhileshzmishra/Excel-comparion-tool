#ifndef _GridViewHelper_H_
#define _GridViewHelper_H_

#include "XLCommonHeader.h"
#include "Grid/GridCtrl.h"
#include "XLCellDataContainer.h"
#include "Afxwin.h"
#include "IProjectInterface.h"
#include "CommonHeader.h"
#include "DifferencePointer.h"
#include "XLSimpleThreads.h"

enum AsyncProgramType
{
    AsyncProgramType_SaveToDB,


	AsyncProgramType_Max
};
struct SaveDBParam
{
	int r;
	int c;
	CString value;
	SaveDBParam():r(0), c(0)   {}
	SaveDBParam(int r1, int c1, CString& value1): r(r1), c(c1), value(value1) {}
};

class GridViewHelper: public IThreadFunc
{
	static GridViewHelper* m_Instance;
	class GridTableCompareView*  m_Helped;
	AsyncProgramType mType;
	SaveDBParam* mpDbParam;
private:
	GridViewHelper(void);
	GridViewHelper(const GridViewHelper&);
public:
	~GridViewHelper(void);
	static GridViewHelper* GetInstance();

	void RunProgramAsync(AsyncProgramType type);

	void SetDBParam(SaveDBParam* param)
	{
		mpDbParam = param;
	}

	void SetGridView(class GridTableCompareView* view);

private:
	//Functions overloaded from IThreadFunc
	void RunProgram();

	void m_RunSaveDB();

	
};



#endif