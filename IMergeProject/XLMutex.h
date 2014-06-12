#ifndef XL_MUTEX__H__
#define XL_MUTEX__H__
#include "XLSemaPhore.h"
class XLMutex
{
	XLSemaPhore mSem;
	int         mThreadId;
	int         mCount;
public:
	XLMutex(void);
	bool Create();
	void Destroy();
	void Lock();
	bool Unlock();
	bool TryLock(int msec = 0);
	~XLMutex(void);

	bool IsCreated() { return mSem.IsCreated(); }
};


#endif
