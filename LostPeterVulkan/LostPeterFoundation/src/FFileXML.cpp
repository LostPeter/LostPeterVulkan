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

    void FXMLElement::AddAttribute(const String& strName, const String& strValue)
    {
        m_aAttributes.push_back(FXMLAttribute(strName, strValue));
    }

    void FXMLElement::AddAttribute(FXMLAttribute& attribute)
    {
        m_aAttributes.push_back(attribute);
    }

    void FXMLElement::AddAttributes(FXMLAttributeVector& aAttributes)
    {
        size_t count = aAttributes.size();
        for (size_t i = 0; i < count; i++)
        {
            AddAttribute(aAttributes[i]);
        }
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