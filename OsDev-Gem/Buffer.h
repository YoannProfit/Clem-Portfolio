//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Buffer.h
// Date de création : Octobre 2015
// Description : Cette classe permet de gérer un buffer
// (un tampon de donnée)
//---------------------------------------------------

#ifndef BUFFER_H
#define BUFFER_H

class Buffer
{
public:
	Buffer(char* n,size_t iSize);
	virtual ~Buffer(); 
		
	void	Add(u8* c,u32 s);
	u32		Get(u8* c,u32 s);
	void	Clear();
	u32		IsEmpty();
		
	Buffer &operator>>(char *c);

private:
	size_t	m_iSize;
	char*	map;
	
};

#endif
