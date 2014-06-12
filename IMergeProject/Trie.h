#ifndef _C_TRIE_H__
#define _C_TRIE_H__

/*
#include <set>
#include <map>
#include <string>

class CTrieNodeItr;

class CTrieNode
{ 
	typedef std::map<char, CTrieNode*> Element;
	typedef Element::iterator EleItr;
	typedef std::pair<char, CTrieNode*> InsPair;
	Element    mElement;
	bool       HasAPhi;
	int        NumChilderen;
	friend class CTrieNodeItr;
public:
	CTrieNode() : HasAPhi(false), NumChilderen(0) {}
	~CTrieNode() {}
	CTrieNode* Next(char x);
	void Insert(char x);
	void SetEnd()                      { HasAPhi = true;}
	bool IsEnd()                       { return HasAPhi;}
	int Children()                     { return NumChilderen;}
	bool IsChild(char c)               { return (mElement.find(c) != mElement.end()); }
private:
	bool AtEnd(EleItr itr)
	{
		return (itr == mElement.end());
	}
	bool AtBegin(EleItr itr)
	{
		return (itr == mElement.begin());
	}
};

class CTrieNodeItr
{
	CTrieNode*    mNode;
	EleItr        mItr;
	int           mCount;
public:
	CTrieNodeItr(CTrieNode* node):
	mNode(node),
	mCount(0)
	{
		Reset();
	}
	void Reset()
	{
		if(mNode)
		{
			mItr = mNode.begin();
		}
	}
	void operator ++()
	{
		if(mElement.end() != mItr)
		{
			mItr++;
			mCount++;
		}
	}
	void operator ++(int)
	{
		if(mElement.end() != mItr)
		{
			mItr++;
			mCount++;
		}
	}
	void operator --()
	{
		if(mCount > 0)
		{
			mItr--;
			mCount--;
		}
	}
	void operator --(int)
	{
		if(mCount > 0)
		{
			mItr--;
			mCount--;
		}
	}
	CTrieNode* Value()
	{
		return *mItr;
	}
	bool IsEnd()
	{
		if(!mNode)
			return false;

		return mNode->AtEnd(mItr);
	}
	bool IsBegin()
	{
		if(!mNode)
			return false;

		return mNode->AtBegin(mItr);
	}
};


class CTrie
{
	CTrieNode  mHead;
	int        mDepth;
public:
	CTrie(void);
	~CTrie(void);
	void Insert(std::string* str, int count);
	void DeleteAll();
	std::vector<std::string> GetMatches(std::string& s);
private:
	void InsertAString(std::string& str);
};

*/

#endif