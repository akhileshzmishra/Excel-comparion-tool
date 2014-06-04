#ifndef _XLLCS_H_
#define _XLLCS_H_

#include <vector>

typedef std::vector<std::pair<int, int> > RETURNTYPE;

template<class M, class N>
class XLLCS
{
	int m_iLengthA;
	int m_iLengthB;
	M   m_fPredicate;
	N&  m_ProviderOne;
	N&  m_ProviderTwo;

public:
	XLLCS(int len1, int len2, N& provider1, N& provider2):
	m_iLengthA(len1), 
	m_iLengthB(len2),
	m_ProviderOne(provider1),
	m_ProviderTwo(provider2)
	{
	}
	void SetProviders(N& provider1, N& provider2)
	{
		m_ProviderOne = provider1;
	    m_ProviderTwo = provider2;
	}
	bool Compare(RETURNTYPE& returnValue)
	{
		if(m_iLengthA <= 0 || m_iLengthB <= 0)
			return false;

		int** matrix;
		int length = 0;
		CreateMatrix(&matrix, m_iLengthA + 1, m_iLengthB + 1);

		int start = 0;
		for(int i = 0; i <= m_iLengthA ; i++)
		{
			matrix[i][0] = 0;
		}
		for(int i = 0; i <= m_iLengthB; i++)
		{
			matrix[0][i] = 0;
		}
		for(int i = 1; i < m_iLengthA + 1; i++)
		{
			for(int j = 1; j < m_iLengthB + 1; j++)
			{
				if(m_fPredicate(m_ProviderOne[i - 1], m_ProviderTwo[j - 1]))
				{
					matrix[i][j] = matrix[i - 1][j -1] + 1;
				}
				else
				{
					if(matrix[i - 1][j] > matrix[i][j - 1])
					{
						matrix[i][j] = matrix[i - 1][j];
					}
					else
					{
						matrix[i][j] = matrix[i][j - 1];
					}
				}
			}
			cout<<endl;
		}
		length = matrix[m_iLengthA][m_iLengthB];

		returnValue.resize(length);

		int x = length - 1;
		int k = m_iLengthA - 1;
		int j = m_iLengthB - 1;
		while(x >= 0)
		{
			if(m_fPredicate(m_ProviderOne[k], m_ProviderTwo[j]))
			{
				returnValue[x].first = k;
				returnValue[x].second = j;
				x--;
				k--;
				j--;
			}
			else if(matrix[k + 1][j] > matrix[k][j + 1])
			{
				j--;
			}
			else
			{
				k--;
			}
		}
		DeleteMatrix(&matrix, m_iLengthA + 1);
		return true;
	}
private:

	void CreateMatrix(int*** mat, int r, int c)
	{
		*mat = new int*[r];
		for(int i = 0; i < r; i++)
		{
			(*mat)[i] = new int[c];
		}
	}
	void DeleteMatrix(int*** mat, int r)
	{
		for(int i = 0; i < r; i++)
		{
			delete[] (*mat)[i];
		}
		delete *mat;
	}
};











#endif