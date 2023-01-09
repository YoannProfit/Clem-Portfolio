//----------------------------------------------------
// Auteur : Clément Profit
// Nom du fichier : DataParameters.h
// Date de création : Octobre 2015
// Description : Cette classe qui permet de gérer 
// et de regrouper des données ensemble
//----------------------------------------------------

#ifndef DATA_PARAMETERS_H
#define DATA_PARAMETERS_H
  
#include "Array.h"

template<typename CharType, typename StringType>
class DataParameters
{
public:
    // Différents types de variables à stocker
    struct Param { int i; float f; StringType s; };
  
    DataParameters();
    virtual ~DataParameters();
      
    // Premier numéro d'index (c'est-à-dire le numéro 0)
    DataParameters(const int param);
    DataParameters(const float param);
    DataParameters(const CharType* param);
    DataParameters(const StringType& sParam);
  
    // Par nom
    DataParameters(const StringType& sParamName, const int param);
    DataParameters(const StringType& sParamName, const float param);
    DataParameters(const StringType& sParamName, const CharType* param);
    DataParameters(const StringType& sParamName, const StringType& sParam);
  
    /******************/
  
    // Par nom
    void SetParam(const StringType& sParamName, const int param);
    void SetParam(const StringType& sParamName, const float param);
    void SetParam(const StringType& sParamName, const CharType* param);
    void SetParam(const StringType& sParamName, const StringType& sParam);
  
    // Par index
    void PushParam(const int param);
    void PushParam(const float param);
    void PushParam(const CharType* param);
    void PushParam(const StringType& param);
  
    Param* GetParameter(const StringType& sParamName);

    int GetParameterAsInt(const StringType& sParamName);
    float GetParameterAsFloat(const StringType& sParamName);
    const CharType* GetParameterAsString(const StringType& sParamName);
  
    // Par accès au premier argument
    Param* GetFirstParameter();
    int GetFirstParameterAsInt();
    float GetFirstParameterAsFloat();
    const CharType* GetFirstParameterAsString();
 
    uint16 GetParamNumber();
  
    // Opérateurs >> flux pour lire les données
    DataParameters& operator >>(bool& data);
    DataParameters& operator >>(int& data);
    DataParameters& operator >>(unsigned int& data);
    DataParameters& operator >>(float& data);
    DataParameters& operator >>(CharType* data);
    DataParameters& operator >>(StringType& data);
 
    // Opérateurs << flux pour écrire des données
    DataParameters& operator <<(bool data);
    DataParameters& operator <<(int data);
    DataParameters& operator <<(unsigned int data);
    DataParameters& operator <<(float data);
    DataParameters& operator <<(CharType* data);
    DataParameters& operator <<(StringType data);
 
    Core::Map<StringType, Param>& GetParameters();
 
    // Réinitialise les flux sortants
	void ResetStream();

private:
    // Liste des paramètres
    Core::Map<StringType, Param> m_params;
 
    // Numéro de paramètre courant
    uint16 m_iIndex;
 
    // Position de lecture du stream courant
    uint16 m_iCurrentStreamPos;
};
  
#endif