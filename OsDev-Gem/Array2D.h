//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Array2D.h
// Date de création : Octobre 2015
// Description : Cette classe permet de gérer
// un tableau à deux dimensions (utilisé pour la matrice
// de texte VideoMemory)
//----------------------------------------------------

#ifndef ARRAY_2D_H
#define ARRAY_2D_H
 
template<typename T>
class Array2D : public Array<T>
{
public:
    Array2D(uint32 iWidth = 0, uint32 iHeight = 0) :
	Array(iWidth * iHeight),
    m_iWidth(iWidth),
    m_iHeight(iHeight,
    {
    }
 
    void Resize(uint32 iWidth, uint32 iHeight)
    {
        m_iWidth = iWidth;
        m_iHeight = iHeight;

        Reallocate(iWidth * iHeight);
    }

    inline T& operator[] (uint32 x, uint32 y)
    {
		AssertMsg(((y * m_iWidth) + x) < Size(), "Accès indice de tableau or marge !");
		
        return m_array[ (y * m_iWidth) + x ];
    }
 
    inline T& operator[] (uint32 pos)
    {
        return m_array[pos];
    }
 
    inline const T& operator[] (uint32 x, uint32 y) const
    {
		AssertMsg(((y * m_iWidth) + x) < Size(), "Accès indice de tableau or marge !");
		
        return m_array[ (y * m_iWidth) + x ];
    }
 
    inline const T& operator[] (uint32 pos) const
    {
        return m_array[pos];
    }   
 
    inline int GetWidth() const { return m_iWidth; }
	inline int GetHeight() const { return m_iHeight; }
	inline int GetSize() const { return m_iWidth * m_iHeight; }

	void RemoveByIndices(uint32 iPosX, uint32 iPosY)
	{
		AssertMsg(((iPosY * m_iWidth) + iPosX) < Size(), "Accès indice de tableau hors marge !");
		
		T elem = m_array[ (iPosY * m_iWidth) + iPosX ];

		Remove(elem);

		// On recalcule les dimensions
		if (iPosX >= 1)
		{
			m_iHeight--;
		}
		else if (iPosY >= 1)
		{
			m_iWidth--;
		}
	}

private:
    uint32 m_iWidth; // Largeur
    uint32 m_iHeight; // Hauteur
};
 
#endif