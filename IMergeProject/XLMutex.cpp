#include "StdAfx.h"
#include "XLMutex.h"

XLMutex::XLMutex(void):
mThreadId(0),
mCount(0)
{
}

XLMutex::~XLMutex(void)
{
}

bool XLMutex::Create()
{
	if(!mSem.IsCreated())
	{
		mSem.Create(1);
	}
}
void XLMutex::Destroy()
{
	mSem.Destroy();
	mThreadId = 0;
}
void XLMutex::Lock()
{
	if (mThreadId == (int) ::GetCurrentThreadId())
	{
		mCount++;
		return;
	}
	mSem.Take();
	mThreadId = (int) ::GetCurrentThreadId();
	mCount    = 1;
}
bool XLMutex::Unlock()
{
	if (mThreadId != (int) ::GetCurrentThreadId())
	{
		return false;
	}

	mCount--;

	if (mCount > 0)
	{
		return true;
	}

	mThreadId = 0;

	mSem.Give();

	return true;
}
bool XLMutex::TryLock(int msec)
{
	if (mThreadId == (int)::GetCurrentThreadId())
	{
		mCount++;

		return true;
	}

	if (mSem.Try(msec) == false)
	{
		return false;
	}

	mThreadId = (int) ::GetCurrentThreadId();
	mCount    = 1;

	return true;
}
