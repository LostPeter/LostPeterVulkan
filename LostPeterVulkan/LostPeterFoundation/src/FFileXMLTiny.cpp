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

#include "../include/FFileXMLTiny.h"
#include "../include/FUtilString.h"

#include "tinyxml.h"
#include "tinystr.h"

namespace LostPeterFoundation
{
    FFileXMLTiny::FFileXMLTiny()
		: m_pDocXml(0)
		, m_bIsOpenXML(false)
	{
		
	}

	FFileXMLTiny::~FFileXMLTiny()
	{
		Release();
	}

	void FFileXMLTiny::Release()
	{
		F_DELETE(m_pDocXml)
		m_bIsOpenXML = false;
	}

	bool FFileXMLTiny::LoadXML(const char* szXMLPath)
	{
		Release();
		m_pDocXml = new TiXmlDocument;
		if (!m_pDocXml)
			return false;

		if (m_pDocXml->LoadFile(szXMLPath))
		{
			if(!loadInternal())
				return false;

			m_bIsOpenXML = true;
			return true;
		}

		return false;
	}

	bool FFileXMLTiny::loadInternal()
	{
		return true;
	}

	bool FFileXMLTiny::SaveXML(const char* szXMLPath)
	{
		if (!m_pDocXml)
			return false;

		if (!saveInternal())
			return false;

		return m_pDocXml->SaveFile(szXMLPath);
	}

	bool FFileXMLTiny::saveInternal()
	{
		return true;
	}

	bool FFileXMLTiny::CreateXML(bool bIsEmpty /*= true*/)
	{
		if (m_pDocXml)
			Release();

		m_pDocXml = new TiXmlDocument;
		if (!m_pDocXml)
			return false;

		TiXmlDeclaration* dec = new TiXmlDeclaration("1.0", "utf-8", "");
		m_pDocXml->LinkEndChild(dec);

		if (!bIsEmpty)
		{
			if (!createInternal())
				return false;
		}

		m_bIsOpenXML = true;
		return true;
	}

	bool FFileXMLTiny::createInternal()
	{
		return true;
	}

	bool FFileXMLTiny::IsFileExsit(const char* szPath)
	{
		FILE* file = fopen(szPath,"r");
		if (file) 
		{
			fclose(file);
			return true;
		}	
		return false;
	}
	
	TiXmlDocument* FFileXMLTiny::GetDocument()
	{
		return m_pDocXml;
	}

	TiXmlElement* FFileXMLTiny::GetRootItem()
	{
		if (!m_pDocXml)
			return 0;

		return m_pDocXml->RootElement();
	}

	TiXmlElement* FFileXMLTiny::FindItem(TiXmlElement* pParent, const char* szItemName)
	{
		if (!pParent || !szItemName)
			return 0;

		TiXmlElement *pCur = pParent->FirstChildElement();
		while (pCur)
		{
			if (strcmp(pCur->Value(), szItemName) == 0)
				return pCur;

			pCur = pCur->NextSiblingElement();
		}

		return 0;
	}

	TiXmlElement* FFileXMLTiny::FindItemAttribute(TiXmlElement* pParent, const char* szAttributeName, const char* szToMatch)
	{
		if (!pParent || !szAttributeName || !szAttributeName)
			return 0;

		TiXmlElement *pCur = pParent->FirstChildElement();
		while (pCur)
		{
			if (strcmp(pCur->Attribute(szAttributeName),szToMatch) == 0)
				return pCur;

			pCur = pCur->NextSiblingElement();
		}

		return 0;
	}

	TiXmlElement* FFileXMLTiny::FindRootItem(const char* szItemName)
	{
		TiXmlElement *pRoot = GetRootItem();
		if (!pRoot)
			return 0;

		return FindItem(pRoot,szItemName);
	}

	bool FFileXMLTiny::ParserBool(TiXmlElement* pElement, const char* szItemName, bool& bValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserBool")
		bValue = false;
		if (!pElement->Attribute(szItemName))
			return false;
		if (0 == strcmp(pElement->Attribute(szItemName),"true"))
			bValue = true;

		return true;
	}

	bool FFileXMLTiny::ParserInt(TiXmlElement* pElement, const char* szItemName, int32& nValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserInt")
		nValue = 0;
		if (!pElement->Attribute(szItemName))
			return false;
		pElement->Attribute(szItemName,&nValue);

		return true;
	}	

	bool FFileXMLTiny::ParserUint(TiXmlElement* pElement,const char* szItemName,uint32& nValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserUint")
		nValue = 0;
		if (!pElement->Attribute(szItemName))
			return false;
		int n = 0;
		pElement->Attribute(szItemName,&n);
		nValue = (uint32)n;

		return true;
	}

	bool FFileXMLTiny::ParserFloat(TiXmlElement* pElement, const char* szItemName, float& fValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserFloat")
		fValue = 0;
		if (!pElement->Attribute(szItemName))
			return false;
		double dValue = 0;
		pElement->Attribute(szItemName,&dValue);
		fValue = (float)dValue;

		return true;
	}

	bool FFileXMLTiny::ParserDouble(TiXmlElement* pElement, const char* szItemName, double& dValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserDouble")
		dValue = 0;
		if (!pElement->Attribute(szItemName))
			return false;
		pElement->Attribute(szItemName,&dValue);

		return true;
	}

	bool FFileXMLTiny::ParserString(TiXmlElement* pElement, const char* szItemName, String& strValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserString")
		if (!pElement->Attribute(szItemName))
			return false;
		strValue = pElement->Attribute(szItemName);

		return true;
	}

	bool FFileXMLTiny::ParserColor(TiXmlElement* pElement, const char* szItemName, FColor& clValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserColor")
		if (!pElement->Attribute(szItemName))
			return false;
		clValue = FUtilString::ParserColor(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserSize(TiXmlElement* pElement, const char* szItemName, FSize& size)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserSize")
		if (!pElement->Attribute(szItemName))
			return false;
		size = FUtilString::ParserSize(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserPoint(TiXmlElement* pElement, const char* szItemName, FPoint& point)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserPoint")
		if (!pElement->Attribute(szItemName))
			return false;
		point = FUtilString::ParserPoint(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserVector2(TiXmlElement* pElement, const char* szItemName, FVector2& v2Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserVector2")
		if (!pElement->Attribute(szItemName))
			return false;
		v2Value = FUtilString::ParserVector2(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserVector3(TiXmlElement* pElement, const char* szItemName, FVector3& v3Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserVector3")
		if (!pElement->Attribute(szItemName))
			return false;
		v3Value = FUtilString::ParserVector3(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserVector4(TiXmlElement* pElement, const char* szItemName, FVector4& v4Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserVector4")
		if (!pElement->Attribute(szItemName))
			return false;
		v4Value = FUtilString::ParserVector4(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserQuaternion(TiXmlElement* pElement, const char* szItemName, FQuaternion& qValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserQuaternion")
		if (!pElement->Attribute(szItemName))
			return false;
		qValue = FUtilString::ParserQuaternion(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserMatrix3(TiXmlElement* pElement, const char* szItemName, FMatrix3& mat3Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserMatrix3")
		if (!pElement->Attribute(szItemName))
			return false;
		mat3Value = FUtilString::ParserMatrix3(pElement->Attribute(szItemName));

		return true;
	}

	bool FFileXMLTiny::ParserMatrix4(TiXmlElement* pElement, const char* szItemName, FMatrix4& mat4Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::ParserMatrix4")
		if (!pElement->Attribute(szItemName))
			return false;
		mat4Value = FUtilString::ParserMatrix4(pElement->Attribute(szItemName));

		return true;
	}

	void FFileXMLTiny::SaveBool(TiXmlElement* pElement, const char* szItemName, bool bValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveBool")
		pElement->SetAttribute(szItemName,bValue?"true":"false");
	}

	void FFileXMLTiny::SaveInt(TiXmlElement* pElement, const char* szItemName, int32 nValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveInt")
		pElement->SetAttribute(szItemName,nValue);
	}

	void FFileXMLTiny::SaveUint(TiXmlElement* pElement, const char* szItemName, uint32 nValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveUint")
		pElement->SetAttribute(szItemName,(int32)nValue);
	}

	void FFileXMLTiny::SaveFloat(TiXmlElement* pElement, const char* szItemName, float fValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveFloat")
		pElement->SetDoubleAttribute(szItemName,(double)fValue);
	}

	void FFileXMLTiny::SaveDouble(TiXmlElement* pElement, const char* szItemName, double dValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveDouble")
		pElement->SetDoubleAttribute(szItemName,dValue);
	}

	void FFileXMLTiny::SaveString(TiXmlElement* pElement, const char* szItemName, const String& strValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveString")
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveColor(TiXmlElement* pElement, const char* szItemName, const FColor& clValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveColor")
		String strValue = FUtilString::SaveColor(clValue);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveSize(TiXmlElement* pElement, const char* szItemName, const FSize& size)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveSize")
		String strValue = FUtilString::SaveSize(size);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SavePoint(TiXmlElement* pElement, const char* szItemName, const FPoint& point)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SavePoint")
		String strValue = FUtilString::SavePoint(point);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveVector2(TiXmlElement* pElement, const char* szItemName, const FVector2& v2Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveVector2")
		String strValue = FUtilString::SaveVector2(v2Value);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveVector3(TiXmlElement* pElement, const char* szItemName, const FVector3& v3Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveVector3")
		String strValue = FUtilString::SaveVector3(v3Value);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveVector4(TiXmlElement* pElement, const char* szItemName, const FVector4& v4Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveVector4")
		String strValue = FUtilString::SaveVector4(v4Value);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveQuaternion(TiXmlElement* pElement, const char* szItemName, const FQuaternion& qValue)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveQuaternion")
		String strValue = FUtilString::SaveQuaternion(qValue);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveMatrix3(TiXmlElement* pElement, const char* szItemName, const FMatrix3& mat3Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveMatrix3")
		String strValue = FUtilString::SaveMatrix3(mat3Value);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}

	void FFileXMLTiny::SaveMatrix4(TiXmlElement* pElement, const char* szItemName, const FMatrix4& mat4Value)
	{
		F_Assert(pElement && szItemName && "FFileXMLTiny::SaveMatrix4");
		String strValue = FUtilString::SaveMatrix4(mat4Value);
		pElement->SetAttribute(szItemName,strValue.c_str());
	}
    
}; //LostPeterFoundation