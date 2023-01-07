//----------------------------------------------------
// Auteur : Cl�ment Profit
// Nom du fichier : Buffer.h
// Date de cr�ation : Octobre 2015
// Description : Cette classe permet de g�rer un buffer
// (un tampon de donn�e)
//---------------------------------------------------

#ifndef BUFFER_H
#define BUFFER_H

class Buffer
{
public:
	Buffer(char* n,size_t iSize);
	virtual ~Buffer(); 
		
	void	add(u8* c,u32 s);
	u32	get(u8* c,u32 s);
	void	clear();
	u32		isEmpty();
		
	Buffer &operator>>(char *c);

private:
	size_t	m_iSize;
	char*	map;
	
};

#endif
