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

#ifndef _F_FILE_XML_TINY_H_
#define _F_FILE_XML_TINY_H_

#include "FPreDefine.h"

class TiXmlDocument;
class TiXmlElement;

namespace LostPeterFoundation
{
    class LPF_Export FFileXMLTiny
	{
	public:
		FFileXMLTiny();
		virtual ~FFileXMLTiny();

	public:
	protected:
		TiXmlDocument* m_pDocXml;
		bool m_bIsOpenXML;

	public:
		bool IsFileExsit(const char* szPath);
		bool IsOpen() { return m_bIsOpenXML; }	

		bool CreateXML(bool bIsEmpty = true);		
		bool LoadXML(const char* szXMLPath);			
		bool SaveXML(const char* szXMLPath);			
		void Release();								
		
		TiXmlDocument* GetDocument();
		TiXmlElement* GetRootItem();
		TiXmlElement* FindItem(TiXmlElement* pParent, const char* szItemName);
		TiXmlElement* FindItemAttribute(TiXmlElement* pParent, const char* szAttributeName, const char* szToMatch);
		TiXmlElement* FindRootItem(const char* szItemName);

	public:
		static bool	ParserBool(TiXmlElement* pElement, const char* szItemName, bool& bValue);
		static bool	ParserInt(TiXmlElement* pElement, const char* szItemName, int32& nValue);
		static bool	ParserUint(TiXmlElement* pElement, const char* szItemName, uint32& nValue);
		static bool	ParserFloat(TiXmlElement* pElement, const char* szItemName, float& fValue);
		static bool	ParserDouble(TiXmlElement* pElement, const char* szItemName, double& dValue);
		static bool	ParserString(TiXmlElement* pElement, const char* szItemName, String& strValue);
		static bool	ParserColor(TiXmlElement* pElement, const char* szItemName, FColor& clValue);
		static bool ParserPointI(TiXmlElement* pElement, const char* szItemName, FPointI& point);
		static bool ParserPointF(TiXmlElement* pElement, const char* szItemName, FPointF& point);
		static bool ParserSizeI(TiXmlElement* pElement, const char* szItemName, FSizeI& size);
		static bool ParserSizeF(TiXmlElement* pElement, const char* szItemName, FSizeF& size);
		static bool ParserRectI(TiXmlElement* pElement, const char* szItemName, FRectI& rect);
		static bool ParserRectF(TiXmlElement* pElement, const char* szItemName, FRectF& rect);
		static bool	ParserVector2(TiXmlElement* pElement, const char* szItemName, FVector2& v2Value);
		static bool	ParserVector3(TiXmlElement* pElement, const char* szItemName, FVector3& v3Value);
		static bool	ParserVector4(TiXmlElement* pElement, const char* szItemName, FVector4& v4Value);
		static bool	ParserQuaternion(TiXmlElement* pElement, const char* szItemName, FQuaternion& qValue);
		static bool	ParserMatrix3(TiXmlElement* pElement, const char* szItemName, FMatrix3& mat3Value);
		static bool	ParserMatrix4(TiXmlElement* pElement, const char* szItemName, FMatrix4& mat4Value);

		static void	SaveBool(TiXmlElement* pElement, const char* szItemName, bool bValue);
		static void	SaveInt(TiXmlElement* pElement, const char* szItemName, int32 nValue);
		static void	SaveUint(TiXmlElement* pElement, const char* szItemName, uint32 nValue);
		static void	SaveFloat(TiXmlElement* pElement, const char* szItemName, float fValue);
		static void	SaveDouble(TiXmlElement* pElement, const char* szItemName, double dValue);
		static void	SaveString(TiXmlElement* pElement, const char* szItemName, const String& strValue);
		static void	SaveColor(TiXmlElement* pElement, const char* szItemName, const FColor& clValue);
		static void	SavePointI(TiXmlElement* pElement, const char* szItemName, const FPointI& point);
		static void	SavePointF(TiXmlElement* pElement, const char* szItemName, const FPointF& point);
		static void	SaveSizeI(TiXmlElement* pElement, const char* szItemName, const FSizeI& size);
		static void	SaveSizeF(TiXmlElement* pElement, const char* szItemName, const FSizeF& size);
		static void	SaveRectI(TiXmlElement* pElement, const char* szItemName, const FRectI& rect);
		static void	SaveRectF(TiXmlElement* pElement, const char* szItemName, const FRectF& rect);
		static void	SaveVector2(TiXmlElement* pElement, const char* szItemName, const FVector2& v2Value);
		static void	SaveVector3(TiXmlElement* pElement, const char* szItemName, const FVector3& v3Value);
		static void	SaveVector4(TiXmlElement* pElement, const char* szItemName, const FVector4& v4Value);
		static void	SaveQuaternion(TiXmlElement* pElement, const char* szItemName, const FQuaternion& qValue);
		static void	SaveMatrix3(TiXmlElement* pElement, const char* szItemName, const FMatrix3& mat3Value);
		static void	SaveMatrix4(TiXmlElement* pElement, const char* szItemName, const FMatrix4& mat4Value);

	protected:
		virtual bool createInternal();		
		virtual	bool loadInternal();
		virtual bool saveInternal();
	};

}; //LostPeterFoundation

#endif