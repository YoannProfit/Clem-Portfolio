//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Array.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer un tableau dynamique
// (comme la std::vector de la STL en C++)
//
//
// Sources : http://codereview.stackexchange.com/questions/60484/stl-vector-implementation
// et code source de Irrlicht 1.5
//----------------------------------------------------

#ifndef ARRAY_H
#define ARRAY_H

#include "Types.h"

namespace Core
{
	template <class T>
	class Array
	{
	public:
		typedef T* Iterator;

		Array() : 
		
			m_aData(nullptr), 
			m_iAllocated(0), 
			m_iUsed(0)
			
		{
		}

		Array(size_t iSize) : 
		
			m_aData(nullptr), 
			m_iAllocated(iSize), 
			m_iUsed(0)
			
		{
			Reallocate(iSize);
		}	
		
		~Array()
		{
			Clear();
		}
		
		void Reallocate(size_t iSize)
		{
			T* pNewBuffer = new T[iSize];

			for (size_t i = 0; i < m_iUsed; i++)
			{
				pNewBuffer[i] = m_aData[i];
			}
			
			m_iAllocated = iSize;
			
			delete[] m_aData;
			m_aData = pNewBuffer;
		}	
		
		void Clear()
		{
			SAFE_DELETE_ARRAY(m_aData);

			m_iUsed = 0;
			m_iAllocated = 0;
		}
		
		void SetUsed(size_t iUsedNow)
		{
			if (m_iAllocated < iUsedNow)
			{
				Reallocate(iUsedNow);
			}
			
			m_iUsed = iUsedNow;
		}
		
		T& operator [](uint32 index)
		{
			DEBUG_BREAK_IF(index >= m_iUsed) // access violation
			
			return m_aData[index];
		}

		const T& operator [](uint32 index) const
		{
			DEBUG_BREAK_IF(index >= m_iUsed) // access violation

			return m_aData[index];
		}

		T& GetLast()
		{
			DEBUG_BREAK_IF(!m_iUsed) // access violation

			return m_aData[m_iUsed-1];
		}

		const T& GetLast() const
		{
			DEBUG_BREAK_IF(!m_iUsed) // access violation

			return m_aData[m_iUsed-1];
		}
	
		T* GetPointer()
		{
			return m_aData;
		}

		const T* ConstPointer() const
		{
			return m_aData;
		}

		size_t Size() const
		{
			return m_iUsed;
		}
		
		size_t Capacity()
		{
			return m_iAllocated;
		}
		
		bool Empty() const
		{
			return m_iUsed == 0;
		}
		
		Iterator Begin()
		{
			return m_aData;
		}
		
		Iterator End()
		{
			return m_aData + Size(); // -1 ??
		}
		
		T& Front()
		{
			return m_aData[0];
		}
		
		T& Back()
		{
			return m_aData[m_iUsed - 1];
		}
		
		void PushBack(const T& value)
		{
			if (m_iUsed >= m_iAllocated) 
			{
				Reallocate(++m_iAllocated);
			}
			
			m_aData[m_iUsed++] = value;
		}

		void PopFront()
		{
			T firstElem = m_aData[0];
		
			Remove(firstElem);
		}	

		void PopBack()
		{
			T lastElem = m_aData[m_iUsed - 1];
			
			/* On détruit le dernier objet en appelant
			   directement le constructeur */
			lastElem.~T();

			m_iUsed--;
		}	
		
		
		bool Remove(T elem)
		{
			Iterator it;
			
			uint32 iCount = 0;
			
			/* On parcourt le tableau jusqu'à ce que l'on trouve
			   cet élément à effacer */
			for (it = Begin(); it != End(); it++)
			{
				T currElem = *(it);
				
				if (elem == currElem)
				{
					currElem.~T();
					
					// On reconstruit le tableau en le scindant en deux parties
					T* pNewBuffer = new T[--m_iUsed];

					// 1ère partie
					for (uint32 i = 0; i < iCount; i++)
					{
						pNewBuffer[i] = m_aData[i];
					}
					
					// 2ème partie
					for (uint32 i = iCount+1; i < m_iUsed; i++)
					{
						pNewBuffer[i] = m_aData[i];
					}					
					
					delete[] m_aData;
					m_aData = pNewBuffer;
					
					break;
				}
				
				iCount++;
			}
		}

	private:
		T* m_aData;
		size_t m_iAllocated;
		size_t m_iUsed;
	};
}

#endif