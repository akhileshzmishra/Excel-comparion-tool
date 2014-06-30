#ifndef _TRIE___SET__
#define _TRIE___SET__

#include <map>
#include <vector>
#include <stack>
#include <queue>

template<class T>
class TSNodeItr;

template<class T>
class TSNode
{
	typedef std::map<T, TSNode<T>* >             ChildList;
	typename typedef ChildList::iterator         ChildItr;
	typedef std::pair<T, TSNode<T>* >            ChildPair;

	ChildList                                   mChildList;
	T                                           mKey;
	bool                                        mIsALeaf;
	friend class TSNodeItr<T>;

public:
	TSNode():
    mIsALeaf(false)	   
	{
	}
	TSNode(T key):
	mKey(key),
    mIsALeaf(false)	   
	{
	}
	

	~TSNode()              {}
	T& Key()               {return mKey;}
	bool IsLeafNode()      {return mIsALeaf;}
	int NoChildren()       {return (int) mChildList.size();}


	TSNode<T>* GetChild(T data)		
	{
		ChildItr itr = mChildList.find(data);
		if(itr != mChildList.end())
		{
			return itr->second;
		}
		return 0;
	}

	TSNode<T>* InsertChild(T key)
	{
		ChildItr itr = mChildList.find(key);
		if(itr == mChildList.end())
		{
			TSNode<T>* ptr = new TSNode<T>(key) ;
			mChildList.insert(ChildPair(key, ptr ));   
			return ptr;
		}
		else
		{
			return itr->second;
		}
	}

	void DeleteKey(T Key)
	{
	    ChildItr itr = mChildList.find(Key);
		if(itr != mChildList.end())
		{
			delete itr->second;
			mChildList.erase(itr);
		}
	}

	void SetLeafNode(bool set)
	{
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

template<class T>
class TSNodeItr
{
	TSNode<T>*                           mHead;
	typedef typename TSNode<T>::ChildItr BaseItr;
	BaseItr                              mItr;
public:
	TSNodeItr(TSNode<T>* head):
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
	TSNode<T>* Value()
	{
		return mItr->second;
	}


};

template<class T>
struct TSetValue
{
	T*  Array;
	int SizeArray;

	TSetValue():
	Array(0),
	SizeArray(0)
	{
	}
	TSetValue(int size):
	Array(new T[size]),
	SizeArray(size)
	{
	}
	~TSetValue()
	{
		if(Array)
		{
			delete []Array;
			Array = 0;
			SizeArray = 0;
		}
	}
	TSetValue(const TSetValue<T>& other):
	Array(0),
	SizeArray(0)
	{
		if(other.SizeArray > 0)
		{
			Array = new T[other.SizeArray];
			memcpy(Array, other.Array, sizeof(T)*other.SizeArray);
			SizeArray = other.SizeArray;
		}
	}
	TSetValue& operator = (const TSetValue<T>& other)
	{
		if(this == &other)
		{
			return *this;
		}
		if(Array)
		{
			delete []Array;
			Array = 0;
			SizeArray = 0;
		}
		if(other.SizeArray > 0)
		{
			Array = new T[other.SizeArray];
			memcpy(Array, other.Array, sizeof(T)*other.SizeArray);
			SizeArray = other.SizeArray;
		}

	}
	T& operator [](int x)
	{
		return Array[x];
	}
};

template<class T>
struct TSStackNode
{
	TSNode<T>* Node;
	int        Level;
	TSStackNode():
	Node(0),
	Level(0)
	{
	}
	TSStackNode(TSNode<T>* node, int level):
	Node(node),
	Level(level)
	{
	}
};

template<class T>
class TTrieSet
{
	TSNode<T>   mHead;
	int         mDepth;
public:
	TTrieSet():
	mDepth(1)
	{
	}
	~TTrieSet()
	{
		DeleteAllItems();
	}
	void Insert(T* tArray, int ArrSize)
	{
		TSNode<T>*  ptr  = &mHead;
		int depth = 1;
		for(int i = 0; i < ArrSize; i++)
		{
			if(ptr)
			{
				ptr = ((ptr)->InsertChild(tArray[i]));
				depth++;
			}
			else
			{
				break;
			}
		}
		if(ptr)
		{
			(ptr)->SetLeafNode(true);
		}
		if(depth > mDepth)
		{
			mDepth = depth;
		}

	}

	bool Value(T* tArray, int ArrSize)
	{
		if(ArrSize > mDepth)
		{
			return 0;
		}
		if(!tArray)
		{
			return 0;
		}

		TSNode<T>*  ptr  = &mHead;
		TNode<T, D>*& ref = ptr;
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
				return ref->IsLeafNode();
			}
		}
		return false;
	}
	int MaxDepth()
	{
		return mDepth;
	}
	bool Delete(T* tArray, int ArrSize)
	{
		if(ArrSize > mDepth)
		{
			return false;
		}
		if(!tArray)
		{
			return false;
		}
		TSNode<T>* ref = &mHead;
		std::stack<TSNode<T>* > Stack;
		for(int i = 0; i < ArrSize; i++)
		{
			if(ref)
			{
				ref = ref->GetChild(tArray[i]);
				Stack.push(ref);
			}
			else
			{
				return false;
			}
		}
		if(ref)
		{
			if(!ref->IsLeafNode())
			{
				return false;
			}
			TSNode<T>* nodeup = Stack.top();
			Stack.pop();
			T key = nodeup->Key();
			while(Stack.size() > 0)
			{
				TSNode<T>* node = Stack.top();
				Stack.pop();
				if(node)
				{
					TSNode<T>* child = node->GetChild(key);
					if(child)
					{
						if(child->NoChildren() == 0)
						{
							node->DeleteKey(key);
							key = node->Key();
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
		}
		return true;
	}
	std::vector<TSetValue<T>> GetAllMatches(T* tArray, int ArrSize)
	{
		std::vector<TSetValue<T>> result;
		if(ArrSize > mDepth)
		{
			return result;
		}
		if(!tArray)
		{
			return result;
		}
		if(mDepth > 0)
		{
			TSetValue<T> newVal(mDepth);

			memcpy(newVal.Array, tArray, ArrSize);
			TSNode<T>* ref = &mHead;
			for(int i = 0; i < ArrSize; i++)
			{
				if(ref)
				{
					ref = ref->GetChild(tArray[i]);
				}
				else
				{
					return result;
				}
			}
			if(ref)
			{
				std::stack<TSStackNode<T> > Stack;
				Stack.push(TSStackNode<T>(ref, 0));
				int pos = ArrSize - 1;
				while(Stack.size() > 0)
				{
					TSStackNode<T> node = Stack.top();
					Stack.pop();
					if((node.Node->IsLeafNode()) || (node.Node->NoChildren() == 0))	
					{
						newVal[pos + node.Level] = node.Node->Key();
						newVal[pos + node.Level + 1] = 0;
						result.push_back(newVal);
					}
					else
					{
						newVal[pos + node.Level] = node.Node->Key();
						TSNodeItr<T> itr(node.Node);
						while(!itr.IsEnd())
						{
							Stack.push(TSStackNode<T>(itr.Value(), node.Level + 1));
							itr++;
						}
					}


				}

			}
			return result;

		}
	}



private:
	void DeleteAllItems()
	{
		std::queue<TSNode<T>* > Q;

		TSNodeItr<T> itr(&mHead);
		while(!itr.IsEnd())
		{
			Q.push(itr.Value());
			itr++;
		}

		while(Q.size() > 0)
		{
			TSNode<T>* node = Q.front();
			Q.pop();
			TSNodeItr<T> itr1(node);
			while(!itr1.IsEnd())
			{
				Q.push(itr1.Value());
				itr1++;
			}

			delete node;
		}
	}

};


#endif
