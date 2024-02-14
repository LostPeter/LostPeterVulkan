/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-02-03
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include "../include/ShaderProgram.h"
#include "../include/ShaderProgramManager.h"
#include "../include/ShaderParameter.h"

namespace LostPeterEngine
{
    ////////////////////////////// ShaderConstantDefinition ////////////////////////////
    ShaderConstantDefinition::ShaderConstantDefinition()
        : m_eShaderParamConstant()
        , m_nPhysicalIndex((std::numeric_limits<uint32>::max)())
        , m_nLogicalIndex(0)
        , m_nElementSize(0)
        , m_nArraySize(1)
        , m_nVariability(F_ShaderParamVariability_Global)
    {

    }

	ShaderConstantDefinition::~ShaderConstantDefinition()
    {

    }



    ////////////////////////////// ShaderConstantNamed /////////////////////////////////
    ShaderConstantNamed::ShaderConstantNamed()
    {

    }

	ShaderConstantNamed::~ShaderConstantNamed()
    {

    }

    uint32 ShaderConstantNamed::CalculateSize() const
    {
        uint32 nMemSize = (uint32)sizeof(*this);
        nMemSize += (uint32)(sizeof(ShaderConstantDefinition) * m_mapConstantDefinition.size());
        return nMemSize;
    }

    void ShaderConstantNamed::GenerateConstantDefinitionArrayEntries(const String& strNameParam, const ShaderConstantDefinition& constantDef)
    {
        ShaderConstantDefinition arrayDef = constantDef;
		arrayDef.m_nArraySize = 1;
		String strArrayName;

		uint32 nMaxArrayIndex = 1;
		if (constantDef.m_nArraySize <= 16)
			nMaxArrayIndex = constantDef.m_nArraySize;
		for (uint32 i = 0; i < nMaxArrayIndex; i++)
		{
			strArrayName = strNameParam + "[" + FUtilString::SaveUInt(i) + "]";
			m_mapConstantDefinition.insert(ShaderConstantDefinitionMap::value_type(strArrayName, arrayDef));
			arrayDef.m_nPhysicalIndex += arrayDef.m_nElementSize;
		}
    }

    
    ////////////////////////////// ShaderProgram ///////////////////////////////////////
    const String ShaderProgram::ms_nameShaderProgram = "ShaderProgram";
    ShaderProgram::ShaderProgram(ResourceManager* pResourceManager,
                                 uint32 nGroup, 
                                 const String& strName,
                                 const String& strGroupName,
                                 ResourceHandle nHandle,
                                 bool bIsManualLoad /*= false*/,
                                 ResourceManualLoader* pResourceManualLoader /*= nullptr*/)
        : Resource(pResourceManager,
				   nGroup, 
				   strName,
				   strGroupName,
				   nHandle,
				   bIsManualLoad,
				   pResourceManualLoader)
        , m_strPath("")
        , m_eShader(F_Shader_Vertex)
        , m_bInternalResourcesCreated(false)
        , m_pShaderParameter(nullptr)
    {
        if (createParameterDictionary(ms_nameShaderProgram))
		{
			addParameterBase();
		}
    }
        void ShaderProgram::addParameterBase()
        {

        }
        
    ShaderProgram::~ShaderProgram()
    {

    }   

    void ShaderProgram::Destroy()
    {
        
        Resource::Destroy();
    }

    ShaderParameter* ShaderProgram::GetShaderParameter()
	{
		if (nullptr == m_pShaderParameter)
		{
            m_pShaderParameter = createShaderParameter();
        }
		return m_pShaderParameter;
	}

	void ShaderProgram::SetShaderParameter(ShaderParameter* pShaderParameter)
	{
		F_Assert(pShaderParameter && "ShaderProgram::SetShaderParameter")
		F_DELETE(m_pShaderParameter)
		m_pShaderParameter = pShaderParameter;
	}

    void ShaderProgram::loadImpl()
    {
        createInternalResources();
    }

    void ShaderProgram::unloadImpl()
    {
        destroyInternalResources();
    }

    uint32 ShaderProgram::calculateSize() const
    {
        uint32 nMemSize = sizeof(*this);
        nMemSize += m_strPath.size() * sizeof(char);
        nMemSize += GetName().size() * sizeof(char);
        nMemSize += m_strSource.size() * sizeof(char);
        nMemSize += m_strSyntaxCode.size() * sizeof(char);

        uint32 nParamsSize = 0;
        if (m_pShaderParameter)
            nParamsSize += m_pShaderParameter->CalculateSize();

        return nMemSize + nParamsSize;
    }

    void ShaderProgram::destroyInternalResources()
    {
        if (m_bInternalResourcesCreated)
		{
			destroyInternalResourcesImpl();
			m_bInternalResourcesCreated = false;

			if (m_eResourceLoading.load() != E_ResourceLoading_Unloading)
            {
                m_eResourceLoading.store(E_ResourceLoading_Unloaded);
                _FireUnloadingComplete();
            }
		}
    }

    bool ShaderProgram::createInternalResources()
    {
        if (!m_bInternalResourcesCreated)
		{
			createInternalResourcesImpl();
			m_bInternalResourcesCreated = true;

			if (!IsLoading())
            {
                m_eResourceLoading.store(E_ResourceLoading_Loaded);
                _FireLoadingComplete(false);
            }
		}
		return true;
    }

    ShaderParameter* ShaderProgram::createShaderParameter()
    {
        ShaderParameter* pShaderParameter = new ShaderParameter;
		return pShaderParameter;
    }

}; //LostPeterEngine