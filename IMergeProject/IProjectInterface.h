#ifndef _IPROJECTINTERFACE_H_
#define _IPROJECTINTERFACE_H_

#include "IObserver.h"

enum XLEventType
{
	GRID_NONE = WM_USER + 1000,
	GRID_VSCROLL,
	GRID_HSCROLL,




	GRID_POINTER_MOVE_LEFT,
	GRID_POINTER_MOVE_RIGHT,


	GRID_LEFT_FILE_CHANGED,
	GRID_RIGHT_FILE_CHANGED,
	GRID_FILE_SAVED,


	EVENTTYP_MAX
};


enum XLWindowEventType
{
	WM_MODELESS_CLOSED = EVENTTYP_MAX + 1
};

struct ScrollData
{
	XLEventType EventType;
	int Data;
	int Action;
	ScrollData():EventType(GRID_NONE),Data(0), Action(0){}
	ScrollData(char* dest)
	{
		int x = 0;
		memcpy(&EventType, &dest[x], sizeof(XLEventType));
		x += sizeof(XLEventType);
		memcpy(&Data, &dest[x], sizeof(int));
		x += sizeof(int);
		memcpy(&Action, &dest[x], sizeof(int));
		x += sizeof(int);
	}
	void MakeStream(char* dest)
	{
		int x = 0;
		memcpy(&dest[x], &EventType, sizeof(XLEventType));
		x += sizeof(XLEventType);
		memcpy(&dest[x], &Data, sizeof(int));
		x += sizeof(int);
		memcpy(&dest[x], &Action, sizeof(int));
		x += sizeof(int);
	}
	static const int StreamSize()
	{
		return (sizeof(XLEventType) + sizeof(int) + sizeof(int));
	}

};

		
template <class T, class X>
class GridViewSubject : public ISubject2<T, X>
{
	IObserver2<T, X>* m_Observer;
public:
	GridViewSubject():
	m_Observer(0)
	{
	}
	void SetCurrentObserver(IObserver2<T, X>* observer)
	{
		m_Observer = observer;
	}
	IObserver2<T, X>* GetCurrentObserver()
	{
		return m_Observer;
	}
	virtual void SaveToDatabase(int r, int c, CString& value) = 0;
	virtual void OnCopyFromRightToLeft(int r1, int r2, int c1, int c2) = 0;
	virtual void OnCopyFromLeftToRight(int r1, int r2, int c1, int c2) = 0;
};


typedef IObserver2<ScrollData, XLEventType> ScrollObserver;
typedef GridViewSubject<ScrollData, XLEventType> ScrollSubject;




#endif