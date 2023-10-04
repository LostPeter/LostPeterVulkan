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

#include "../include/FFileXML.h"
#include "../include/FUtilString.h"

//RapidXML
#include "rapidxml_ext.hpp"
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
using namespace rapidxml;

//TinyXML
#include "tinyxml.h"
#include "tinystr.h"

namespace LostPeterFoundation
{
    ////////////////////////// FXMLAttribute /////////////////////

    
    ////////////////////////// FXMLElement ///////////////////////
    FXMLElement::~FXMLElement()
    {
        Destroy();
    }

    void FXMLElement::Destroy()
    {
        DeleteElements(m_aElementChildren);
    }

////Element
    FXMLElement* FXMLElement::SetElementParent(FXMLElement* pElementParent)
    {
        FXMLElement* pParent = m_pElementParent;
        m_pElementParent = pElementParent;
        return pParent;
    }
    int FXMLElement::GetElementChildrenCount() const
    {
        return (int)m_aElementChildren.size();
    }
    FXMLElement* FXMLElement::GetElementChild(int index)
    {
        if (index < 0 || index >= (int)m_aElementChildren.size())
            return nullptr;
        return m_aElementChildren[index];
    }
    FXMLElement* FXMLElement::InsertElementChild(FXMLElement* pElementChild, int index)
    {
        if (index < 0 || index >= (int)m_aElementChildren.size())
            return nullptr;
        pElementChild->m_pElementParent = this;
        m_aElementChildren.insert(m_aElementChildren.begin() + index, pElementChild);
        return pElementChild;
    }
    FXMLElement* FXMLElement::AddElementChild(FXMLElement* pElementChild)
    {
        pElementChild->m_pElementParent = this;
        m_aElementChildren.push_back(pElementChild);
        return pElementChild;
    }
    FXMLElement* FXMLElement::FindElementChild(const String& strName)
    {
        size_t count = m_aElementChildren.size();
        for (size_t i = 0; i < count; i++)
        {
            FXMLElement* pChild = m_aElementChildren[i];
            if (pChild->m_strName == strName)
                return pChild;
        }
        return nullptr;
    }
    void FXMLElement::FindElementChildren(const String& strName, FXMLElementPtrVector& aElementChildren)
    {
        size_t count = m_aElementChildren.size();
        for (size_t i = 0; i < count; i++)
        {
            FXMLElement* pChild = m_aElementChildren[i];
            if (pChild->m_strName == strName)
            {
                aElementChildren.push_back(pChild);
            }
        }
    }
    void FXMLElement::RemoveElementChild(FXMLElement* pElementChild)
    {
        size_t count = m_aElementChildren.size();
        for (size_t i = 0; i < count; i++)
        {
            FXMLElement* pChild = m_aElementChildren[i];
            if (pChild == pElementChild)
            {
                m_aElementChildren.erase(m_aElementChildren.begin() + i);
                return;
            }
        }
    }
    void FXMLElement::RemoveElementChildren(const String& strName, FXMLElementPtrVector& aElementChildren)
    {
        FXMLElementPtrVector aLeft;
        size_t count = m_aElementChildren.size();
        for (size_t i = 0; i < count; i++)
        {
            FXMLElement* pChild = m_aElementChildren[i];
            if (pChild->m_strName == strName)
            {
                aElementChildren.push_back(pChild);
            }
            else
            {
                aLeft.push_back(pChild);
            }
        }
        m_aElementChildren.clear();
        m_aElementChildren = aLeft;
    }
    void FXMLElement::DeleteElementChildren(const String& strName)
    {
        FXMLElementPtrVector aDelete;
        RemoveElementChildren(strName, aDelete);
        DeleteElements(aDelete);
    }
    void FXMLElement::DeleteElements(FXMLElementPtrVector& aElements)
    {
        size_t count = aElements.size();
        for (size_t i = 0; i < count; i++)
        {
            F_DELETE(aElements[i])
        }
        aElements.clear();
    }

////Attribute
    int FXMLElement::GetAttributeCount() const
    {
        return (int)m_aAttributes.size();
    }
    FXMLAttribute* FXMLElement::GetAttribute(int index)
    {
        if (index < 0 || index >= (int)m_aAttributes.size())
            return nullptr;
        return &m_aAttributes[index];
    }
    void FXMLElement::AddAttribute(const String& strName, const String& strValue)
    {
        m_aAttributes.push_back(FXMLAttribute(strName, strValue));
    }
    void FXMLElement::AddAttribute(const FXMLAttribute& attribute)
    {
        m_aAttributes.push_back(attribute);
    }
    void FXMLElement::AddAttributes(const FXMLAttributeVector& aAttributes)
    {
        size_t count = aAttributes.size();
        for (size_t i = 0; i < count; i++)
        {
            AddAttribute(aAttributes[i]);
        }
    }
    FXMLAttribute* FXMLElement::FindAttribute(const String& strName)
    {
        size_t count = m_aAttributes.size();
        for (size_t i = 0; i < count; i++)
        {
            FXMLAttribute* pAttr = &m_aAttributes[i];
            if (pAttr->m_strName == strName)
            {
                return pAttr;
            }
        }
        return nullptr;
    }
    void FXMLElement::RemoveAttribute(const String& strName)
    {
        size_t count = m_aAttributes.size();
        for (size_t i = 0; i < count; i++)
        {
            FXMLAttribute* pAttr = &m_aAttributes[i];
            if (pAttr->m_strName == strName)
            {
                m_aAttributes.erase(m_aAttributes.begin() + i);
                return;
            }
        }
    }
    void FXMLElement::RemoveAttributes(const StringVector& aNames)
    {
        int count = (int)aNames.size();
        for (int i = 0; i < count; i++)
        {
            RemoveAttribute(aNames[i]);
        }
    }
    void FXMLElement::RemoveAttributeAll()
    {
        m_aAttributes.clear();
    }


    bool FXMLElement::ParserAttribute_Bool(const String& strName, bool& b)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        b = FUtilString::ParserBool(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Int(const String& strName, int32& i)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        i = FUtilString::ParserInt(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_UInt(const String& strName, uint32& u)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        u = FUtilString::ParserUInt(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_SizeT(const String& strName, size_t& s)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        s = FUtilString::ParserSizeT(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Float(const String& strName, float& f)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        f = FUtilString::ParserFloat(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Double(const String& strName, double& d)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        d = FUtilString::ParserDouble(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Color(const String& strName, FColor& color)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        color = FUtilString::ParserColor(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_PointI(const String& strName, FPointI& pi)
    {   
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        pi = FUtilString::ParserPointI(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_PointF(const String& strName, FPointF& pf)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        pf = FUtilString::ParserPointF(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_SizeI(const String& strName, FSizeI& si)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        si = FUtilString::ParserSizeI(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_SizeF(const String& strName, FSizeF& sf)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        sf = FUtilString::ParserSizeF(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_RectI(const String& strName, FRectI& ri)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        ri = FUtilString::ParserRectI(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_RectF(const String& strName, FRectF& rf)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        rf = FUtilString::ParserRectF(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Vector2(const String& strName, FVector2& v2)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        v2 = FUtilString::ParserVector2(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Vector3(const String& strName, FVector3& v3)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        v3 = FUtilString::ParserVector3(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Vector4(const String& strName, FVector4& v4)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        v4 = FUtilString::ParserVector4(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Quaternion(const String& strName, FQuaternion& q)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        q = FUtilString::ParserQuaternion(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Matrix3(const String& strName, FMatrix3& mat3)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        mat3 = FUtilString::ParserMatrix3(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_Matrix4(const String& strName, FMatrix4& mat4)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        mat4 = FUtilString::ParserMatrix4(pAttribute->GetValue());
        return true;
    }
    bool FXMLElement::ParserAttribute_String(const String& strName, String& str)
    {
        FXMLAttribute* pAttribute = FindAttribute(strName);
        if (pAttribute == nullptr)
            return false;
        str = pAttribute->GetValue();
        return true;
    }

    void FXMLElement::SaveAttribute_Bool(const String& strName, bool bValue)
    {
        AddAttribute(strName, FUtilString::SaveBool(bValue));
    }
    void FXMLElement::SaveAttribute_Int(const String& strName, int32 nValue)
    {
        AddAttribute(strName, FUtilString::SaveInt(nValue));
    }
    void FXMLElement::SaveAttribute_UInt(const String& strName, uint32 nValue)
    {
        AddAttribute(strName, FUtilString::SaveUInt(nValue));
    }
    void FXMLElement::SaveAttribute_SizeT(const String& strName, size_t sValue)
    {   
        AddAttribute(strName, FUtilString::SaveSizeT(sValue));
    }
    void FXMLElement::SaveAttribute_Float(const String& strName, float fValue)
    {
        AddAttribute(strName, FUtilString::SaveFloat(fValue));
    }
    void FXMLElement::SaveAttribute_Double(const String& strName, double dValue)
    {
        AddAttribute(strName, FUtilString::SaveDouble(dValue));
    }
    void FXMLElement::SaveAttribute_Color(const String& strName, const FColor& clValue)
    {
        AddAttribute(strName, FUtilString::SaveColor(clValue));
    }
    void FXMLElement::SaveAttribute_PointI(const String& strName, const FPointI& pointI)
    {
        AddAttribute(strName, FUtilString::SavePointI(pointI));
    }
    void FXMLElement::SaveAttribute_PointF(const String& strName, const FPointF& pointF)
    {
        AddAttribute(strName, FUtilString::SavePointF(pointF));
    }
    void FXMLElement::SaveAttribute_SizeI(const String& strName, const FSizeI& sizeI)
    {
        AddAttribute(strName, FUtilString::SaveSizeI(sizeI));
    }
    void FXMLElement::SaveAttribute_SizeF(const String& strName, const FSizeF& sizeF)
    {
        AddAttribute(strName, FUtilString::SaveSizeF(sizeF));
    }
    void FXMLElement::SaveAttribute_RectI(const String& strName, const FRectI& rectI)
    {
        AddAttribute(strName, FUtilString::SaveRectI(rectI));
    }
    void FXMLElement::SaveAttribute_RectF(const String& strName, const FRectF& rectF)
    {
        AddAttribute(strName, FUtilString::SaveRectF(rectF));
    }
    void FXMLElement::SaveAttribute_Vector2(const String& strName, const FVector2& v2Value)
    {
        AddAttribute(strName, FUtilString::SaveVector2(v2Value));
    }
    void FXMLElement::SaveAttribute_Vector3(const String& strName, const FVector3& v3Value)
    {
        AddAttribute(strName, FUtilString::SaveVector3(v3Value));
    }
    void FXMLElement::SaveAttribute_Vector4(const String& strName, const FVector4& v4Value)
    {
        AddAttribute(strName, FUtilString::SaveVector4(v4Value));
    }
    void FXMLElement::SaveAttribute_Quaternion(const String& strName, const FQuaternion& qValue)
    {
        AddAttribute(strName, FUtilString::SaveQuaternion(qValue));
    }
    void FXMLElement::SaveAttribute_Matrix3(const String& strName, const FMatrix3& mat3Value)
    {   
        AddAttribute(strName, FUtilString::SaveMatrix3(mat3Value));
    }
    void FXMLElement::SaveAttribute_Matrix4(const String& strName, const FMatrix4& mat4Value)
    {
        AddAttribute(strName, FUtilString::SaveMatrix4(mat4Value));
    }
    void FXMLElement::SaveAttribute_String(const String& strName, const String& strValue)
    {
        AddAttribute(strName, strValue);
    }


    ////////////////////////// FXMLDocument //////////////////////
    FXMLDocument::FXMLDocument()
        : m_pElementDeclaration(nullptr) 
        , m_pElementRoot(nullptr)
    {

    }

    FXMLDocument::~FXMLDocument()
    {
        Destroy();
    }

    void FXMLDocument::Destroy()
    {
        F_DELETE(m_pElementDeclaration)
        F_DELETE(m_pElementRoot)
    }

    FXMLElement* FXMLDocument::CreateElementDeclaration(const String& strVersion /*= "1.0"*/, const String& strEncoding /*= "utf-8"*/, const String& strStandalone /*= ""*/)
    {
        F_DELETE(m_pElementDeclaration)
        m_pElementDeclaration = new FXMLElement();
        if (!strVersion.empty())
        {
            FXMLAttribute attrVersion("version", strVersion);
            m_pElementDeclaration->AddAttribute(attrVersion);
        }
        if (!strEncoding.empty())
        {
            FXMLAttribute attrEncoding("encoding", strEncoding);
            m_pElementDeclaration->AddAttribute(attrEncoding);
        }
        if (!strStandalone.empty())
        {
            FXMLAttribute attrStandalone("standalone", strStandalone);
            m_pElementDeclaration->AddAttribute(attrStandalone);
        }
        return m_pElementDeclaration;
    }

    FXMLElement* FXMLDocument::CreateElementRoot(const String& strName)
    {
        F_DELETE(m_pElementRoot)
        m_pElementRoot = new FXMLElement(strName);
        return m_pElementRoot;
    }

    FXMLElement* FXMLDocument::CreateElement(FXMLElement* pElementParent)
    {
        FXMLElement* pElement = new FXMLElement();
        pElement->m_pElementParent = pElementParent;
        return pElement;
    }
    
    ////////////////////////// FFileXMLImplement_RapidXML /////////
    class FFileXMLImplement_RapidXML : public FFileXMLImplement
    {
    public:
        FFileXMLImplement_RapidXML()
        {

        }
        virtual ~FFileXMLImplement_RapidXML()
        {

        }

    public:
        static void s_ParseAttributes(rapidxml::xml_node<>* pNode, FXMLElement* pElement)
        {
            rapidxml::xml_attribute<>* pAttribute = pNode->first_attribute();
            while (pAttribute != nullptr)
            {
                pElement->AddAttribute(String(pAttribute->name(), pAttribute->name_size()), 
                                       String(pAttribute->value(), pAttribute->value_size()));
                pAttribute = pAttribute->next_attribute();
            }
        }
        static void s_ParseNode(rapidxml::xml_node<>* pNode, FXMLElement* pElement)
        {
            s_ParseAttributes(pNode, pElement);

            rapidxml::xml_node<>* pNodeChild = pNode->first_node();
            while (pNodeChild != nullptr)
            {
                FXMLElement* pElementChild = pElement->AddElementChild(new FXMLElement(String(pNodeChild->name(), pNodeChild->name_size())));
                s_ParseNode(pNodeChild, pElementChild);

                pNodeChild = pNodeChild->next_sibling();
            }
        }

        static void s_AddAttributes(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNode, FXMLElement* pElement)
        {
            FXMLAttributeVector& atrributes = pElement->GetAttributes();
            size_t count = atrributes.size();
            for (size_t i = 0; i < count; i++)
            {
                FXMLAttribute& attribute = atrributes[i];
                pNode->append_attribute(doc.allocate_attribute(attribute.GetName().c_str(), attribute.GetValue().c_str()));
            }
        }
        static rapidxml::xml_node<>* s_AddNode(rapidxml::xml_document<>& doc, rapidxml::xml_node<>* pNodeParent, FXMLElement* pElement, rapidxml::node_type type = rapidxml::node_element)
        {
            rapidxml::xml_node<>* pChild = doc.allocate_node(type, pElement->GetName().c_str());
            pNodeParent->append_node(pChild);
            s_AddAttributes(doc, pChild, pElement);
            FXMLElementPtrVector& aElements = pElement->GetElementChildren();
            size_t count = aElements.size();
            for (size_t i = 0; i < count; i++)
            {
                FXMLElement* pCC = aElements[i];
                s_AddNode(doc, pChild, pCC);
            }
            return pChild;
        }
        static rapidxml::xml_node<>* s_AddRootNode(rapidxml::xml_document<>& doc, FXMLElement* pElement, rapidxml::node_type type)
        {
            rapidxml::xml_node<>* pNode = doc.allocate_node(type, pElement->GetName().c_str());
            doc.append_node(pNode);
            s_AddAttributes(doc, pNode, pElement);
            FXMLElementPtrVector& aElements = pElement->GetElementChildren();
            size_t count = aElements.size();
            for (size_t i = 0; i < count; i++)
            {
                FXMLElement* pChild = aElements[i];
                s_AddNode(doc, pNode, pChild);
            }
            return pNode;
        }

    public:
        virtual void CreateXML()
        {

        }

        virtual bool ParseXML(const char* szContent, FXMLDocument* pDocument)
        {
            rapidxml::xml_document<> doc;
            doc.parse<0>(const_cast<char*>(szContent));

            rapidxml::xml_node<>* pNode = doc.first_node();
            while (pNode != nullptr)
            {
                switch (pNode->type())
                {
                case rapidxml::node_type::node_declaration:
                    {
                        FXMLElement* pElementDeclaration = pDocument->CreateElementDeclaration("", "", "");
                        pElementDeclaration->SetName(String(pNode->name(), pNode->name_size()));
                        s_ParseNode(pNode, pElementDeclaration);
                    }
                    break;
                case rapidxml::node_type::node_element:
                    {
                        FXMLElement* pElementRoot = pDocument->CreateElementRoot(String(pNode->name(), pNode->name_size()));
                        s_ParseNode(pNode, pElementRoot);
                    }
                    break;
                default:
                    break;
                }

                pNode = pNode->next_sibling();
            }
            return true;
        }

        virtual bool LoadXML(const String& strPath, FXMLDocument* pDocument)
        {
            rapidxml::file<> file(strPath.c_str());
            if (file.data() == nullptr)
            {
                return false;
            }
            return ParseXML(file.data(), pDocument);
        }

        virtual bool SaveXML(const String& strPath, FXMLDocument* pDocument)
        {
            if (pDocument == nullptr)
                return false;
            
            FXMLElement* pElementDeclaration = pDocument->GetElementDeclaration();
            FXMLElement* pElementRoot = pDocument->GetElementRoot();
            if (pElementDeclaration == nullptr ||
                pElementRoot == nullptr)
            {
                return false;
            }

            //1> Doc
            rapidxml::xml_document<> doc;
            //2> Declaration
            rapidxml::xml_node<>* declaration = s_AddRootNode(doc, pElementDeclaration, rapidxml::node_declaration);
            //3> Node
            rapidxml::xml_node<>* root = s_AddRootNode(doc, pElementRoot, rapidxml::node_element);
            //4> Save file
            std::ofstream outFile(strPath.c_str(), std::ios::out);
            if (outFile)
            {
                outFile << doc;
                outFile.close();
                return true;
            }
            return false;
        }
    };


    ////////////////////////// FFileXMLImplement_TinyXML /////////
    class FFileXMLImplement_TinyXML : public FFileXMLImplement
    {
    public:
        FFileXMLImplement_TinyXML()
        {

        }
        virtual ~FFileXMLImplement_TinyXML()
        {

        }

    public:
        virtual void CreateXML()
        {
            
        }
        virtual bool ParseXML(const char* szContent, FXMLDocument* pDocument)
        {
            return true;
        }
        virtual bool LoadXML(const String& strPath, FXMLDocument* pDocument)
        {
            return true;
        }
        virtual bool SaveXML(const String& strPath, FXMLDocument* pDocument)
        {
            return true;
        }
    };


    ////////////////////////// FFileXML //////////////////////////
    FFileXML::FFileXML()
        : m_eFileXML(F_FileXML_RapidXML)
        , m_pXMLDocument(nullptr)
    {
        init(m_eFileXML);
    }

    FFileXML::FFileXML(FFileXMLType eFileXML)
        : m_eFileXML(eFileXML)
        , m_pXMLDocument(nullptr)
    {
        init(m_eFileXML);
    }

    FFileXML::~FFileXML()
    {
        Destroy();
    }

    void FFileXML::init(FFileXMLType eFileXML)
    {
        m_pXMLDocument = new FXMLDocument();
        switch ((int32)eFileXML)
        {
        case F_FileXML_RapidXML:
            m_pFileXMLImplement = new FFileXMLImplement_RapidXML();
            break;
        case F_FileXML_TinyXML:
            m_pFileXMLImplement = new FFileXMLImplement_TinyXML();
            break;
        default:
            F_Assert(false && "FFileXML::init: Wrong FFileXMLType type !")
        }   
    }

    void FFileXML::Destroy()
    {
        F_DELETE(m_pXMLDocument)
        F_DELETE(m_pFileXMLImplement)
    }

    void FFileXML::CreateXML()
    {
        Destroy();
        m_pXMLDocument = new FXMLDocument();
        m_pFileXMLImplement->CreateXML();
    }
    bool FFileXML::CreateXML(const String& strContent)
    {
        return CreateXML(strContent.c_str());
    }
    bool FFileXML::CreateXML(const char* szContent)
    {
        CreateXML();
        return m_pFileXMLImplement->ParseXML(szContent, m_pXMLDocument);
    }

    bool FFileXML::LoadXML(const String& strPath)
    {
        CreateXML();
        return m_pFileXMLImplement->LoadXML(strPath, m_pXMLDocument);
    }

    bool FFileXML::SaveXML(const String& strPath)
    {
        return m_pFileXMLImplement->SaveXML(strPath, m_pXMLDocument);
    }
    
}; //LostPeterFoundation