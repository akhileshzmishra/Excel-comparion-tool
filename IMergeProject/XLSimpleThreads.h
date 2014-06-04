#ifndef XLSIMPLETHREADS_H_
#define XLSIMPLETHREADS_H_

#include "afxwin.h"
#include "CommonHeader.h"
#include "IProjectInterface.h"

class IThreadFunc
{
public:
	virtual void RunProgram() = 0;
};

class XLSimpleThreads
{
public:
	XLSimpleThreads(void);
	~XLSimpleThreads(void);
	static void RunSimpleThread(IThreadFunc* Thread);

private:
	static UINT mBeginThread(LPVOID param);
};





#endif