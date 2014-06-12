#ifndef _XL_SEMAPHORE_H__
#define _XL_SEMAPHORE_H__

typedef void XLSemaphoreID;

class XLSemaPhore
{
	XLSemaphoreID* mID;
public:
	XLSemaPhore();
	bool Create(int count);
	bool IsCreated(void)                 { return mID != NULL; }
	void Destroy();
	~XLSemaPhore(void);

	void Take(void);
	bool Try(unsigned long msec = 0);
	void Give(void);
};





#endif