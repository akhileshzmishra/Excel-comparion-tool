#ifndef ___LINKED__LIST___H___
#define ___LINKED__LIST___H___

#include <vector>
#include <list>
#include "IObserver.h"

template<class T>
class LListNode
{
	T             mData;
	LListNode<T>* mNext;
	LListNode<T>* mPrevious;
public:
	LListNode():mNext(0),mPrevious(0)
	{
	}
	LListNode(T data):mData(data), mNext(0),mPrevious(0)
	{
	}
	LListNode<T>*& Next()       { return mNext;}
	LListNode<T>*& Previous()   { return mPrevious;}
	T& Data()                   { return mData;}
};

enum LinkedListEvents
{
	LinkedList_AddNodeFront,
	LinkedList_DeleteNodeFront,
	LinkedList_AddNodeBack,
	LinkedList_DeleteNodeBack,

	LinkedList_DeleteNode,
	LinkedList_ClearedAll,

	LinkedListEvents_Maximum
};

template<class T>
class LinkedListData
{
public:
	LinkedListEvents Event;
	LListNode<T>*    Node;
	LinkedListData():Node(0) {}
};

template<class T>
class LinkedListItr;


template<class T>
class LinkedList: public Subject<LinkedListData<T> >
{
	LListNode<T>* mHead;
	LListNode<T>* mTail;
	int           mElements;
	friend class LinkedListItr<T>;
public:
	LinkedList(): mHead(0), mTail(0), mElements(0)
	{
	}
	~LinkedList()
	{
		Clear();
	}

	void Clear()
	{
		LListNode<T>* node = mHead;
		while(node)
		{
			mHead = mHead->Next();
			delete node;
			node = mHead;
		}
		mHead = 0;
		mTail = 0;
		mElements = 0;
		CallListener(LinkedList_ClearedAll, 0);
	}

	LListNode<T>* FrontNode()
	{
		return mHead;
	}

	LListNode<T>* BackNode()
	{
		return mTail;
	}

	void InsertAtBack(T data)
	{
		LListNode<T>* newNode = new LListNode<T>(data);
		if(!mTail)
		{
			mTail = newNode;
			mHead = mTail;
		}
		else
		{
			mTail->Next() = newNode;
			newNode->Previous() = mTail;
			mTail = mTail->Next();
		}
		CallListener(LinkedList_AddNodeBack, mTail);
		mElements++;
	}
	void InsertAtFront(T data)
	{
		if(!mHead)
		{
			mHead = new LListNode<T>(data);
			mTail = mHead;
		}
		else
		{
			LListNode<T>* newNode = new LListNode<T>(data);
			mHead->Previous() = newNode;
			newNode->Next() = mHead;
			mHead = mHead->Previous();
		}
		CallListener(LinkedList_AddNodeFront, mHead);
		mElements++;
	}
	bool RemoveFromFront(T& output)
	{
		if(!mHead)
			return false;

		output = mHead->Data();	
		LListNode<T>* secHead = mHead;
		mHead = mHead->Next();
		if(mHead)
		{
			mHead->Previous() = 0;
		}
		if(mTail == secHead)
		{
			mTail = 0;
		}
		CallListener(LinkedList_DeleteNodeFront, secHead);
		mElements--;
		delete secHead;
		return true;

	}
	bool RemoveFromBack(T& output)
	{
		if(!mTail)
			return false;

		output = mTail->Data();
		LListNode<T>* secHead = mTail;
		mTail = mTail->Previous();
		if(mTail)
		{
			mTail->Next() = 0;
		}
		if(mHead == secHead)
		{
			mHead = 0;
		}
		CallListener(LinkedList_DeleteNodeBack, secHead);
		mElements--;
		delete secHead;	
		return true;
	}
	bool Front(T& output)
	{
		if(!mHead)
			return false;

		output = mHead->Data();
		return true;
	}
	bool Back(T& output)
	{
		if(!mTail)
			return false;

		output = mTail->Data();
		return true;
	}
	int Size()
	{
		return mElements;
	}
	void Delete(LListNode<T>* node)
	{
		if(!node)
			return;

		if(node == mHead)
		{
			mHead = mHead->Next();
		}
		if(node == mTail)
		{
			mTail = mTail->Previous();
		}
		CallListener(LinkedList_DeleteNode, node);
		LListNode<T>* nxt = node->Next();
		LListNode<T>* pre = node->Previous();
		if(nxt)
		{
			nxt->Previous() = pre;
		}
		if(pre)
		{
			pre->Next() = nxt;
		}
		delete node;

	}
private:
	void CallListener(LinkedListEvents evnets, LListNode<T>* Node)
	{
		LinkedListData<T> notifyData;
		notifyData.Event = evnets;
		notifyData.Node = Node;
		Notify(notifyData);
	}
	bool IsBegin(LListNode<T>* node)
	{
		if(!mHead || !node)
			return false;

		return (node == mHead);
	}
	bool IsEnd(LListNode<T>* node)
	{
		if(!mTail || !node)
			return false;

		return (node == mTail);
	}
};



template<class T>
class LinkedListItr	: public IObserver<LinkedListData<T> >
{
	LinkedList<T>* mList;
	LListNode<T>*  mItr;
	int            mCounter;
public:
	LinkedListItr(LinkedList<T>* alist): 
	IObserver<LinkedListData<T> >(alist), 
	mList(alist),
	mCounter(0)
	{
		if(mList)
		{
			mItr = mList->mHead;
		}
	}

	void DeleteThis()
	{
		if(mList)
		{
			mList->Delete(mItr);
		}
	}

	void SetToFront()
	{
		if(mList)
		{
			mItr = mList->FrontNode();
		}
	}

	void SetToBack()
	{
		if(mList)
		{
			mItr = mList->BackNode();
		}
	}

	void operator++()
	{
		if(!mItr)
			return;
		if(mList->Size() == 0)
		{
			return;
		}
		if(!mList->IsEnd(mItr))
		{
			mItr = mItr->Next();
		}
	}
	void operator++(int)
	{
		if(!mItr)
			return;
		if(mList->Size() == 0)
		{
			return;
		}
		if(!mList->IsEnd(mItr))
		{
			mItr = mItr->Next();
		}
	}
	void operator--()
	{
		if(!mItr)
			return;
		if(mList->Size() == 0)
		{
			return;
		}
		if(!mList->IsBegin(mItr))
		{
			mItr = mItr->Previous();
		}
	}
	void operator--(int)
	{
		if(!mItr)
			return;
		if(mList->Size() == 0)
		{
			return;
		}
		if(!mList->IsBegin(mItr))
		{
			mItr = mItr->Previous();
		}
	}
	T* Value()
	{
		if(mItr)
		{
			return &(mItr->Data());
		}
		return 0;
	}
	LListNode<T>* Node()
	{
		return mItr;
	}
	bool HasNext()
	{
		if(!mItr)
			return false;

		return (mItr->Next() != 0);
	}
	bool HasPrevious()
	{
		if(!mItr)
			return false;

		return (mItr->Previous() != 0);
	}
	bool AtFirst()
	{
		if(!mItr)
			return false;

		return (mList->IsBegin(mItr));
	}
	bool AtEnd()
	{
		if(!mItr)
			return false;

		return (mList->IsEnd(mItr));
	}
	void Reset()
	{
		if(mList)
		{
			mItr = mList->mHead;
		}
	}
	void Notify(LinkedListData<T> listData)
	{
		switch(listData.Event)
		{
		case LinkedList_AddNodeFront:
			if(!mItr)
			{
				mItr = mList->mHead;
			}
			break;
		case LinkedList_AddNodeBack:
			if(!mItr)
			{
				mItr = mList->mTail;
			}
			break;
		case LinkedList_DeleteNodeFront:
			if(mItr == listData.Node)
			{
				if(mItr->Next())
				{
					mItr = mItr->Next();
				}
				else
				{
					mItr = 0;
				}
			}
			break;
		case LinkedList_DeleteNodeBack:
			if(mItr == listData.Node)
			{
				if(mItr->Previous())
				{
					mItr = mItr->Previous();
				}
				else
				{
					mItr = 0;
				}
			}
			break;
		case LinkedList_DeleteNode:
			if(mItr == listData.Node)
			{
				if(mItr->Previous())
				{
					mItr = mItr->Previous();
				}
				else if(mItr->Next())
				{
					mItr = mItr->Next();
				}
				else
				{
					mItr = 0;
				}
			}
			break;
		case LinkedList_ClearedAll:
			mItr = 0;
			break;
		default:
			break;
		}
	}
protected:
	void __SetItrAT(LListNode<T>* node)
	{
		if(mList && node)
		{
			mItr = node;
		}
	}
};



#endif
