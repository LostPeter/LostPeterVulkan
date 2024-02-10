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

#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include "Resource.h"

namespace LostPeterEngine
{
    ////////////////////////////// ShaderConstantDefinition ////////////////////////////
	class engineExport ShaderConstantDefinition
	{
	public:
		ShaderConstantDefinition();
		~ShaderConstantDefinition();

    public:
		FShaderParamConstantType m_eShaderParamConstant;
		uint32 m_nPhysicalIndex;
        uint32 m_nLogicalIndex;
		uint32 m_nElementSize;
		uint32 m_nArraySize;
        mutable uint16 m_nVariability;

    public:
        F_FORCEINLINE bool IsFloat() const { return F_ShaderParamConstant_IsFloat(m_eShaderParamConstant); }
		F_FORCEINLINE bool IsInt() const { return F_ShaderParamConstant_IsInt(m_eShaderParamConstant); }
        F_FORCEINLINE bool IsDouble() const { return F_ShaderParamConstant_IsDouble(m_eShaderParamConstant); }
        F_FORCEINLINE bool IsUint() const { return F_ShaderParamConstant_IsUint(m_eShaderParamConstant); }
        F_FORCEINLINE bool IsBool() const { return F_ShaderParamConstant_IsBool(m_eShaderParamConstant); }
        F_FORCEINLINE bool IsSampler() const { return F_ShaderParamConstant_IsSampler(m_eShaderParamConstant); }
        F_FORCEINLINE bool IsSpecialization() const { return F_ShaderParamConstant_IsSpecialization(m_eShaderParamConstant); }
	};
    typedef std::map<String, ShaderConstantDefinition> ShaderConstantDefinitionMap;

    
    ////////////////////////////// ShaderConstantNamed /////////////////////////////////
    class engineExport ShaderConstantNamed
    {
    public:
        ShaderConstantNamed();
        ~ShaderConstantNamed();

    public:
        uint32 m_nBufferSize;
        uint32 m_nRegisterCount;
        ShaderConstantDefinitionMap m_mapConstantDefinition;

    public:
        uint32 CalculateSize() const;

    public:
        void GenerateConstantDefinitionArrayEntries(const String& strNameParam, const ShaderConstantDefinition& constantDef);
    };

    
    ////////////////////////////// ShaderLogical/Physical //////////////////////////////
    struct engineExport ShaderLogicalIndexUsed
	{
		ShaderLogicalIndexUsed(uint32 _nPhysicalIndex, uint32 _nCurrentSize) 
			: nPhysicalIndex(_nPhysicalIndex)
			, nCurrentSize(_nCurrentSize) 
		{
	
		}

		uint32 nPhysicalIndex;
		uint32 nCurrentSize;
	};
    typedef std::map<uint32, ShaderLogicalIndexUsed> ShaderLogicalIndexUsedMap;

    struct engineExport ShaderLogicalBufferStruct
	{
		ShaderLogicalBufferStruct()
			: nBufferSize(0)
		{

		}

		uint32 nBufferSize;
        ShaderLogicalIndexUsedMap mapLogicalIndexUsed;
	};

    struct engineExport ShaderPhysicalIndexUsed
	{
		ShaderPhysicalIndexUsed(uint32 _nLogicalIndex, uint32 _nCurrentSize)
			: nLogicalIndex(_nLogicalIndex)
			, nCurrentSize(_nCurrentSize)
		{

		}

		uint32 nLogicalIndex;
		uint32 nCurrentSize;		
	};
    typedef std::map<uint32, ShaderPhysicalIndexUsed> ShaderPhysicalIndexUsedMap;

    struct engineExport ShaderPhysicalBufferStruct
	{
		ShaderPhysicalBufferStruct()
			: nBufferSize(0)
		{

		}

        size_t nBufferSize;
		ShaderPhysicalIndexUsedMap mapPhysicalIndexUsed;
	};


    ////////////////////////////// ShaderProgram ///////////////////////////////////////
    class engineExport ShaderProgram : public Resource
    {
        friend class ShaderProgramManager;

    public:
        ShaderProgram(ResourceManager* pResourceManager,
                      uint32 nGroup, 
                      const String& strName,
                      const String& strGroupName,
                      ResourceHandle nHandle,
                      bool bIsManualLoad = false,
                      ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~ShaderProgram();

    public:
		static const String ms_nameProgramShader;

    public:
        String m_strPath;

        FShaderType m_eShader;
        String m_strSource;
        String m_strSyntaxCode;

        bool m_bIsLoadFromFile;
        bool m_bHasSkeletalAnimation;
        bool m_bHasMorphAnimation;
        bool m_bIsVertexTextureFetch;
        bool m_bIsNeedsAdjacencyInfo;
        bool m_bHasCompileError;
        bool m_bIsLoadedManualNamedConstants;

        bool m_bInternalResourcesCreated;

        ShaderParameter* m_pShaderParameter;

        mutable ShaderLogicalBufferStruct m_FloatLogicalToPhysical;
		mutable ShaderLogicalBufferStruct m_IntLogicalToPhysical;
		mutable ShaderPhysicalBufferStruct m_FloatPhysicalToLogical;

    public:
        F_FORCEINLINE const String& GetPath() const { return m_strPath; }
		F_FORCEINLINE void SetPath(const String& strPath) { m_strPath = strPath; }
        F_FORCEINLINE FShaderType GetShaderType() const { return m_eShader; }
		F_FORCEINLINE void SetShaderType(FShaderType eShader) { m_eShader = eShader; }
        F_FORCEINLINE String GetSource() const { return m_strSource; }
		F_FORCEINLINE void SetSource(const String& strSource) { m_strSource = strSource; }
        F_FORCEINLINE String GetSyntaxCode() const { return m_strSyntaxCode; }
		F_FORCEINLINE void SetSyntaxCode(const String& strSyntaxCode) { m_strSyntaxCode = strSyntaxCode; }

    public:
        virtual void Destroy();


    public:
        virtual bool HasShaderParameter() const { return nullptr != m_pShaderParameter; }
		virtual ShaderParameter* GetShaderParameter();
		virtual void SetShaderParameter(ShaderParameter* pShaderParameter);

        
    protected:
        virtual void loadImpl();
		virtual void unloadImpl();
		virtual uint32 calculateSize() const;

    protected:
        virtual void destroyInternalResources();
			virtual void destroyInternalResourcesImpl() = 0;

		virtual bool createInternalResources();
			virtual void createInternalResourcesImpl() = 0;

        virtual ShaderParameter* createShaderParameter();

	protected:
        virtual void addParameterBase();
		
    };

}; //LostPeterEngine

#endif