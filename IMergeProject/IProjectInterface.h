#ifndef _IPROJECTINTERFACE_H_
#define _IPROJECTINTERFACE_H_

#include "IObserver.h"

enum XLEventType
{
	XLEVENT_EVENT_NONE = WM_USER + 1000 /* Offset*/,

	//Scroll commands for view
	XLEVENT_GRID_VSCROLL,
	XLEVENT_GRID_HSCROLL,

	//Command for arrow button
	XLEVENT_POINTER_LEFT_MOVE,
	XLEVENT_POINTER_RIGHT_MOVE,

	//Command for DPTextBox. This is used to save files at the end if there is any change
	XLEVENT_LEFT_FILE_CHANGED,
	XLEVENT_RIGHT_FILE_CHANGED,
	XLEVENT_FILE_SAVED,

	XLEVENT_COMPARE_DONE,

	//Command for Sidebar. This is used to fill and change the display of sidebar
	XLEVENT_SIDEBAR_MAKE_ALL,
	XLEVENT_SIDEBAR_MAKE_ONE,
	XLEVENT_SIDEBAR_MAKE_NONE,


	XLEVENT_EVENTTYP_MAX
};


enum XLWindowEventType
{
	WM_MODELESS_CLOSED = XLEVENT_EVENTTYP_MAX + 1
};

enum
{
	XL_SIDEBAR_ACTION_DELETE_LINE_T1,
	XL_SIDEBAR_ACTION_DELETE_LINE_T2,
	XL_SIDEBAR_ACTION_ADD_LINE_T1,
	XL_SIDEBAR_ACTION_ADD_LINE_T2,

	XL_SIDEBAR_ACTION_MAX
};

struct XLObservedData
{
	XLEventType EventType;
	int Data;
	int Action;
	XLObservedData():EventType(XLEVENT_EVENT_NONE),Data(0), Action(0){}
	XLObservedData(char* dest)
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
	virtual void SetCurrentObserver(IObserver2<T, X>* observer)
	{
		m_Observer = observer;
	}
	IObserver2<T, X>* GetCurrentObserver()
	{
		return m_Observer;
	}
	virtual void SaveToDatabase(int r, int c, CString& value) = 0;
	//virtual void SaveToTable(int r, int c, CString& value, int table) = 0;
	virtual void OnCopyFromRightToLeft(int r1, int r2, int c1, int c2) = 0;
	virtual void OnCopyFromLeftToRight(int r1, int r2, int c1, int c2) = 0;
	virtual void OnMoveFromRightToLeft(int r1, int r2, int c1, int c2) = 0;
	virtual void OnMoveFromLeftToRight(int r1, int r2, int c1, int c2) = 0;
};

template<class T, class X>
class GridViewObserver : public IObserver2<T, X>
{
public:
	GridViewObserver(ISubject2<T, X>* subject, std::vector<X>& conditions):
	IObserver2(subject, conditions)
	{
	}
	GridViewObserver(ISubject2<T, X>* subject, X* conditions = 0, int size = 0):
	IObserver2(subject, conditions, size)
	{
	}
	virtual void SaveToModelDB(int r, int c, CString& str) {}
	virtual void SetThisObserverCurrent()                  {}
};


typedef GridViewObserver<XLObservedData, XLEventType> XLCtrlObserver;
typedef GridViewSubject<XLObservedData, XLEventType> XLCtrlSubject;




#endif
