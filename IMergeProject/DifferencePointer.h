#pragma once

#include "IProjectInterface.h"

// CDifferencePointer
enum DiffPointerType
{
	DiffPointerLeftType,
	DiffPointerRightType
};
enum
{
	ActionShow,
	ActionHide
};
class CDifferencePointer : public CBitmapButton , public XLCtrlObserver
{
	CView* mView;
	bool  m_bCreate;
	int   mX;
	int   mY;
	DiffPointerType mType;
	DECLARE_DYNAMIC(CDifferencePointer)

public:
	CDifferencePointer(XLCtrlSubject* subject, DiffPointerType type);
	virtual ~CDifferencePointer();

	void Initialize(CView* view = 0);
	void Show();
	void Hide();

	void Move(int x, int y);

	void SetPosition(int x, int y);

	void Notify(XLObservedData* data, XLEventType* condition);

	virtual void OnFinalRelease();

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};


