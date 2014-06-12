#include "StdAfx.h"
#include "XLEventQueue.h"

XLEventQueue::XLEventQueue(int MaxSize):
mMaxSize(MaxSize),
mCreated(false)
{
}

XLEventQueue::~XLEventQueue(void)
{
}

bool XLEventQueue::Create()
{
	if(mCreated)
	{
		return false;
	}
	mMut.Create();
	if(!mMut.IsCreated())
	{
		return false;
	}
	mSem.Create(1);
	if(!mSem.IsCreated())
	{
		mMut.Destroy();
	}

	mCreated = true;
	return true;
}


void XLEventQueue::Destroy()
{
	if(mCreated)
	{
		mMut.Destroy();
		mSem.Destroy();
		mCreated = false;
	}
}

bool XLEventQueue::Put(XLThreadEvent* e)
{ 
	if(!IsCreated())
	{
		return false;
	}
	mMut.Lock();

	mEvent = *e;
	if(mQ.size() >= mMaxSize)
	{
		mMut.Unlock();
		return false;
	}
	mQ.push(mEvent); 	

	if(mQ.size() == 1)
	{
		mMut.Unlock();
		mSem.Give();
	}
	else
	{
		mMut.Unlock();
	}

	return true;

}

bool XLEventQueue::Get(XLThreadEvent* e)
{
	if(!IsCreated())
	{
		return false;
	}
	mMut.Lock();

	if(mQ.size() == 0)
	{
		mMut.Unlock();
		mSem.Take();
		return true;
	}
	*e = mQ.front();
	mQ.pop();

	mMut.Unlock();
	return true;
}
