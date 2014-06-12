#include "StdAfx.h"
#include "Trie.h"
#include <queue>
#include <stack>

/*

CTrieNode* CTrieNode::Next(char x)
{
	EleItr itr = mElement.find(x);
	if(itr == mElement.end())
	{
		return 0;
	}
	return *itr;
}
void CTrieNode::Insert(char x)
{
	mElement.insert(InsPair(x, new CTrieNode()));
	NumChilderen++;
}




CTrie::CTrie(void):
mHead(0),
mDepth(0)
{
}

CTrie::~CTrie(void)
{
}


void CTrie::Insert(std::string* str, int count)
{
	if(!str|| (count <= 0)
	{
		return;
	}

	for(int i = 0; i < count; i++)
	{
		InsertAString(str[i]);
	}


}

void CTrie::InsertAString(std::string& str)
{
	CTrieNode*& refItr = &mHead;
	int x = 0;
	while(x < str.size())
	{
		if(refItr)
		{
			if(refItr->IsChild(str[x]))
			{
				refItr = refItr->Next(x);
			}
			else
			{
				refItr->Insert(x);
				refItr = refItr->Next(x);
			}
		}
		else
		{
			refItr = new CTrieNode();
			refItr = refItr->Next(x);
		}
		x++;
	}
}

void CTrie::DeleteAll()
{
	std::stack<CTrieNode*> Stack;
	Stack.push(&mHead);
	int level = 0;
	while(Stack.size() > 0)
	{
		CTrieNode* node = Stack.top();
		Stack.pop();
	    CTrieNodeItr itr(node);
		while(!itr.IsEnd())
		{
			Stack.push(itr.Value());
			itr++;
		}
		if(level > 0)
		{
			delete node;
		}
		level++;
	}
}



std::vector<std::string> CTrie::GetMatches(std::string& s)
{
	CTrieNode*& refItr = &mHead;
	std::vector<std::string> Answer;
	int x = 0;
	while(x < s.size())
	{
		if(refItr)
		{
			if(refItr->IsChild(str[x]))
			{
				refItr = refItr->Next(x);
			}
			else
			{
				return Answer;
			}
		}
		else
		{
			return Answer;
		}
		x++;
	}

	std::stack<CTrieNode*> Stack;
	Stack.push(refItr);
	char Val[216];
	memset(Val, 0, 216);
	for(int i = 0; i < s.size(); i++)
	{
		Val[i] = s[i];
	}
	int i = 0;
	while(Stack.size() > 0)
	{
		CTrieNodeItr* node = Stack.top();
		Stack.pop();
		CTrieNodeItr itr(node);
		while(!itr.IsEnd())
		{
			Stack.push(itr.Value());
			itr++;
		}
		if(node->Value()->Children() == 0)
		{
		}
	}

}

*/