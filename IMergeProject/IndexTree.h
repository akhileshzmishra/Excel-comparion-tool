#ifndef _INDEX_TREE_H_
#define _INDEX_TREE_H_

#include <vector>
#include <iostream>
#include <stack>

using namespace std;
namespace IndexTreeSpace
{

const int DefaultPocketSize = 16;

template<class T>
class IndexPocket
{
	typedef std::vector<T> DataList;
	int                    m_iCapacity;
	int                    m_iSize;
	bool                   m_bAutoEnhance;
	DataList               m_DataList;
public:
	IndexPocket(int initialSize, bool autoEnhance = false):
	m_iCapacity(initialSize > 0? initialSize: DefaultPocketSize),
	m_iSize(0),
	m_bAutoEnhance(autoEnhance),
	m_DataList(m_iCapacity)
	{
	}

	IndexPocket(const IndexPocket<T>& other):
	m_iCapacity(other.m_iCapacity),
	m_iSize(0),
	m_bAutoEnhance(other.m_bAutoEnhance),
	m_DataList(other.m_DataList)
	{
	}

	IndexPocket<T>& operator = (IndexPocket<T>& other)
	{
		if(this == &other)
		{
			return *this;
		}

		m_iCapacity = other.m_iCapacity;
		m_iSize = other.m_iSize;
		m_bAutoEnhance = other.m_bAutoEnhance;
		m_DataList = other.m_DataList;

		return *this;
	}

	bool Expandable()                        {return m_bAutoEnhance;}
	int  Size()                              {return m_iSize;}
	int  Capacity()                          {return m_iCapacity;}
	bool Full()                              {return (Size() == Capacity());}
	bool Empty()                             {return (Size() == 0);}
	void SetExpandable(bool set)             {m_bAutoEnhance = set;}


	bool Insert(int index, T Data)
	{
		if(m_iSize == m_iCapacity)
		{
			if(!Expandable())
			{
				return false;
			}

			if(!__Expand())
			{
				return false;
			}
		}

		if(__ShiftRight(index))
		{
			m_DataList[index] = Data;
			m_iSize++;
			return true;
		}
		if(m_iSize == index)
		{
			m_DataList[index] = Data;
			m_iSize++;
			return true;
		}

		return false;

	}

	bool Pushback(T Data)
	{
		return Insert(m_iSize, Data);
	}

	bool Pushfront(T Data)
	{
		return Insert(0, Data);
	}

	bool Delete(int index)
	{
		if(__ShiftLeft(index))
		{
			m_iSize--;
			return true;
		}

		return false;
	}

	bool Popback()
	{
		return Delete(m_iSize - 1);
	}

	bool Popfront()
	{
		return Delete(0);
	}


	T& operator [] (int index)
	{
		if(__IndexInsideSize(index))
		{
			return m_DataList[index];
		}
		//assert(0);
		return m_DataList[0];

	}

	T* Value(int index)
	{
		if(__IndexInsideSize(index))
		{
			return &m_DataList[index];
		}
		return 0;
	}

	T* Front()
	{
		return Value(0);
	}

	T* Back()
	{
		return Value(m_iSize - 1);
	}

	IndexPocket<T>* Divide()
	{
		if(!__IndexInsideSize(1))
		{
			return false;
		}
		int mid = m_iSize/2;
		IndexPocket<T>* newPocket = new IndexPocket<T>(m_iCapacity, m_bAutoEnhance);
		int index = 0;
		for(int i = mid + 1; i < m_iSize; i++)
		{
			newPocket->m_DataList[index] = m_DataList[i];
			newPocket->m_iSize++;
			index++;
		}
		m_iSize = mid + 1;

		return newPocket;

	}


private:

	bool __Expand()
	{
		m_DataList.resize(m_iCapacity*2);
		m_iCapacity *= 2;
		return true;
	}

	bool __IndexInsideSize(int index)
	{
		return ((index >= 0) && (index < m_iSize));
	}

	bool __ShiftRight(int _fromIdx)
	{
		if(__IndexInsideSize(_fromIdx))
		{
			for(int i = m_iSize - 1; i >= _fromIdx; i--)
			{
				m_DataList[i + 1] = m_DataList[i];
			}
			return true;
		}
		return false;
	}

	bool __ShiftLeft(int _fromIdx)
	{
		if(__IndexInsideSize(_fromIdx))
		{
			for(int i = _fromIdx + 1; i < m_iSize; i++)
			{
				m_DataList[i - 1] = m_DataList[i];
			}
			return true;
		}
		return false;
	}

};



template<class T>
class IndexTreeNode
{
	typedef IndexTreeNode<T>                        _Class;
	typedef IndexPocket<T>                          _Pocket;

	template<class M>
	class ChildNode
	{
		typedef IndexTreeNode<M>                     _Node;
		int                                          m_iFrequency;
		_Node*                                       m_pNode;

	public:
		ChildNode():
	    m_iFrequency(0),
		m_pNode(0)
		{
		}

		ChildNode(_Node* node):
	    m_iFrequency(0),
		m_pNode(node)
		{
		}

		int& Frequency()                            {return m_iFrequency;}
		_Node*& Node()                              {return m_pNode;}

	};

	typedef ChildNode<T>                            _ChildNode;
	typedef IndexPocket<_ChildNode>                 _ChildList;



	union ChildUnion
	{
		_ChildList*                                 m_ChildList;
	    _Pocket*                                    m_pPocket;
	};

	ChildUnion                                      m_ChildUnion;
	bool                                            m_bLeaf;
	_Class*                                         m_pParent;
	int                                             m_iPosition;


public:
	IndexTreeNode(int ContainerSize, bool leafNode, _Class* parent = 0):
	m_bLeaf(leafNode),
	m_pParent(parent),
	m_iPosition(0)
	{
		if(m_bLeaf)
		{
			m_ChildUnion.m_pPocket = new _Pocket(ContainerSize);
		}
		else
		{
			m_ChildUnion.m_ChildList = new _ChildList(ContainerSize);
		}
	}

	IndexTreeNode(_ChildList* list, _Class* parent = 0):
	m_bLeaf(false),
	m_pParent(parent),
	m_iPosition(0)
	{
		m_ChildUnion.m_ChildList = list;
		for(int i = 0; i < list->Size(); i++)
		{
			_ChildNode* node = m_ChildUnion.m_ChildList->Value(i);
			if(node)
			{
				node->Node()->Parent() = this;
			}
		}
		__CalibrateFrequencyFrom(0);
	}

	IndexTreeNode(_Pocket* list, _Class* parent = 0):
	m_bLeaf(true),
	m_pParent(parent),
	m_iPosition(0)
	{
		m_ChildUnion.m_pPocket = list;
	}

	~IndexTreeNode()
	{
		if(m_bLeaf)
		{
			delete m_ChildUnion.m_pPocket;
		}
		else
		{
			delete m_ChildUnion.m_ChildList;
		}
	}



	bool IsLeaf()                      {return m_bLeaf;}
	_Class*& Parent()                  {return m_pParent;}
	int& Position()                    {return m_iPosition;}

	int Size()
	{
		if(!IsLeaf())
		{
			if(m_ChildUnion.m_ChildList->Size() == 0)
			{
				return 0;
			}
			return m_ChildUnion.m_ChildList->Back()->Frequency();
		}
		else
		{
			return m_ChildUnion.m_pPocket->Size();
		}
	}

	int ChildListSize()
	{
		if(IsLeaf())
		{
			return 0;
		}
		return m_ChildUnion.m_ChildList->Size();
	}


	bool Full()
	{
		if(IsLeaf())
		{
			return m_ChildUnion.m_pPocket->Full();
		}
		else
		{
			return m_ChildUnion.m_ChildList->Full();
		}
	}

	bool Empty()
	{
		if(IsLeaf())
		{
			return m_ChildUnion.m_pPocket->Empty();
		}
		else
		{
			return m_ChildUnion.m_ChildList->Empty();
		}
	}


	bool Insert(int index, T Data)
	{
		if(!IsLeaf())
		{
			return false;
		}
		if(m_ChildUnion.m_pPocket->Insert(index, Data))
		{
			__TellParentsAboutSizeChange();
			return true;
		}
		return false;
	}

	bool Insert(int index, IndexTreeNode<T>* node)
	{
		if(IsLeaf() || !node)
		{
			return false;
		}
		if(m_ChildUnion.m_ChildList->Insert(index, _ChildNode(node)))
		{
			node->Parent() = this;
			node->Position() = index;
			__CalibrateFrequencyFrom(index);
			__TellParentsAboutSizeChange();
			return true;
		}
		return false;
	}

	bool Pushback(T Data)
	{
		if(!IsLeaf())
		{
			return false;
		}
		if(m_ChildUnion.m_pPocket->Pushback(Data))
		{
			__TellParentsAboutSizeChange();
			return true;
		}
		return false;
	}

	bool Pushback(IndexTreeNode<T>* node)
	{
		if(IsLeaf())
		{
			return false;
		}
		if(m_ChildUnion.m_ChildList->Pushback(_ChildNode(node)))
		{
			node->Parent() = this;
			node->Position() = m_ChildUnion.m_ChildList->Size() - 1;
			__CalibrateFrequencyFrom(m_ChildUnion.m_ChildList->Size() - 1);
			__TellParentsAboutSizeChange();
			return true;
		}
		return false;
	}

	bool Pushfront(T Data)
	{
		if(!IsLeaf())
		{
			return false;
		}
		if(m_ChildUnion.m_pPocket->Pushfront(Data))
		{
			__TellParentsAboutSizeChange();
			return true;
		}
		return false;
	}

	bool Pushfront(IndexTreeNode<T>* node)
	{
		if(IsLeaf())
		{
			return false;
		}
		if(m_ChildUnion.m_ChildList->Pushfront(_ChildNode(node)))
		{
			node->Parent() = this;
			node->Position() = 0;
			__CalibrateFrequencyFrom(0);
			__TellParentsAboutSizeChange();
			return true;
		}
		return false;
	}

	bool Delete(int index)
	{
		bool bRet = false;
		if(IsLeaf())
		{
			bRet = __DeleteData(index);
		}
		else
		{
			bRet = __DeleteChild(index);
		}

		if(bRet)
		{
			__TellParentsAboutSizeChange();
		}
		return bRet;
	}

	IndexTreeNode<T>* Divide()
	{
		if(!IsLeaf())
		{
			_ChildList* list = m_ChildUnion.m_ChildList->Divide();
			if(list)
			{
				IndexTreeNode<T>* node =  new _Class(list);
				__CalibrateFrequencyFrom(0);
				__TellParentsAboutSizeChange();
				return node;

			}
			return 0;
		}
		else
		{
			_Pocket* list = m_ChildUnion.m_pPocket->Divide();
			if(list)
			{
				IndexTreeNode<T>* node =  new _Class(list);
				__TellParentsAboutSizeChange();
				return node;

			}
			return 0;
		}
	}


	bool Value(int index, T*& ret)
	{
		if(!IsLeaf())
		{
			return false;
		}

		T* tValue = m_ChildUnion.m_pPocket->Value(index);
		if(tValue)
		{
			ret = tValue;
			return true;
		}
		return false;
	}

	bool Value(int index, _Class*& ret)
	{
		if(IsLeaf())
		{
			return false;
		}

		_ChildNode* tValue = m_ChildUnion.m_ChildList->Value(index);
		if(tValue)
		{
			ret = tValue->Node();
			return true;
		}
		return false;
	}


	IndexTreeNode<T>* GetChild(int& absoluteIdx)
	{
		//return __GetChildNormal(absoluteIdx);
		return __GetChild(absoluteIdx);
	}

	



private:

	bool __CalibrateFrequencyFrom(int index)
	{
		if(!IsLeaf())
		{
			int prevIndex = 0;
			if(index > 0)
			{
				_ChildNode* cNode = m_ChildUnion.m_ChildList->Value(index - 1);
				if(!cNode)
				{
					return false;
				}
				prevIndex = cNode->Frequency();
			}
			for(int i = index; i < m_ChildUnion.m_ChildList->Size(); i++)
			{
				_ChildNode* cNode = m_ChildUnion.m_ChildList->Value(i);
				if(!cNode)
				{
					return false;
				}
				cNode->Frequency() = prevIndex + cNode->Node()->Size();
				cNode->Node()->Position() = i;
				prevIndex = cNode->Frequency();
			}
			return true;
		}
		else
		{
		}
		return false;
	}

	bool __TellParentsAboutSizeChange()
	{
		_Class* pItr = this;
		_Class* parent = pItr->Parent();

		while(parent)
		{
			parent->__CalibrateFrequencyFrom(pItr->Position());
			pItr = parent;
			parent = parent->Parent();
		}

		return true;
	}

	bool __DeleteData(int index)
	{
		if(!IsLeaf())
		{
			return false;
		}

		if(m_ChildUnion.m_pPocket->Delete(index))
		{
			__CalibrateFrequencyFrom(index);
			return true;
		}

		return false;
	}


	bool __DeleteChild(int index)
	{
		if(IsLeaf())
		{
			return false;
		}

		_ChildNode* node = m_ChildUnion.m_ChildList->Value(index);

		if( m_ChildUnion.m_ChildList->Delete(index))
		{
			__CalibrateFrequencyFrom(index);
			return true;
		}

		return false;
	}

	IndexTreeNode<T>* __DivideChild()
	{
		if(IsLeaf())
		{
			return 0;
		}

		_ChildList* cList = m_ChildUnion.m_ChildList->Divide();
		if(cList)
		{
			return new IndexTreeNode<T>(cList);
		}

		return 0;

	}

	_Class* __GetChildNormal(int& absoluteIdx)
	{
		int NumChildren = m_ChildUnion.m_ChildList->Size();
		int prev = 0;
		for(int i = 0; i < NumChildren; i++)
		{
			_ChildNode* xNode = m_ChildUnion.m_ChildList->Value(i);
			int xFrequency = xNode->Frequency();
			if(absoluteIdx < xFrequency)
			{
				absoluteIdx -= prev;
				return xNode->Node();
			}
			prev = xFrequency;
		}
		return m_ChildUnion.m_ChildList->Back()->Node();
	}

	_Class* __GetChild(int& absoluteIdx)
	{
		if(IsLeaf())
		{
			return 0;
		}

		int NumChildren = m_ChildUnion.m_ChildList->Size();
		//Border case 1: In case of only one element, there is no range
		//Range checking is done upwards
		if(NumChildren == 1)
		{
			return m_ChildUnion.m_ChildList->Front()->Node();
		}

		//If the absoluteIdx goes above the last element
		if(absoluteIdx >= m_ChildUnion.m_ChildList->Back()->Frequency())
		{
			absoluteIdx -= m_ChildUnion.m_ChildList->Value(NumChildren - 2)->Frequency();
			return m_ChildUnion.m_ChildList->Back()->Node();
		}
		//If the absoluteIdx is less than that of first element
		if(absoluteIdx < m_ChildUnion.m_ChildList->Front()->Frequency())
		{
			return m_ChildUnion.m_ChildList->Front()->Node();
		}

		//there are (n - 1) ranges. we have to check the range now

		int x = 0;
		int y = NumChildren - 1;

		_ChildNode* childNode = 0;
		_ChildNode* xNode = m_ChildUnion.m_ChildList->Value(x);
		_ChildNode* yNode = m_ChildUnion.m_ChildList->Value(y);
		if(!xNode || !yNode)
		{
			return 0; //Fail check case. Should not come here.
		}

		int xFrequency = xNode->Frequency();
		int yFrequency = yNode->Frequency();
		int mid = 0;
		int midFrequency = 0;
		_ChildNode* midNode = 0;

		while((y - x) > 1)
		{
			mid = (y + x)/2;

			midNode = m_ChildUnion.m_ChildList->Value(mid);
			if(!midNode)
			{
				//Fail check case. Should not come here.
				return 0;
			}
			midFrequency = midNode->Frequency();

			if(absoluteIdx < midFrequency)
			{
				y = mid;
				yNode = midNode;
				yFrequency = midFrequency;
			}
			else
			{
				x = mid;
				xNode = midNode;
				xFrequency = midFrequency;
			}
		}
		if(absoluteIdx < xFrequency)
		{
			if(x > 0)
			{
				absoluteIdx -= m_ChildUnion.m_ChildList->Value(x - 1)->Frequency();
			}
			childNode = xNode;
		}
		else
		{
			absoluteIdx -= m_ChildUnion.m_ChildList->Value(y - 1)->Frequency();
			childNode = yNode;
		}
		return childNode->Node();

	}
};


template<class T>
class CIndexTree
{
	typedef IndexTreeNode<T>           _Node;
	typedef std::stack<_Node*>         _StackNode;
	_Node*                             m_pHead;
	int                                m_iPocketSize;
	int                                m_iOrder;
public:
	CIndexTree(int pocketSize, int Order):
    m_pHead(new _Node(pocketSize,true)),
	m_iPocketSize(pocketSize),
	m_iOrder(Order)
	{
	}

	~CIndexTree()
	{
		__DeleteAll(false);
	}

	int Size()
	{
		if(m_pHead)
		{
			return m_pHead->Size();
		}
		return 0;
	}

	bool Clear()
	{
		return __DeleteAll();
	}

	bool Insert(int index, T Data)
	{
		if(index >= Size())
		{
			return false;
		}

		_Node* pItr = m_pHead;

		//we will find the leaf node to enter
		pItr = __GetToLeafNode(index);
		if(!pItr)
		{
			cout<<"Failed to insert"<<endl;
			return false;
		}

		if(!pItr->Full())
		{
			if(pItr->Insert(index, Data))
			{
				return true;
			}
			return false;

		}

		if(__RearrangeAndInsert(pItr, index, Data))
		{
			return true;
		}


		return false;

	}

	bool Pushback(T Data)
	{
		_Node* pItr = m_pHead;
		//we will find the leaf node to enter
		int index = Size() - 1;
		pItr = __GetToLeafNode(index);
		if(!pItr)
		{
			cout<<"Failed to Pushback"<<endl;
			return false;
		}

		if(!pItr->Full())
		{
			if(pItr->Pushback(Data))
			{
				return true;
			}
			return false;

		}

		if(__RearrangeAndInsert(pItr, index + 1, Data))
		{
			return true;
		}
		return false;
	}

	bool Delete(int index)
	{
		_Node* pItr = __GetToLeafNode(index);
		if(!pItr)
		{
			cout<<"Failed to delete"<<endl;
			return false;
		}

		if(!pItr->Delete(index))
		{
			return false;
		}

		if(__RearrangeAndDelete(pItr))
		{
			return true;
		}

		return false;

	}

	bool Popback()
	{
		return Delete(Size() - 1);
	}

	bool Value(int index, T*& val)
	{
		_Node* pItr = __GetToLeafNode(index);
		if(!pItr)
		{
			return false;
		}
		if(!pItr->IsLeaf())
		{
			return false;
		}
		return pItr->Value(index, val);
	}

private:
	_Node* __GetToLeafNode(int& index)
	{
		_Node* pItr = m_pHead;
		_Node* pChild = 0;
		while(!pItr->IsLeaf())
		{
			pChild = pItr->GetChild(index);
			if(index < 0 )
			{
				cout<<"Index is negative"<<endl;
				return 0;
			}
			if(pChild)
			{
				pItr = pChild;
			}
		}
		return pItr;
	}

	bool __RearrangeAndInsert(_Node* pItr, int index, T Data)
	{
		if(!pItr)
		{
			return false;
		}
		if(!pItr->IsLeaf())
		{
			return false;
		}

		_Node* sibling = pItr->Divide();
		_Node* Uncle = 0;
		_Node* parent = 0;
		_Node* child = 0;
		_Node* brother = 0;

		if(pItr->Size() >= index)
		{
			if(!pItr->Insert(index, Data))
			{
				return false;
			}
		}
		else if(sibling->Size() + pItr->Size() >= index)
		{
			if(!sibling->Insert(index - pItr->Size(), Data))
			{
				return false;
			}
		}
		else
		{
			return false;
		}

		while(pItr && sibling)
		{
			parent = pItr->Parent();
			if(parent)
			{
				if(parent->Full())
				{
					//bifurcate
					Uncle = parent->Divide();
					if(pItr->Parent() == parent)
					{
						if(!parent->Insert(pItr->Position() + 1, sibling))
						{
							return false;
						}
					}
					else// if(pItr->Parent() == Uncle)
					{
						if(!Uncle->Insert(pItr->Position() + 1, sibling))
						{
							return false;
						}
					}
					child = pItr;
					pItr = parent;
					brother = sibling;
					sibling = Uncle;
				}
				else
				{
					if(!parent->Insert(pItr->Position() + 1, sibling))
					{
						return false;
					}
					return true;
				}
			}
			else
			{
				child = pItr;
				pItr = parent;
				brother = sibling;
				sibling = Uncle;
				break;
			}

		}
		if(!pItr)
		{
			child->Parent() = new _Node(m_iOrder, false);
			m_pHead = child->Parent();
			m_pHead->Pushback(child);
			m_pHead->Pushback(brother);
			return true;
		}
		return false;
	}

	bool __RearrangeAndDelete(_Node* pItr)
	{
		if(!pItr)
		{
			return false;
		}
		if(!pItr->IsLeaf())
		{
			return false;
		}

		_Node* parent = 0;
		while(pItr)
		{
			if(!pItr->Empty())
			{
				return true;
			}
			parent = pItr->Parent();
			if(parent)
			{
				parent->Delete(pItr->Position());
			}
			pItr = parent;
		}
		return true;
	}

	bool __DeleteAll(bool recreateHead = true)
	{
		_StackNode sn;
		sn.push(m_pHead);

		_Node* child = 0;
		_Node* currNode = 0;
		while(sn.size() > 0)
		{
			currNode = sn.top();
			sn.pop();

			int childCount = currNode->ChildListSize();
			for(int i = 0; i < childCount; i++)
			{
				if(currNode->Value(i, child))
				{
					if(child)
					{
						sn.push(child);
					}
				}
			}
			delete currNode;
		}
		if(recreateHead)
		{
			m_pHead = new _Node(m_iPocketSize, true);
		}
		else
		{
			m_pHead = 0;
		}
		return true;
	}

};

}; //namespace IndexTreeSpace
#endif
