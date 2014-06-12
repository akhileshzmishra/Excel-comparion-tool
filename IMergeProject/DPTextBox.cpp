#include "StdAfx.h"
#include "DPTextBox.h"
#include "GridTableCompareView.h"

DPTextBox::DPTextBox(RECT rect)	:
XLCtrlObserver(GridTableCompareView::GetInstance(),0,0),
m_X(0),
m_Y(0),
m_bTextSet(false),
m_bCreate(false),
m_Parent(0),
m_bLeft(false),
m_bIsDirty(false)
{
	memcpy(&m_Rectangle, &rect, sizeof(RECT));
}

DPTextBox::DPTextBox():
XLCtrlObserver(GridTableCompareView::GetInstance(),0,0),
m_X(0),
m_Y(0),
m_bTextSet(false),
m_bCreate(false),
m_Parent(0),
m_bLeft(false),
m_bIsDirty(false)
{
}

DPTextBox::~DPTextBox(void)
{
}

void DPTextBox::Create(CView* parent, RECT rect, bool bLeft)
{
	m_bLeft = bLeft;
	memcpy(&m_Rectangle, &rect, sizeof(RECT));
	m_Parent = parent;
	m_Create();
	if(m_bLeft)
	{
		Register(GRID_LEFT_FILE_CHANGED);
	}
	else
	{
		Register(GRID_RIGHT_FILE_CHANGED);
	}
	Register(GRID_FILE_SAVED);
}

void DPTextBox::Notify(XLObservedData* data, XLEventType* condition)
{
	if(*condition == GRID_FILE_SAVED)
	{
	   m_SetDirty(false);
	}
	else
	{
		m_SetDirty(true);
	}
}

void DPTextBox::m_SetDirty(bool set)
{
	std::string dirty = m_str + "*";
	if(set)
	{
		m_SetText(dirty);
	}
	else
	{
		m_SetText(m_str);
	}
	m_bIsDirty = set;
}

void DPTextBox::SetRect(RECT rect)
{
	memcpy(&m_Rectangle, &rect, sizeof(RECT));
}
void DPTextBox::SetText(std::string& text)
{
	m_str = text;
	m_SetText(m_str);
}
void DPTextBox::m_SetText(std::string str)
{
	std::wstring wstr(str.begin(), str.end());
	m_String.SetString(wstr.c_str());
	m_bTextSet = true;
	if(m_bCreate)
	{
		SetWindowText((LPCTSTR)wstr.c_str());
		UpdateData();
		
	}
}
void DPTextBox::Draw(CDC* pDC)
{
}


void DPTextBox::m_Create()
{
	if(m_Parent && !m_bCreate)
	{
		static int uint = 7839723;
		m_bCreate = CEdit::Create( ES_AUTOHSCROLL | WS_BORDER | ES_LEFT , m_Rectangle, m_Parent, uint++);

		if(m_bCreate)
		{
			EnableToolTips(TRUE);
			//SetBkColor(DARKSLATEGRAYCOL);
			//SetText(std::string("File Name will be displayed here"));
			EnableTrackingToolTips(TRUE);
			SetMargins(10, 100);
			ShowWindow(SW_SHOW);
		}


	}
}