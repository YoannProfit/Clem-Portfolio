//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : Singleton.h
// Date de création : Octobre 2015
// Description : Singleton design pattern pour gérer
// une instance unique d'un objet
//----------------------------------------------------

#ifndef SINGLETON_H
#define SINGLETON_H

#include "Types.h"
#include "Defines.h"

template <typename T>
class Singleton
{
public:
    Singleton()
    {
        assert(!ms_instance);
 
        ms_instance = static_cast<T*>(this);
    }
 
    ~Singleton () {}
 
    static T* GetSingletonPtr()
    {
        assert(ms_instance);
 
        return ms_instance;
    }
 
    static void Destroy()
    {
        SAFE_DELETE(ms_instance);
    }
 
    static bool IsCreated()
    {
        return ms_instance != nullptr;
    }
 
private: // Opération interdite
    Singleton(const Singleton<T> &);
    Singleton& operator=(const Singleton<T> &);
 
private:
    static T* ms_instance;
};
 
#endif