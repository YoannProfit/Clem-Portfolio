//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Map.h
// Date de création : Octobre 2015
// Description : Une classe pour gérer un tableau associatif
// (comme pour la std::map du C++),
//
// Implémentée en tant que liste chainée
//
// Niveau performance en complexité algorithmtique
// ce n'est pas la meilleur solution mais j'ai préféré
// une approche par un algorithme simple à comprendre 
// plutôt que prendre celui de l'arbre bicolor
//----------------------------------------------------
#ifndef MAP_H
#define MAP_H

#include "Types.h"
#include "Array.h"

namespace Core
{	
	template <class KeyType, class ValueType>
	class Map
	{
		struct Element
		{
			KeyType key;
			ValueType value;
			Element* pNextElement;
		};	
		
	public:	
		Map() :
		m_aLinkedList(nullptr),
		m_iSize(0);
		{
			
		}

		~Map()
		{
			Clear();
		}	
		
		class Iterator
		{
		public:

			Iterator(Element* pPosition) :
			{
				m_pCur = pPosition;
			}
				
			// Une liste chainée finit toujours par 0
			bool AtEnd() const
			{
				return m_pCur == nullptr;
			}
			
			bool operator==(const Iterator& src)
			{
				return (m_pCur == src.m_pCur);
			}
			
			void operator++()
			{
				if (m_pCur != nullptr)
				{
					m_pCur += sizeof(Element);
				}
			}

			void operator--()
			{
				if (m_pCur != m_pStart)
				{
					m_pCur -= sizeof(Element);
				}
			}

			Element* operator->()
			{
				return m_pCur;
			}

			Element& operator*()
			{
				return *m_pCur;
			}
			
			KeyType GetKey()
			{
				return m_pCur->key;		
			}
			
			ValueType GetValue()
			{
				return m_pCur->value;	
			}
				
		private:
			Element* m_pCur;
		};
		
		class DirectAccess
		{
			friend class Map<KeyType, ValueType>;

		public:
			void operator=(const ValueType& value)
			{
				m_map.Set(m_key, value);
			}

		private:
			DirectAccess(Map& map, const KeyType& key) : 
			m_map(map), 
			m_key(key) 
			{}
			
		private:
			Map& m_map;
			const KeyType& m_key;
		};
			
		DirectAccess operator[](const KeyType& k)
		{
			return DirectAccess(*this, k);
		}	
		
		bool Empty() const
		{
			return m_iSize == 0;
		}	

		uint32 Size() const
		{
			return m_iSize;
		}	
		
		void Clear(bool bDestroy = false)
		{
			Iterator it;
			
			for (it = Begin(); it != End(); it++)
			{
				Element* pElem = *(it);
				
				delete pElem;
			}
			
			m_aLinkedList = nullptr;
			
			m_iSize = 0;
		}
		
		bool Remove(const KeyType& k)
		{
			Iterator it;
			
			uint32 iCount = 0;
			
			for (it = Begin(); it != End(); it++)
			{
				Element* pElem = *(it);
				
				if (pElem->key == k)
				{
					for (uint32 j = iCount; j < m_iSize - 1; j++)
					{
						m_aLinkedList[j] = m_aLinkedList[j + 1];
					}
					
					delete pElem;
						
					m_iSize--;
					
					return true;
				}
				
				iCount++;
			}
			
			return false;
		}	
		
		bool HasKey(const KeyType& k)
		{
			Iterator it;
			
			for (it = Begin(); it != End(); it++)
			{
				Element* pElem = *(it);
				
				if (pElem->pKey == k)
				{
					return true;
				}
			}
			
			return false;
		}
		
		// On ajoute un élément à la fin de la liste chainée
		void Set(const KeyType& k, const ValueType& v)
		{
			if (!HasKey(k))
			{
				Element* pElem = new Element();
				pElem->key = k;
				pElem->value = v;
				
				if (Size() != 0)
				{
					m_aLinkedList->pNextElement = pElem;
				}
				else
				{
					m_aLinkedList-> = pElem;
				}
				
				pElem->pNextElement = nullptr;			
				
				m_iSize++;
			}
		}
		
		Iterator Begin()
		{
			return Iterator(m_aLinkedList[0]);
		}
			
		Iterator End()
		{
			return Iterator(m_aLinkedList[ Size() ]);
		}
			
	private:
		Element** m_aLinkedList;
		size_t m_iSize;
	};
}

#endif