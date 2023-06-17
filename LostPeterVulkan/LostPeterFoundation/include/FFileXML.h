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

#ifndef _F_FILE_XML_H_
#define _F_FILE_XML_H_

#include "FPreDefine.h"

namespace LostPeterFoundation
{
    class FXMLAttribute;
    class FXMLElement;
    class FXMLDocument;

    typedef std::vector<FXMLAttribute> FXMLAttributeVector;
    typedef std::vector<FXMLElement*> FXMLElementPtrVector;

    ////////////////////////// FXMLAttribute /////////////////////
    class LPF_Export FXMLAttribute
    {
        friend class FXMLElement;

    public:
        FXMLAttribute()
            : m_strName("")
            , m_strValue("")
        {

        }
        FXMLAttribute(const String& strName, const String& strValue)
            : m_strName(strName)
            , m_strValue(strValue)
        {

        }
        ~FXMLAttribute()
        {

        }

    public:
    protected:
        String m_strName;
        String m_strValue;

    public:
        LP_FORCEINLINE const String& GetName() const { return m_strName; }
        LP_FORCEINLINE void SetName(const String& strName) { m_strName = strName; }
        LP_FORCEINLINE const String& GetValue() const { return m_strValue; }
        LP_FORCEINLINE void SetValue(const String& strValue) { m_strValue = strValue; }
    };

    ////////////////////////// FXMLElement ///////////////////////
    class LPF_Export FXMLElement
    {
        friend class FXMLDocument;

    public:
        FXMLElement()
            : m_strName("")
        {

        }
        FXMLElement(const String& strName)
            : m_strName(strName)
        {

        }
        ~FXMLElement();

    public:
    protected:
        String m_strName;
        FXMLAttributeVector m_aAttributes;

        FXMLElement* m_pElementParent;
        FXMLElementPtrVector m_aElementChildren;

    public:
        LP_FORCEINLINE const String& GetName() const { return m_strName; }
        LP_FORCEINLINE void SetName(const String& strName) { m_strName = strName; }
        LP_FORCEINLINE const FXMLAttributeVector& GetAttributes() const { return m_aAttributes; }
        LP_FORCEINLINE FXMLAttributeVector& GetAttributes() { return m_aAttributes; }
        LP_FORCEINLINE FXMLElement* GetElementParent() const { return m_pElementParent; }
        LP_FORCEINLINE const FXMLElementPtrVector& GetElementChildren() const { return m_aElementChildren; }
        LP_FORCEINLINE FXMLElementPtrVector& GetElementChildren() { return m_aElementChildren; }

    public:
        void Destroy();

        FXMLElement* SetElementParent(FXMLElement* pElementParent);

        int GetElementChildrenCount() const;
        FXMLElement* GetElementChild(int index);

        FXMLElement* InsertElementChild(FXMLElement* pElementChild, int index);
        FXMLElement* AddElementChild(FXMLElement* pElementChild);

        FXMLElement* FindElementChild(const String& strName);
        void FindElementChildren(const String& strName, FXMLElementPtrVector& aElementChildren);

        void RemoveElementChild(FXMLElement* pElementChild);
        void RemoveElementChildren(const String& strName, FXMLElementPtrVector& aElementChildren);
        void DeleteElementChildren(const String& strName);

        void DeleteElements(FXMLElementPtrVector& aElements);

        void AddAttribute(const String& strName, const String& strValue);
        void AddAttribute(FXMLAttribute& attribute);
        void AddAttributes(FXMLAttributeVector& aAttributes);
    };

    ////////////////////////// FXMLDocument //////////////////////
    class LPF_Export FXMLDocument
    {
    public:
        FXMLDocument();
        virtual ~FXMLDocument();

    public:
    protected:
        FXMLElement* m_pElementDeclaration;
        FXMLElement* m_pElementRoot;

    public:
        LP_FORCEINLINE FXMLElement* GetElementDeclaration() const { return m_pElementDeclaration; }
        LP_FORCEINLINE FXMLElement* GetElementRoot() const { return m_pElementRoot; }

    public:
        void Destroy();
        
        FXMLElement* CreateElementDeclaration(const String& strVersion = "1.0", const String& strEncoding = "utf-8", const String& strStandalone = "");
        FXMLElement* CreateElementRoot(const String& strName);

        FXMLElement* CreateElement(FXMLElement* pElementParent);

    };

    ////////////////////////// FFileXML //////////////////////////
    class LPF_Export FFileXMLImplement
    {
    public:
        FFileXMLImplement()
        {

        }
        virtual ~FFileXMLImplement()
        {

        }

    public:
        virtual void CreateXML() = 0;
        virtual bool ParseXML(const char* szContent, FXMLDocument* pDocument) = 0;
        virtual bool LoadXML(const String& strPath, FXMLDocument* pDocument) = 0;
        virtual bool SaveXML(const String& strPath, FXMLDocument* pDocument) = 0;
    };

    class LPF_Export FFileXML
    {
    public:
        FFileXML();
        FFileXML(FFileXMLType eFileXML);
        virtual ~FFileXML();

    public:
    protected:
        FFileXMLType m_eFileXML;
        FXMLDocument* m_pXMLDocument;

        FFileXMLImplement* m_pFileXMLImplement;

    public:
        FFileXMLType GetFileXMLType() const { return m_eFileXML; }
        LP_FORCEINLINE FXMLDocument* GetXMLDocument() const { return m_pXMLDocument; }

    public:
        void Destroy();

        void CreateXML();
        bool CreateXML(const String& strContent);
        bool CreateXML(const char* szContent);
        bool LoadXML(const String& strPath);
        bool SaveXML(const String& strPath);

    protected:
        void init(FFileXMLType eFileXML);
    };

}; //LostPeterFoundation

#endif