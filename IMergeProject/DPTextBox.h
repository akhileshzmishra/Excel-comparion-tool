#ifndef _DPTEXTBOX_H___
#define _DPTEXTBOX_H___
#include "CommonHeader.h"
#include "IProjectInterface.h"

class DPTextBox	:   public CEdit, public XLCtrlObserver
{
	typedef CEdit _Parent;
	CString    m_String;
	std::string m_str;
	RECT       m_Rectangle;
	int        m_X;
	int        m_Y;
	bool       m_bTextSet;
	bool       m_bCreate;
	CView*     m_Parent;
	bool       m_bLeft;
	bool       m_bIsDirty;
public:
	DPTextBox(RECT rect);
	DPTextBox();
	~DPTextBox(void);
	void Create(CView* parent, RECT rect, bool bLeft = true);
	void SetRect(RECT rect);
	void SetText(std::string& text);
	void Draw(CDC* pDC);
	void SetPosition(int x, int y)
	{
		m_X = x;
		m_Y = y;
	}
	bool IsDirty()
	{
		return m_bIsDirty;
	}

	virtual void Notify(XLObservedData* data, XLEventType* condition);
private:
	void m_Create();
	void m_SetDirty(bool set);
	void m_SetText(std::string str);
};




#endif