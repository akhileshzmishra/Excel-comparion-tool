#ifndef _TRIE___MAP__
#define _TRIE___MAP__

#include <map>

template<class T, class D>
class TNodeItr;

template<class T, class D>
class TNode
{
	typedef std::map<T, TNode<T,D>* >           ChildList;
	typename typedef ChildList::iterator        ChildItr;
	typedef std::pair<T, TNode<T,D>* >          ChildPair;

	ChildList                                   mChildList;
	T                                           mKey;
	D                                           mNodeValue;
	bool                                        mIsALeaf;
	friend class TNodeItr<T, D>;

public:
	TNode():
    mIsALeaf(false)
	{
	}
	TNode(T Key):
	mKey(Key),
	mIsALeaf(false)
	{
	}


	~TNode()               {}
	D& Value()             {return mNodeValue;}
	T& Key()               {return mKey;}
	bool IsLeafNode()      {return mIsALeaf;}
	int NoChildren()       {return (int) mChildList.size();}


	TNode<T,D>* GetChild(T data)
	{
		ChildItr itr = mChildList.find(data);
		if(itr != mChildList.end())
		{
			return itr->second;
		}
		return 0;
	}

	TNode<T,D>* InsertChild(T key)
	{
		ChildItr itr = mChildList.find(key);
		if(itr == mChildList.end())
		{
			TNode<T, D>* ptr = new TNode<T, D>(key)
			mChildList.insert(ChildPair(T, ptr ));
			return ptr;
		}
		else
		{
			return itr->second;
		}
	}

	void SetLeafNode(D data)
	{
		mNodeValue = data;
		mIsALeaf = true;
	}
	void FlushData()
	{
	    mIsALeaf = false;
	}
private:
	bool IsEnd(const ChildItr& itr)
	{
		return (itr == mChildList.end());
	}
	bool IsBegin(const ChildItr& itr)
	{
		return (itr == mChildList.begin());
	}

};

template<class T, class D>
class TNodeItr
{
	TNode<T, D>*                           mHead;
	typedef typename TNode<T, D>::ChildItr BaseItr;
	BaseItr                                mItr;
public:
	TNodeItr(TNode<T, D>* head):
    mHead(head)
	{
		if(mHead)
		{
			mItr = mHead->mChildList.begin();
		}
	}
	bool IsEnd()
	{
		if(mHead)
		{
			return mHead->IsEnd(mItr);
		}
		return false;
	}
	bool IsBegin()
	{
		if(mHead)
		{
			return mHead->IsBegin(mItr);
		}
		return false;
	}
	void operator ++()
	{
		mItr++;
	}
	void operator ++(int)
	{
		mItr++;
	}
	void operator --()
	{
		mItr--;
	}
	void operator --(int)
	{
		mItr--;
	}
	TNode<T, D>* Value()
	{
		return mItr->second;
	}


};

template<class T, class D>
class TTrieMap
{
	TNode<T, D>   mHead;
	int           mDepth;
public:
	TTrieMap():
	mDepth(0)
	{
	}
	void Insert(T* tArray, int ArrSize, D Value)
	{
		TNode<T, D>*& ref = &mHead;
		int depth = 0;
		for(int i = 0; i < ArrSize; i++)
		{
			if(ref)
			{
				ref = ref->Insert(tArray[i]);
				depth++;
			}
		}
		if(ref)
		{
			ref->SetLeafNode(Value);
		}
		if(depth > mDepth)
		{
			mDepth = depth;
		}

	}

	D* Value(T* tArray, int ArrSize)
	{
		TNode<T, D>*& ref = &mHead;
		for(int i = 0; i < ArrSize; i++)
		{
			if(ref)
			{
				ref = ref->GetChild(tArray[i]);
			}
		}
		if(ref)
		{
			if(ref->IsLeafNode())
			{
				return &(ref->Value());
			}
		}
		return 0;
	}
	int MaxDepth()
	{
		return mDepth;
	}

};


#endif
