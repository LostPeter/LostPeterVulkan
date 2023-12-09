/****************************************************************************
* LostPeterFoundation - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-06-17
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _F_FILE_XML_TINY_CONFIG_H_
#define _F_FILE_XML_TINY_CONFIG_H_

#include "FFileXMLTiny.h"

namespace LostPeterFoundation
{
    class foundationExport FFileXMLTinyConfig
	{
	public:
		FFileXMLTinyConfig();
		~FFileXMLTinyConfig();

	protected:
		FFileXMLTiny m_FileXML;
		String m_strFilePath;
		TiXmlElement* m_pRoot;

	public:
		TiXmlElement* GetRoot()	{ return m_pRoot; }
	
	public:
		bool Create(const char* szRootName);
		bool Load(const char* szFilePath);
		bool Save(const char* szFilePath);

	public:
		bool GetBool(TiXmlElement* pElement, const char* szName, bool& bValue);
		bool SetBool(TiXmlElement* pElement, const char* szName, const bool& bValue);
		bool GetInt(TiXmlElement* pElement, const char* szName, int& nValue);
		bool SetInt(TiXmlElement* pElement, const char* szName, const int& nValue);
		bool GetFloat(TiXmlElement* pElement, const char* szName, float& fValue);
		bool SetFloat(TiXmlElement* pElement, const char* szName, const float& fValue);
		bool GetDouble(TiXmlElement* pElement, const char* szName, double& fValue);
		bool SetDouble(TiXmlElement* pElement, const char* szName, const double& fValue);
		bool GetString(TiXmlElement* pElement, const char* szName, String& str);
		bool SetString(TiXmlElement* pElement, const char* szName, const String& str);
		bool SetString(TiXmlElement* pElement, const char* szName, const char* szStr);
	};

}; //LostPeterFoundation

#endif