#include "StdAfx.h"
#include "XLSimpleThreads.h"

XLSimpleThreads::XLSimpleThreads(void)
{
}

XLSimpleThreads::~XLSimpleThreads(void)
{
}

void XLSimpleThreads::RunSimpleThread(IThreadFunc* Thread)
{
	AfxBeginThread(XLSimpleThreads::mBeginThread, Thread);
}
UINT XLSimpleThreads::mBeginThread(LPVOID param)
{
	IThreadFunc* Thread = (IThreadFunc*) param;
	if(Thread)
	{
		Thread->RunProgram();
	}
	return 0;
}
