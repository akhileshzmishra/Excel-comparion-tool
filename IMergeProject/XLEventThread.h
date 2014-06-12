#ifndef XLEVENT_THREAD_H___
#define XLEVENT_THREAD_H___
#include "XLEventQueue.h"
#include "XLSimpleThreads.h"
#include "XLMutex.h"
#include <set>

class TargetHandler
{
	class XLEventThread* mTask;
public:
	TargetHandler(XLEventThread* Task);
	virtual void OnEvent(const XLThreadEvent* e) {}
};

class XLEventThread: public XLEventQueue, public XLSimpleThreads, public TargetHandler
{
	XLMutex                   mTargetMutex;
	std::set<TargetHandler* > mEventHandlers; 
	bool                      mCreated;
	XLThreadEvent             mEvent;
	bool                      mExit;
public:
	enum XLEventThreadEventType
	{
	   XLEventThreadEventType_User,
	   XLEventThreadEventType_Ping,
	   XLEventThreadEventType_Exit
	};
public:
	XLEventThread(void);
	~XLEventThread(void);
	void AddTarget(TargetHandler* handler);
	void RemoveTarget(TargetHandler* handler);
	bool Create(const char* name);
	void Destroy();
	bool IsCreated()         { return mCreated;}
	void RunProgram();
private:
	void OnEvent(const XLThreadEvent* e);
};



#endif