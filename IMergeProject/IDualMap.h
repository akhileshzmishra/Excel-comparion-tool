#ifndef _I_DUAL_MAP_H_
#define _I_DUAL_MAP_H_

#include <map>

template<class T, class X, class Fnt, class FnX>
class DualMapItr;

template<class T, class X, class FnT = std::less<T>, class FnX = std::less<X> >
class DualMap
{
public:
	typedef std::map<T, X, FnT> FirstMap;
	typedef std::map<X, T, FnX> SecondMap;
	typedef typename FirstMap::iterator FirstItr;
	typedef typename SecondMap::iterator SecondItr;
private:
	typedef std::pair<T, X> FirstPair;
	typedef std::pair<X, T> SecondPair;
	FirstMap  mFirst;
	SecondMap mSecond;
	friend DualMapItr<T, X, FnT, FnX>;
public:
	DualMap()
	{
	}
	~DualMap()
	{
	}
	void Insert(T& t, X& x)
	{
		mFirst.insert(FirstPair(t, x));
		mSecond.insert(SecondPair(x, t));
	}
	bool FindSecond(T& t, X& ret)
	{
		FirstItr itr = mFirst.find(t);
		if(itr != mFirst.end())
		{
			ret = itr->second;
			return true;
		}
		return false;
	}
	bool FindFirst(X& x, T& ret)
	{
		SecondItr itr = mSecond.find(x);
		if(itr != mSecond.end())
		{
			ret = itr->second;
			return true;
		}
		return false;
	}
	void Delete(X& x, T& t)
	{
		mFirst.erase(t);
		mSecond.erase(x);
	}

	int Size()
	{
		return (int)mFirst.size();
	}

	void Clear()
	{
		mFirst.clear();
		mSecond.clear();
	}


};

template<class T, class X, class FnT = std::less<T>, class FnX = std::less<X> >
class DualMapItr
{
	DualMap<T, X, FnT, FnX>*                   mMap;
	bool                                       mFirst;
	typedef typename DualMap<T, X, FnT, FnX>::FirstItr    FITR;
	typedef typename DualMap<T, X, FnT, FnX>::SecondItr   SITR;
	FITR                                       mfitr;
	SITR                                       msitr;
public:
	DualMapItr(DualMap<T, X, FnT, FnX>* _Map, bool ForFirst):
	mMap(_Map),
	mFirst(ForFirst)
	{
		Reset();
	}
	void operator++()
	{
	    if(mFirst)
		{
			mfitr++;
		}
		else
		{
			msitr++;
		}
	}
	void operator--()
	{
	    if(mFirst)
		{
			mfitr--;
		}
		else
		{
			msitr--;
		}
	}
	void operator--(int)
	{
	    operator--();
	}
	void operator++(int)
	{
		operator++();
	}
	bool IsEnd()
	{
		if(mFirst)
		{
			return (mfitr == mMap->mFirst.end());
		}
		return (msitr == mMap->mSecond.end());
	}
	bool IsBegin()
	{
		if(mFirst)
		{
			return (mfitr == mMap->mFirst.begin());
		}
		return (msitr == mMap->mSecond.begin());
	}
	X& GetFValue()
	{
		return mfitr->second;
	}
	T& GetSValue()
	{
		return msitr->second;
	}
	void Reset()
	{
		mfitr = mMap->mFirst.begin();
		msitr = mMap->mSecond.begin();
	}

};



#endif