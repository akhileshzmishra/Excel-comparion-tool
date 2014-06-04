#ifndef _IOBSERVER_H_
#define _IOBSERVER_H_
#include "afxmt.h"
#include <set>
#include <map>
#include <vector>

template<class T> 
class Subject;

template<class T>
class IObserver
{
	Subject<T>* m_Subject;
public:
	IObserver(Subject<T>* subject): m_Subject(subject)
	{
		if(m_Subject)
		{
			m_Subject->RegisterObserver(this);
		}
	}
	virtual void Notify(T data) = 0;
	virtual ~IObserver()
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this);
		}
		m_Subject = 0;
	}
	Subject<T>* GetSubject()
	{
		return m_Subject;
	}
};


template<class T> 
class Subject
{
	typedef std::set<IObserver<T>* > ObserverList;
	typedef typename ObserverList::iterator ObserverItr;
	ObserverList m_ObsList;
public:
	Subject()
	{
	}
	void RegisterObserver(IObserver<T>* o)
	{
		m_ObsList.insert(o);
	}

	virtual void Notify(T data)
	{
		ObserverItr itr = m_ObsList.begin();
		while(itr != m_ObsList.end())
		{
			(*itr)->Notify(data);
			itr++;
		}
	}
	void DeRegister(IObserver<T>* o)
	{
		ObserverItr itr = m_ObsList.find(o);
		if(itr != m_ObsList.end())
		{
			m_ObsList.erase(itr);
		}
	}
};


template<class T, class X> 
class ISubject2;

template<class T, class X>
class IObserver2
{
	ISubject2<T, X>* m_Subject;
public:
	IObserver2(ISubject2<T, X>* subject, std::vector<X>& conditions): m_Subject(subject)
	{
		if(m_Subject)
		{
			for(int i = 0; i < conditions.size(); i++)
			{
				m_Subject->Register(this, conditions[i]);
			}
		}
	}
	IObserver2(ISubject2<T, X>* subject, X* conditions = 0, int size = 0): m_Subject(subject)
	{
		if(m_Subject)
		{
			for(int i = 0; i < size; i++)
			{
				m_Subject->Register(this, conditions[i]);
			}
		}
	}
	void Register(X condition)
	{
		if(m_Subject)
		{
			m_Subject->Register(this, condition);
		}
	}
	virtual void DeRegister(X condition)
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this, condition);
		}
	}
	virtual void Notify(T* data, X* condition) = 0;
	virtual ~IObserver2()
	{
		if(m_Subject)
		{
			m_Subject->DeRegister(this);
		}
	}
	ISubject2<T, X>* GetSubject()
	{
		return m_Subject;
	}
	void SetSubject(ISubject2<T, X>* subject)
	{
		m_Subject = subject;
	}
};


template<class T, class X> 
class ISubject2 
{
	typedef std::multimap<X, IObserver2<T, X>* >   ObserverList;
	typedef std::pair<X, IObserver2<T, X>* >       ObsInsPair;
	typedef typename ObserverList::iterator            ObserverItr;
	ObserverList                                 m_list;
	CMutex                                       m_ListGate;
public:
	ISubject2<T, X>()
	{
	}
	void Register(IObserver2<T, X>* o, X condition)
	{
		m_ListGate.Lock();
		m_list.insert(ObsInsPair(condition, o));
		m_ListGate.Unlock();
	}
	void DeRegister(IObserver2<T, X>* o)
	{
		m_ListGate.Lock();
		std::vector<ObserverItr> deletITr;
		ObserverItr itr = m_list.begin();
		while(itr != m_list.end())
		{
			if(itr->second == o)
			{
				deletITr.push_back(itr);
			}

			itr++;
		}
		for(int i = 0; i < (int) deletITr.size(); i++)
		{
			m_list.erase(deletITr[i]);
		}
		m_ListGate.Unlock();
	}
	virtual void Notify(T* data, X condition)
	{
		m_ListGate.Lock();
		std::pair<ObserverItr, ObserverItr> obsPair = m_list.equal_range(condition);
		ObserverItr itr = obsPair.first;
		while(itr != obsPair.second)
		{
			itr->second->Notify(data, &condition);
			itr++;
		}
		m_ListGate.Unlock();
	}
	void DeRegister(IObserver2<T, X>* o, X condition)
	{
		m_ListGate.Lock();
		std::pair<ObserverItr, ObserverItr> obsPair = m_list.equal_range(condition);
		ObserverItr itr = obsPair.first;
		while(itr != obsPair.second)
		{
			if(o == itr->second)
			{
				m_list.erase(itr);
				break;
			}
			itr++;
		}
		m_ListGate.Unlock();
	}

};



#endif