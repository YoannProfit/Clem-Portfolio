//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : File.h
// Date de création : Octobre 2015
// Description : Une classe qui gère les fichiers
// du système
//----------------------------------------------------


#ifndef FILE_H
#define FILE_H

#include <runtime/types.h>

enum FileType
{
	TYPE_FILE,
	TYPE_DIRECTORY,
	TYPE_DEVICE,
	TYPE_PROCESS,
	TYPE_LINK
};

class File
{
	public:
		File(char* n,u8 t);
		~File();
		
		void	checkName();
		
		u32		addChild(File* n);
		File*	createChild(char* n,u8 t);
		File* 	find(char* n);
		u32 	mmap(u32 sizee,u32 flags,u32 offset,u32 prot);
		
		void	setSize(u32 t);
		void	setType(u8 t);
		void	setParent(File* n);
		void	setChild(File* n);
		void	setNext(File* n);
		void	setPrec(File* n);
		void	setLink(File* n);
		void	setName(char* n);
		
		char*	getName();
		File*	getParent();
		File*	getChild();
		File*	getNext();
		File*	getPrec();
		File*	getLink();
		u8		getType();
		u32		getSize();
		u32		getInode();
		
		stat_fs stat();
		
	protected:
		static u32 inode_system;
	
		char*	map_memory;	/* to mmap */
		
		char*	name;		/* Nom du fichier	*/
		u32		size;		/* Taille du fichier */
		u8		type;		/* Type de fichier */
		u32		inode;		/* Inode du fichier */
		File*	dev;		/* the master device, example : /dev/hda */
		File*	link;		/* the real file, if this file is a link */
		
		
		File*	master;	/* processus maitre ou NULL */
		
		File*	parent;
		File*	child;
		File*	next;
		File*	prec;
		
		File*	device;		/* This file is the device master of the current file */
};

#endif
