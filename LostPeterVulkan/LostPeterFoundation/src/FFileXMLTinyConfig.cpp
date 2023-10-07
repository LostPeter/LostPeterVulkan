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

#include "../include/FFileXMLTinyConfig.h"

#include "tinyxml.h"
#include "tinystr.h"

namespace LostPeterFoundation
{
    FFileXMLTinyConfig::FFileXMLTinyConfig()
		: m_strFilePath("")
		, m_pRoot(0)
	{
		
	}

	FFileXMLTinyConfig::~FFileXMLTinyConfig()
	{
		m_FileXML.Destroy();
	}
	
	bool FFileXMLTinyConfig::Create(const char* szRootName)
	{
		if (!m_FileXML.CreateXML(true))
		{
			F_Assert(false && "FFileXMLTinyConfig::Create")
			return false;
		}
		m_pRoot = new TiXmlElement(szRootName);
		m_FileXML.GetDocument()->LinkEndChild(m_pRoot);

		return true;
	}

	bool FFileXMLTinyConfig::Load(const char* szFilePath)
	{
		m_strFilePath = szFilePath;
		if (!m_FileXML.LoadXML(szFilePath))
		{
			F_Assert(false && "FFileXMLTinyConfig::Load")
			return false;
		}
		m_pRoot = m_FileXML.GetRootItem();

		return true;
	}

	bool FFileXMLTinyConfig::Save(const char* szFilePath)
	{
		if (!m_FileXML.SaveXML(szFilePath))
		{
			F_Assert(false && "FFileXMLTinyConfig::Save")
			return false;
		}

		return true;
	}

	bool FFileXMLTinyConfig::GetBool(TiXmlElement* pElement, const char* szName, bool& bValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::GetBool")
		
		int nValue=0;
		const char* pRet = pElement->Attribute(szName,&nValue);
		bValue = (nValue==0?false:true);

		return pRet?true:false;
	}

	bool FFileXMLTinyConfig::SetBool(TiXmlElement* pElement, const char* szName, const bool& bValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::SetBool")

		pElement->SetAttribute(szName,bValue?1:0);
		return true;
	}

	bool FFileXMLTinyConfig::GetInt(TiXmlElement* pElement, const char* szName, int& nValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::GetInt")
		
		const char* pRet = pElement->Attribute(szName,&nValue);
		return pRet?true:false;
	}

	bool FFileXMLTinyConfig::SetInt(TiXmlElement* pElement, const char* szName, const int& nValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::SetInt")

		pElement->SetAttribute(szName,nValue);
		return true;
	}

	bool FFileXMLTinyConfig::GetFloat(TiXmlElement* pElement, const char* szName, float& fValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::GetFloat")
		
		double dV = 0.0f;
		const char* pRet = pElement->Attribute(szName,&dV);
		fValue = (float)dV;
		return pRet?true:false;
	}

	bool FFileXMLTinyConfig::SetFloat(TiXmlElement* pElement, const char* szName, const float& fValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::SetFloat")

		double dV = fValue;
		pElement->SetDoubleAttribute(szName,dV);
		return true;
	}

	bool FFileXMLTinyConfig::GetDouble(TiXmlElement* pElement, const char* szName, double& fValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::GetDouble")
		
		const char* pRet = pElement->Attribute(szName,&fValue);
		return pRet?true:false;
	}

	bool FFileXMLTinyConfig::SetDouble(TiXmlElement* pElement, const char* szName, const double& fValue)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::SetDouble")

		pElement->SetDoubleAttribute(szName,fValue);
		return true;
	}

	bool FFileXMLTinyConfig::GetString(TiXmlElement* pElement, const char* szName, String& str)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::GetString")
		
		str = pElement->Attribute(szName);
		return str.empty();
	}

	bool FFileXMLTinyConfig::SetString(TiXmlElement* pElement, const char* szName, const String& str)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::SetString")
		
		pElement->SetAttribute(szName,str.c_str());
		return true;
	}

	bool FFileXMLTinyConfig::SetString(TiXmlElement* pElement, const char* szName, const char* szStr)
	{
		F_Assert(pElement && szName && "FFileXMLTinyConfig::SetString")

		pElement->SetAttribute(szName,szStr);
		return true;
	}
    
}; //LostPeterFoundation