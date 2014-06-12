#ifndef XLSIMPLETHREADS_H_
#define XLSIMPLETHREADS_H_

#include "afxwin.h"
#include "CommonHeader.h"
#include "XLSemaPhore.h"
#include "IProjectInterface.h"

enum ThreadConfig
{
	ThreadConfig_StackSize = 8192
};

class IThreadFunc
{
public:
	virtual void RunProgram() = 0;
};



class XLSimpleThreads: public IThreadFunc
{
	XLSemaPhore   m_Sync1;
	XLSemaPhore   m_Sync2;
	int           m_handle;
	std::string   mName;
	int           mID;
	friend DWORD WINAPI RunSimpleThread(LPVOID data);
public:
	XLSimpleThreads(void);
	~XLSimpleThreads(void);
	virtual bool Create(const char *threadname, unsigned long stackSize = ThreadConfig_StackSize);
	void Destroy();
	bool IsCreated()         {return (mID != 0);}
	void RunProgram()        {}
	int ThreadID() const     {return (int)mID;}

};







#endif