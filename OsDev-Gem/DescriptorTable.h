//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : DescriptorTable.h
// Date de création : Octobre 2015
// Description : Une classe générique
// qui représente une tableau de descripteurs
//----------------------------------------------------

#ifndef DESCRIPTOR_TABLE
#define DESCRIPTOR_TABLE

#include "Types.h"
#include "Defines.h"

template <class T>
class DescriptorTable
{
public:
	DescriptorTable()
	{
	}

	virtual ~DescriptorTable()
	{
		m_aDescriptors.Clear();
	}

	void AddSegmentDescriptor(T descriptor)
	{
		m_aDescriptors.PushBack(descriptor);
	}

	size_t GetDescriptorCount()
	{
		return m_aDescriptors.Size();
	}

	void Flush()
	{	
		m_aDescriptors.Clear();
	}

	T GetFirstEntry()
	{
		AssertMsg(m_aDescriptors.Size() > 0, "Le tableau des descripteurs est vide !");

		return m_aDescriptors[0];
	}
	
private:
	Core::Array<T> m_aDescriptors;
};

#endif