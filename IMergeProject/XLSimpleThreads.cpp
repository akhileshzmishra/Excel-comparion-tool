#include "StdAfx.h"
#include "XLSimpleThreads.h"

static DWORD WINAPI RunSimpleThread(LPVOID data);

XLSimpleThreads::XLSimpleThreads(void):
m_Sync1(),
m_Sync2(),
m_handle(0),
mID(0)
{
}

XLSimpleThreads::~XLSimpleThreads(void)
{
}

bool XLSimpleThreads::Create(const char *threadname, unsigned long stackSize)
{
	mName = threadname;
	m_Sync1.Create(1);
	if(!m_Sync1.IsCreated())
	{
		return false;
	}
	m_Sync2.Create(1);
	if(!m_Sync2.IsCreated())
	{
		m_Sync1.Destroy();
		return false;
	}
	m_handle = (int)CreateThread(NULL, (unsigned int)stackSize, RunSimpleThread, (LPVOID)this, 0, (DWORD*)&mID); 
	m_Sync1.Give();

	return true;
}

void XLSimpleThreads::Destroy()
{
	if(!IsCreated())
	{
		return;
	}
	m_Sync2.Take();
	if(	m_Sync1.IsCreated())
	{
		m_Sync1.Destroy();
	}
	if(	m_Sync2.IsCreated())
	{
		m_Sync2.Destroy();
	}
	CloseHandle((HANDLE)m_handle);
	mID = 0;

}

DWORD WINAPI RunSimpleThread(LPVOID data)
{
	XLSimpleThreads* Thread = (XLSimpleThreads*)data;
	if(Thread)
	{
		Thread->m_Sync1.Take();
		if(Thread->IsCreated())
		{
			Thread->RunProgram();
		}
		Thread->m_Sync2.Give();
	}

	return 0;
}
