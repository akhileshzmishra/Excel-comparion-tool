#ifndef _INTERNALSMARTPOINTER_H_
#define _INTERNALSMARTPOINTER_H_



template<class T>
class XLSmartPtr
{
	mutable T* m_ptr;

public:
	XLSmartPtr(T* data): m_ptr(data)
	{
	}
	XLSmartPtr(const XLSmartPtr& data)
	{
		m_ptr = data.m_ptr;
		data.m_ptr = 0;
	}
	~XLSmartPtr()
	{
		if(m_ptr)
		{
			delete m_ptr;
		}
	}
	bool Viable()
	{
		return (m_ptr != 0);
	}
	T* operator->()
	{
		return m_ptr;
	}
	T& operator*()
	{
		return *m_ptr;
	}
	XLSmartPtr<T>& operator = (const XLSmartPtr& other)
	{
		if(this == &other)
			return *this;
		m_ptr = other.m_ptr;
		other.m_ptr = 0;
		return *this;
	}
	bool operator !()
	{
		return !(Viable());
	}
	operator bool()
	{
		return Viable();
	}

};







#endif
