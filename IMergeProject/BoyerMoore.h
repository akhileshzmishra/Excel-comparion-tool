#ifndef __BOYER_MOORE__H__
#define __BOYER_MOORE__H__

#include <vector>

#pragma once

template<class T>
class BoyerMoore
{
	typedef std::vector<int> List;
	List               m_Table1;
	List               m_Table2;
	const T*           m_Sub;
	const T*           m_Src;
	int                m_LengthSrc;
	int                m_LengthSub;
	bool               m_bTable1Prepared;
	bool               m_bTable2Prepared;
	int                m_index;
public:
	static const int No_Index = -1;

	BoyerMoore(void):
	m_Sub(0), 
	m_Src(0), 
	m_LengthSrc(0),
	m_LengthSub(0),
	m_bTable1Prepared(false), 
	m_bTable2Prepared(false), 
	m_index(0)
	{
	}

	~BoyerMoore(void)
	{
	}

	int FindIndex(T* src, int srclen, T* sub, int sublen)
	{
		Reset();
		if(!src || !sub)
		{
			return No_Index;
		}

		m_LengthSrc = srclen;
		m_LengthSub = sublen;
		m_Src       = src;
		m_Sub       = sub;

		return t_FindIndex(true);
	}

	int FindIndex(const T* src, int srclen)   //sub has already been given
	{
		if(!src || !m_Sub || (srclen <= 0) || (m_LengthSub <= 0))
		{
			return No_Index;
		}

		m_LengthSrc = srclen;
		m_Src       = src;

		return t_FindIndex(false);
	}

	void SetSubString(const T* sub, int sublen)
	{
		Reset();
		CreateTables(sub, sublen);
	}

	

	void Reset();
	
private:
	void CreateTables(const T* sub, int sublen)
	{
		if(!sub || (sublen <= 0))
		{
			return;
		}

		m_LengthSub = sublen;
		m_Sub       = sub;
		t_CreateTableOne();
		t_CreateTableTwo();
	}

	void t_CreateTableOne();
	void t_CreateTableTwo();
	bool t_AreTablePrepared()
	{
		return (m_bTable1Prepared & m_bTable2Prepared);
	}
	int t_FindIndex(bool createtable = true);
	
};


template<class T>
void BoyerMoore<T>::t_CreateTableOne()
{
	if(t_AreTablePrepared())
	{
		return;
	}
	m_Table1.clear();
	int length = m_LengthSub;
	if(length <= 0)
	{
		return;
	}
	m_Table1.resize(length, 0);
	for(int i = length - 1; i > 0; i--)
	{
		for(int j = i - 1; j >= 0; j--)
		{
			if(m_Sub[i] == m_Sub[j])
			{
				m_Table1[i - 1] =  i - j;
				break;
			}
		}
	}
	for(int i = 0; i < length; i++)
	{
		if(m_Table1[i] == 0)
			m_Table1[i] = length - i;
	}
	m_bTable1Prepared = true;
}

template<class T>
void BoyerMoore<T>::t_CreateTableTwo()
{
	if(t_AreTablePrepared())
	{
		return;
	}
	m_Table2.clear();
	int length = m_LengthSub;
	if(length <= 0)
	{
		return;
	}
	m_Table2.resize(length, 0);
	std::vector<int> EndTable;
	for(int i = 0; i < length; i++)
	{
		if(m_Sub[i] == m_Sub[length - 1])
		{
			EndTable.push_back(i);
		}
	}

	for(int i = (int)((int)EndTable.size() - 1); i > 0; i--)
	{
		for(int j = i - 1; j >= 0; j--)
		{
			int x = EndTable[i];
			int y = EndTable[j];
			while((m_Sub[x] == m_Sub[y]) && (y >= 0))
			{
				if(m_Table2[x - 1] == 0)
				{
					m_Table2[x - 1] = x - y;
				}
				x--;
				y--;
			}
		}
	}
	for(int i = 0; i < length; i++)
	{
		if(m_Table2[i] == 0)
		{
			m_Table2[i] = length - i - 1;
		}
	}
	m_Table2[length - 1] = 1;
	m_bTable2Prepared = true; 
}


template<class T>
void BoyerMoore<T>::Reset()
{
	m_Sub = 0;
	m_Src = 0;
	m_bTable1Prepared = false;
	m_bTable2Prepared = false;
	m_Table1.clear();
	m_Table2.clear();
}

template<class T>
int BoyerMoore<T>::t_FindIndex(bool createtable)
{
	int currIndex = No_Index;
	int srclen = m_LengthSrc;
	int sublen = m_LengthSub;
	if(sublen > srclen)
	{
		m_index = No_Index;
		return currIndex;
	}
	if(createtable)
	{
		t_CreateTableOne();
		t_CreateTableTwo();
	}
	if(!t_AreTablePrepared())
	{
		return No_Index;
	}

	int i = sublen - 1 + m_index;
	int j = sublen - 1;
	int curr = i;

	while(i < srclen)
	{
		while(j >= 0)
		{
			if(m_Src[i] == m_Sub[j])
			{
				i--;
				j--;
			}
			else
			{
				break;
			}
		}
		if(j >= 0)
		{
			int tab1 = m_Table1[j];
			int tab2 = m_Table2[j];
			int max = tab2;
			if(tab1 > tab2)
				max = tab1;

			if(max == 0)
				max = 1;
			//cout<<"jumps "<<max<<endl;
			curr += max;
			i = curr;
			j = sublen - 1;
		}
		else
		{
			break;
		}
		
	}
	if(j < 0)
	{
		currIndex = i + 1;
		m_index = currIndex;
	}
	else
	{
		m_index = No_Index;
	}
	return m_index;
}




#endif