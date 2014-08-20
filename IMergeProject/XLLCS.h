#ifndef _XLLCS_H_
#define _XLLCS_H_

#include <vector>

typedef std::vector<std::pair<int, int> > RETURNTYPE;

template<class M/*Comparison mechanism*/, class N /* Data provider class */>
class XLLCS
{
	int m_iLengthA;
	int m_iLengthB;
	M   m_fPredicate; /* Comparison mechanism            */
	N&  m_ProviderOne;/* Data provider class for first   */
	N&  m_ProviderTwo;/* Data provider class for second  */
	int** m_Matrix;
	bool  m_bMatrixCreated;
public:
	XLLCS(int len1, int len2, N& provider1, N& provider2):
	m_iLengthA(len1), 
	m_iLengthB(len2),
	m_ProviderOne(provider1),
	m_ProviderTwo(provider2),
	m_bMatrixCreated(false)
	{
	}

	XLLCS(N& provider1, N& provider2):
	m_iLengthA(provider1.Size()), 
	m_iLengthB(provider2.Size()),
	m_ProviderOne(provider1),
	m_ProviderTwo(provider2),
	m_bMatrixCreated(false)
	{
	}

	void SetLength(int l1, int l2)
	{
		m_iLengthA = l1;
		m_iLengthB = l2;
	}

	~XLLCS()
	{
		if(m_bMatrixCreated)
		{
			DeleteMatrix(&m_Matrix, m_iLengthA + 1);
		}
	}

	void SetProviders(N& provider1, N& provider2)
	{
		m_ProviderOne = provider1;
	    m_ProviderTwo = provider2;
	}

	bool CompareAPart(RETURNTYPE& returnValue, int s1, int s2)
	{
		if(s1 <= 0 || s1 >= m_iLengthA)
		{
			return false;
		}

		if(s2 <= 0 || s2 >= m_iLengthB)
		{
			return false;
		}
		int length = 0;

		if(!m_bMatrixCreated)
		{
			CreateMatrix(&m_Matrix, m_iLengthA + 1, m_iLengthB + 1);
			for(int i = 0; i <= m_iLengthA ; i++)
			{
				m_Matrix[i][0] = 0;
			}
			for(int i = 0; i <= m_iLengthB; i++)
			{
				m_Matrix[0][i] = 0;
			}
		}


		for(int i = 1; i < s1 + 1; i++)
		{
			for(int j = 1; j < s2 + 1; j++)
			{
				if(m_fPredicate(m_ProviderOne[i - 1], m_ProviderTwo[j - 1]))
				{
					m_Matrix[i][j] = m_Matrix[i - 1][j -1] + 1;
				}
				else
				{
					if(m_Matrix[i - 1][j] > m_Matrix[i][j - 1])
					{
						m_Matrix[i][j] = m_Matrix[i - 1][j];
					}
					else
					{
						m_Matrix[i][j] = m_Matrix[i][j - 1];
					}
				}
			}
		}
		length = m_Matrix[s1][s2];

		returnValue.resize(length);

		int x = length - 1;
		int k = s1 - 1;
		int j = s2 - 1;
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
			else if(m_Matrix[k + 1][j] > m_Matrix[k][j + 1])
			{
				j--;
			}
			else
			{
				k--;
			}
		}

		return true;
	}

	bool Compare(RETURNTYPE& returnValue)
	{
		if(m_iLengthA <= 0 || m_iLengthB <= 0)
			return false;

		int** matrix;
		int length = 0;
		CreateMatrix(&m_Matrix, m_iLengthA + 1, m_iLengthB + 1);

		for(int i = 0; i <= m_iLengthA ; i++)
		{
			m_Matrix[i][0] = 0;
		}
		for(int i = 0; i <= m_iLengthB; i++)
		{
			m_Matrix[0][i] = 0;
		}
		for(int i = 1; i < m_iLengthA + 1; i++)
		{
			for(int j = 1; j < m_iLengthB + 1; j++)
			{
				if(m_fPredicate(m_ProviderOne[i - 1], m_ProviderTwo[j - 1]))
				{
					m_Matrix[i][j] = m_Matrix[i - 1][j -1] + 1;
				}
				else
				{
					if(m_Matrix[i - 1][j] > m_Matrix[i][j - 1])
					{
						m_Matrix[i][j] = m_Matrix[i - 1][j];
					}
					else
					{
						m_Matrix[i][j] = m_Matrix[i][j - 1];
					}
				}
			}
		}
		length = m_Matrix[m_iLengthA][m_iLengthB];

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
			else if(m_Matrix[k + 1][j] > m_Matrix[k][j + 1])
			{
				j--;
			}
			else
			{
				k--;
			}
		}
		DeleteMatrix(&m_Matrix, m_iLengthA + 1);
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
		m_bMatrixCreated = true;
	}
	void DeleteMatrix(int*** mat, int r)
	{
		for(int i = 0; i < r; i++)
		{
			delete[] (*mat)[i];
		}
		delete *mat;
		m_bMatrixCreated = false;
	}
};











#endif