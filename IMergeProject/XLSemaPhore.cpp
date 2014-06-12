#include "StdAfx.h"
#include "XLSemaPhore.h"

XLSemaPhore::XLSemaPhore():
mID(0)
{ 	
}

bool XLSemaPhore::Create(int count)
{
	mID = ::CreateSemaphore(NULL, (long)count, 0x7fffffff, NULL);

	return IsCreated();
}

void XLSemaPhore::Destroy()
{
	if(IsCreated())
	{
		::CloseHandle(mID);
		mID = 0;
	}
}

XLSemaPhore::~XLSemaPhore(void)
{
	
}

void XLSemaPhore::Take(void)
{
	if(IsCreated())
	{
		::WaitForSingleObject(mID, INFINITE);
	}
}
bool XLSemaPhore::Try(unsigned long msec)
{
	if (msec == (unsigned long)0)
	{
		Take();

		return true;
	}
	if(IsCreated())
	{
		return ::WaitForSingleObject(mID, msec) == WAIT_OBJECT_0;
	}
	return false;
}
void XLSemaPhore::Give(void)
{
	if(IsCreated())
	{
		::ReleaseSemaphore(mID, 1, NULL);
	}
}

