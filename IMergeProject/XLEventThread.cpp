#include "StdAfx.h"
#include "XLEventThread.h"


TargetHandler::TargetHandler(XLEventThread* Task):
mTask(Task)
{
}

XLEventThread::XLEventThread(void):
TargetHandler(this),
mCreated(false),
mExit(false)
{
}

XLEventThread::~XLEventThread(void)
{
}

void XLEventThread::AddTarget(TargetHandler* handler)
{
	if(!mCreated)
	{
		return;
	}
	mTargetMutex.Lock();
	mEventHandlers.insert(handler);
	mTargetMutex.Unlock();

}
void XLEventThread::RemoveTarget(TargetHandler* handler)
{
	if(!mCreated)
	{
		return;
	}
	mTargetMutex.Lock();
	mEventHandlers.erase(handler);
	mTargetMutex.Unlock();
}
bool XLEventThread::Create(const char* name)
{
	if(!mCreated)
	{
		if(!XLSimpleThreads::Create(name))
		{
			return false;
		}
		if(!XLEventQueue::Create())
		{
			XLSimpleThreads::Destroy();
			return false;
		}
		if(!mTargetMutex.Create())
		{
			XLSimpleThreads::Destroy();
			XLEventQueue::Destroy();
		}
		mCreated = true;
		AddTarget(this);
		return true;
	}
	return false;
}
void XLEventThread::Destroy()
{
	if(mCreated)
	{
		XLSimpleThreads::Destroy();
		XLEventQueue::Destroy();
		mTargetMutex.Destroy();
		RemoveTarget(this);
		mCreated = false;
	}
}
void XLEventThread::RunProgram()
{
	if(!mCreated)
	{
		return;
	}
	while(!mExit)
	{
		if(!Get(&mEvent))
		{
			Destroy();
			return;
		}
		mTargetMutex.Lock();
		std::set<TargetHandler* >::iterator itr = mEventHandlers.find(mEvent.Handler);	

		if(itr != mEventHandlers.end())
		{
			mTargetMutex.Unlock();
			(*itr)->OnEvent(&mEvent);
		}
		else
		{
			mTargetMutex.Unlock();
		}

		
	}
	if(!mExit)
	{
		Destroy();
	}	

}


void XLEventThread::OnEvent(const XLThreadEvent* e)
{
	if(e->Type == XLEventThreadEventType_Exit)
	{
		mExit = true; 
	}
}
