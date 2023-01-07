#include "DataParameters.h"
  
DataParameters::DataParameters() :
m_iIndex(0),
m_iCurrentStreamPos(0)
{
}
  
DataParameters::~DataParameters()
{
    m_params.Clear();
}
  
DataParameters::DataParameters(const int param) :
m_iIndex(0),
m_iCurrentStreamPos(0)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].i = param;
    m_iIndex++;
}
  
DataParameters::DataParameters(const float param) :
m_iIndex(0),
m_iCurrentStreamPos(0)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].f = param;
    m_iIndex++;
}
  
DataParameters::DataParameters(const char* param) :
m_iIndex(0),
m_iCurrentStreamPos(0)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].s = param;
    m_iIndex++;
}
  
DataParameters::DataParameters(const StringType& param) :
m_iIndex(0),
m_iCurrentStreamPos(0)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].s = param;
    m_iIndex++;
}
  
/******* Par nom *******/
  
DataParameters::DataParameters(const StringType& sParamName, const int param)
{
    m_params[sParamName].i = param;
}
  
DataParameters::DataParameters(const StringType& sParamName, const float param)
{
    m_params[sParamName].f = param;
}
  
DataParameters::DataParameters(const StringType& sParamName, const char* param)
{
    m_params[sParamName].s = param;
}
  
DataParameters::DataParameters(const StringType& sParamName, const StringType& sParam)
{
    m_params[sParamName].s = sParam;
}
  
/**********************************/
  
void DataParameters::SetParam(const StringType& sParamName, const int param)
{
    m_params[sParamName].i = param;
}
  
void DataParameters::SetParam(const StringType& sParamName, const float param)
{
    m_params[sParamName].f = param;
}
  
void DataParameters::SetParam(const StringType& sParamName, const char* param)
{
    m_params[sParamName].s = param;
}
  
void DataParameters::SetParam(const StringType& sParamName, const StringType& param)
{
    m_params[sParamName].s = param;
}
  
/*************** Getters ***************/
  
int DataParameters::GetParameterAsInt(const StringType& sParamName)
{
    if (m_params.count(sParamName) > 0)
    {
        return m_params[sParamName].i;
    }
  
    return 0;
}
  
float DataParameters::GetParameterAsFloat(const StringType& sParamName)
{
    if (m_params.count(sParamName) > 0)
    {
        return m_params[sParamName].f;
    }
  
    return 0.0f;
}
  
const char* DataParameters::GetParameterAsString(const StringType& sParamName)
{
    if (m_params.count(sParamName) > 0)
    {
        return m_params[sParamName].s.GetPointer();
    }
  
    return nullptr;
}
  
DataParameters::Param* DataParameters::GetParameter(const StringType& sParamName)
{
    if (m_params.count(sParamName) > 0)
    {
        return &m_params[sParamName];
    }
  
    return nullptr;
}
 
std::map<StringType, DataParameters::Param>& DataParameters::GetParameters()
{
    return m_params;
}
 
/*************** Getters sans paramètres ***************/
  
int DataParameters::GetFirstParameterAsInt()
{
    if (m_params.count("0") > 0)
    {
        return m_params["0"].i;
    }
  
    return 0;
}
  
float DataParameters::GetFirstParameterAsFloat()
{
    if (m_params.count("0") > 0)
    {
        return m_params["0"].f;
    }
  
    return 0.0f;
}
  
const char* DataParameters::GetFirstParameterAsString()
{
    if (m_params.count("0") > 0)
    {
        return m_params["0"].s.GetPointer();
    }
  
    return nullptr;
}
  
DataParameters::Param* DataParameters::GetFirstParameter()
{
    if (m_params.count("0") > 0)
    {
        return &m_params["0"];
    }
  
    return nullptr;
}
  
unsigned int DataParameters::GetParamNumber()
{
    return m_params.size();
}
  
/****************/
  
void DataParameters::PushParam(const int param)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].i = param;
    m_iIndex++;
}
  
void DataParameters::PushParam(const float param)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].f = param;
    m_iIndex++;
}
void DataParameters::PushParam(const char* param)
{
    StringType sParamName = std::to_string(m_iIndex);
  
    m_params[sParamName].s = param;
    m_iIndex++;
}
  
void DataParameters::PushParam(const StringType& param)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].s = param;
    m_iIndex++;
}
 
//----------------------------------------
// Opérateurs >> pour lire les données
//----------------------------------------
DataParameters& DataParameters::operator >>(bool& data)
{
	StringType sParamName = StringType::ToString(m_iCurrentStreamPos); 

    if (m_params.Count(sParamName) > 0)
    {
        data = (m_params[sParamName].i == 0) ? 0 : 1;
 
        m_iCurrentStreamPos++;
    }
    else
    {
        data = false;
    }
 
    return *this;
}
 
DataParameters& DataParameters::operator >>(int& data)
{
	StringType sParamName = StringType::ToString(m_iCurrentStreamPos); 

    if (m_params.Count(sParamName) > 0)
    {
        data = m_params[sParamName].i;
 
        m_iCurrentStreamPos++;
    }
    else
    {
        data = 0;
    }
 
    return *this;
}
 
DataParameters& DataParameters::operator >>(unsigned int & data)
{
	StringType sParamName = StringType::ToString(m_iCurrentStreamPos); 

    if (m_params.Count(sParamName) > 0)
    {
        data = m_params[sParamName].i;
 
        m_iCurrentStreamPos++;
    }
    else
    {
        data = 0;
    }
 
    return *this;
}
 
DataParameters& DataParameters::operator >>(float& data)
{
	StringType sParamName = StringType::ToString(m_iCurrentStreamPos); 

    if (m_params.Count(sParamName) > 0)
    {
        data = m_params[sParamName].f;
 
        m_iCurrentStreamPos++;
    }
    else
    {
        data = 0.0f;
    }
 
    return *this;
}
 
DataParameters& DataParameters::operator >>(char* data)
{
	StringType sParamName = StringType::ToString(m_iCurrentStreamPos); 

    if (m_params.Count(sParamName) > 0)
    {
        data = (char*)m_params[sParamName].s.c_str();
 
        m_iCurrentStreamPos++;
    }
    else
    {
        data = "";
    }
 
    return *this;
}
 
DataParameters& DataParameters::operator >>(StringType& data)
{
	StringType sParamName = StringType::ToString(m_iCurrentStreamPos); 

    if (m_params.Count(sParamName) > 0)
    {
        data = m_params[sParamName].s;
 
        m_iCurrentStreamPos++;
    }
    else
    {
        data = "";
    }
 
    return *this;
}
 
//----------------------------------------
// Opérateurs << pour écrire des données
//----------------------------------------
DataParameters& DataParameters::operator <<(bool data)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].i = data;
    m_iIndex++;
 
    return *this;
}
 
DataParameters& DataParameters::operator <<(int data)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].i = data;
    m_iIndex++;
 
    return *this;
}
 
DataParameters& DataParameters::operator <<(unsigned int data)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].i = data;
    m_iIndex++;
 
    return *this;
}
 
DataParameters& DataParameters::operator <<(float data)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].f = data;
    m_iIndex++;
 
    return *this;
}
 
DataParameters& DataParameters::operator <<(char* data)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].s = data;
    m_iIndex++;

    return *this;
}
 
DataParameters& DataParameters::operator <<(StringType data)
{
	StringType sParamName = StringType::ToString(m_iIndex); 

    m_params[sParamName].s = data;
    m_iIndex++;
 
    return *this;
}

void DataParameters::ResetStream()
{
    m_iCurrentStreamPos = 0;
}
