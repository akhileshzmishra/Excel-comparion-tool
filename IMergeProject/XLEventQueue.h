#ifndef XLEVENT_Q___H_
#define XLEVENT_Q___H_
#include <queue>
#include "XLMutex.h"

enum XLEventQueueConfig
{
   XLEventQueueConfig_MaxSize = 256
};

class XLThreadEvent
{
public:
	char Data[32];
	int Type;
	class TargetHandler* Handler;
	XLThreadEvent():Type(0)	, Handler(0)
	{
		memset(Data, 0, sizeof(Data));
	}
	XLThreadEvent& operator = (const XLThreadEvent& other)
	{
		if(this == &other)
		{
			return *this;
		}
		memcpy(Data, other.Data, sizeof(Data));
		Type = other.Type;
		Handler = other.Handler;
		return *this;
	}

};

class XLEventQueue
{
	XLMutex                   mMut;
	XLSemaPhore               mSem;
	std::queue<XLThreadEvent> mQ;
	int                       mMaxSize;
	XLThreadEvent             mEvent;
	bool                      mCreated;
public:
	XLEventQueue(int MaxSize = XLEventQueueConfig_MaxSize);
	~XLEventQueue(void);
	bool Create();
	void Destroy();
	bool Put(XLThreadEvent* e);
	bool Get(XLThreadEvent* e);
	bool IsCreated()               { return mCreated;}
};



#endif