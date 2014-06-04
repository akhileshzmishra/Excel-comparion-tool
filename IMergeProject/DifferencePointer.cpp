// DifferencePointer.cpp : implementation file
//

#include "stdafx.h"
#include "IMergeProject.h"
#include "DifferencePointer.h"
#include "CommonHeader.h"


// CDifferencePointer

static const XLEventType Conditions[] = {GRID_POINTER_MOVE_LEFT};
static const XLEventType ConditionsR[] = {GRID_POINTER_MOVE_RIGHT};

IMPLEMENT_DYNAMIC(CDifferencePointer, CBitmapButton)

CDifferencePointer::CDifferencePointer(ScrollSubject* subject, DiffPointerType type):
ScrollObserver(subject, ((type == DiffPointerLeftType)? (XLEventType*)Conditions: (XLEventType*)ConditionsR) , 1),
mView(0),
m_bCreate(false),
mX(0),
mY(0),
mType(type)
{ 
	EnableAutomation();
}

CDifferencePointer::~CDifferencePointer()
{
}

void CDifferencePointer::Initialize(CView* view)
{
	mView = view;
	mX = DIFF_POINTER_BUTTON_X;
	mY = DIFF_POINTER_BUTTON_Y;
	RECT rect;
	rect.left = mX;
	rect.right = rect.left +  DIFF_POINTER_BUTTON_W;
	rect.top = mY;
	rect.bottom = rect.top + DIFF_POINTER_BUTTON_H;

	

	if(mType == DiffPointerLeftType)
	{
		Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		      rect, mView, IDC_SHIFT_BUTTON_LEFT1);
		LoadBitmaps(IDB_LEFT_ARROW, IDB_LEFT_ARROW_FOCUS, IDB_LEFT_ARROW_FOCUS);
	}
	else
	{
		Create(NULL, WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 
		      rect, mView, IDC_SHIFT_BUTTON_RIGHT);
		LoadBitmaps(IDB_RIGHT_ARROW, IDB_RIGHT_ARROW_FOCUS, IDB_RIGHT_ARROW_FOCUS);
	}

	m_bCreate = true;

}

void CDifferencePointer::Show()
{
	ShowWindow(SW_SHOW);
}
void CDifferencePointer::Hide()
{
	ShowWindow(SW_HIDE);
}

void CDifferencePointer::Move(int x, int y)
{
	if(!m_bCreate)
	{
		return;
	}
	mX = x;
	mY = y;
	RECT rect;
	rect.left = x;
	rect.right = rect.left +  DIFF_POINTER_BUTTON_W;
	rect.top = y;
	rect.bottom = rect.top + DIFF_POINTER_BUTTON_H;
	MoveWindow(&rect);
	
}

void CDifferencePointer::SetPosition(int x, int y)
{
	Move(x, y);
}

void CDifferencePointer::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CButton::OnFinalRelease();
}


void CDifferencePointer::Notify(ScrollData* data, XLEventType* condition)
{
	Move(mX, data->Data);
	if(data->Action == ActionShow)
	{
		Show();
	}
	else
	{
		Hide();
	}
}

BEGIN_MESSAGE_MAP(CDifferencePointer, CBitmapButton)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CDifferencePointer, CBitmapButton)
END_DISPATCH_MAP()

// Note: we add support for IID_IDifferencePointer to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {89E98F07-CE98-43A2-B151-1B94FEA615FD}
static const IID IID_IDifferencePointer =
{ 0x89E98F07, 0xCE98, 0x43A2, { 0xB1, 0x51, 0x1B, 0x94, 0xFE, 0xA6, 0x15, 0xFD } };

BEGIN_INTERFACE_MAP(CDifferencePointer, CBitmapButton)
	INTERFACE_PART(CDifferencePointer, IID_IDifferencePointer, Dispatch)
END_INTERFACE_MAP()


// CDifferencePointer message handlers


