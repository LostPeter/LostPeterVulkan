/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2024-01-27
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_PARAM_DEFINE_H_
#define _SHADER_PARAM_DEFINE_H_

#include "Base.h"

namespace LostPeterEngine
{
    ////////////////////////////// ShaderConstantRegister //////////////////////////////
    class engineExport ShaderConstantRegister
	{
	public:
		ShaderConstantRegister();

	public:
		float m_fBuf[4];
	};


    ////////////////////////////// ShaderConstantEntryAuto /////////////////////////////
    class engineExport ShaderConstantEntryAuto
	{
	public:
		ShaderConstantEntryAuto(FShaderParamConstantAutoType eShaderParamConstantAuto, 
                                uint32 nIndex, 
                                uint32 nData,
			                    uint32 nElementCount,
                                bool bIsArray = false);
		ShaderConstantEntryAuto(FShaderParamConstantAutoType eShaderParamConstantAuto, 
                                uint32 nIndex,
                                float fData,
			                    uint32 nElementCount);

		bool operator ==(const ShaderConstantEntryAuto& src);

	public:
		FShaderParamConstantAutoType m_eShaderParamConstantAuto;
		uint32 m_nElementCount;
		uint32 m_nRegisterIndex;
		uint32 m_nRegisterCount;

		union
		{
			uint32 m_nData;
			float m_fData;
		};
	};

    struct engineExport ShaderConstantEntryAutoCompare : public std::binary_function<ShaderConstantEntryAuto*, ShaderConstantEntryAuto*, bool>
	{
	public:
		bool operator ()(const ShaderConstantEntryAuto* pSrc1, const ShaderConstantEntryAuto* pSrc2) const
		{
			return pSrc1->m_nRegisterIndex < pSrc2->m_nRegisterIndex;
		}
	};
	typedef std::vector<ShaderConstantEntryAuto*> SahderConstantEntryAutoPtrVector;
	typedef std::vector<ShaderConstantEntryAuto> SahderConstantEntryAutoVector;

    
    ////////////////////////////// ShaderConstantEntryManual ///////////////////////////
    class engineExport ShaderConstantEntryManual : public Base
	{
	public:
		ShaderConstantEntryManual(const String& strName, 
                                  uint32 nIndex, 
                                  const float* pData, 
                                  uint32 nDataCount);
		ShaderConstantEntryManual(const ShaderConstantEntryManual& src);
		virtual ~ShaderConstantEntryManual();

	public:
		uint32 m_nRegisterIndex;
		uint32 m_nRegisterCount;
		ShaderConstantRegister* m_pRegisterData;

	public:
		void SetManualConstantEntryData(const float* pData, uint32 nDataCount);
	};

	struct engineExport ShaderConstantEntryManualCompare : public std::binary_function<ShaderConstantEntryManual*, ShaderConstantEntryManual*, bool>
	{
	public:
		bool operator ()(const ShaderConstantEntryManual* pSrc1, const ShaderConstantEntryManual* pSrc2) const
		{
			return pSrc1->m_nRegisterIndex < pSrc2->m_nRegisterIndex;
		}
	};
	typedef std::vector<ShaderConstantEntryManual*> SahderConstantEntryManualPtrVector;
	typedef std::vector<ShaderConstantEntryManual*> SahderManualConstantEntryVector;

	
    ////////////////////////////// ShaderParameter /////////////////////////////////////
    class engineExport ShaderConstantAutoDefinition
    {
    public:
        ShaderConstantAutoDefinition(FShaderParamConstantAutoType eShaderParamConstantAuto,
                                     FShaderParamConstantDataType eShaderParamConstantData,
                                     uint32 nElementCount,
                                     bool bIsArray)
            : m_eShaderParamConstantAuto(eShaderParamConstantAuto)
            , m_eShaderParamConstantData(eShaderParamConstantData)
            , m_nElementCount(nElementCount)
            , m_bIsArray(bIsArray)
        {

        }

    public:
        FShaderParamConstantAutoType m_eShaderParamConstantAuto;
        FShaderParamConstantDataType m_eShaderParamConstantData;
        uint32 m_nElementCount;
        bool m_bIsArray;
    };
    typedef std::map<String, ShaderConstantAutoDefinition*>	Name2ShaderConstantAutoDefinitionPtrMap;


    class engineExport ShaderParameter
	{
	public:
		ShaderParameter();
		ShaderParameter(uint32 nStartRangeIndex, uint32 nEndRangeIndex);
		~ShaderParameter();

		ShaderParameter& operator= (const ShaderParameter& src);

	public:
        static ShaderConstantAutoDefinition	ms_aShaderConstantAutoDefinitions[];
		static Name2ShaderConstantAutoDefinitionPtrMap m_mapName2ShaderConstantAutoDefinition;

    public:
        static uint32 GetShaderConstantAutoDefinitionCount();
		static const ShaderConstantAutoDefinition* GetConstantAutoDefinition(const String& strName);
        static const ShaderConstantAutoDefinition* GetConstantAutoDefinition(uint32 nIndex);
		static uint32 GetDefaultRangeIndexStart();
		static uint32 GetDefaultRangeIndexEnd();

    protected:
        static void initParamStaticMap();
		
	public:
	protected:
		uint32 m_nStartRangeIndex;
		uint32 m_nEndRangeIndex;
		bool m_bIsTransposeMatrix;
		ShaderProgramGroup* m_pShaderProgramGroup;

		SahderConstantEntryAutoPtrVector m_aShaderConstantEntryAuto;
		SahderConstantEntryManualPtrVector m_aShaderConstantEntryManual;

	public:
		F_FORCEINLINE bool GetIsTransposeMatrix() { return m_bIsTransposeMatrix; }
		F_FORCEINLINE void SetIsTransposeMatrix(bool bIsTransposeMatrix) { m_bIsTransposeMatrix = bIsTransposeMatrix; }
		F_FORCEINLINE void SetShaderProgramGroup(ShaderProgramGroup* pShaderProgramGroup) { m_pShaderProgramGroup = pShaderProgramGroup; }
		F_FORCEINLINE const SahderConstantEntryAutoPtrVector& GetSahderConstantEntryAutoPtrVector() const { return m_aShaderConstantEntryAuto; }
		F_FORCEINLINE const SahderConstantEntryManualPtrVector& GetSahderConstantEntryManualPtrVector() const { return m_aShaderConstantEntryManual; }

        F_FORCEINLINE uint32 GetConstantEntryAutoCount() const { return (uint32)m_aShaderConstantEntryAuto.size(); }
        F_FORCEINLINE uint32 GetConstantEntryManualCount() const { return (uint32)m_aShaderConstantEntryManual.size(); }

	public:
		bool IsRegisterUsed(uint32 nRegisterIndex, uint32 nRegisterCount);
		void SetRegisterRange(uint32 nStartRangeIndex, uint32 nEndRangeIndex);

	public:
		ShaderConstantEntryAuto* GetConstantAutoByIndex(uint32 nIndex);

		const String& GetConstantManualName(uint32 nIndex);
		ShaderConstantEntryManual* GetConstantManualByIndex(uint32 nIndex);
		ShaderConstantEntryManual* GetConstantManualByName(const String& strName);
		bool SetConstantManualByName(const String& strName, const float* pData, uint32 nDataCount);
		bool SetConstantManualByIndex(uint32 nBeginIndex, const float* pData, uint32 nDataCount);
		bool SetConstantManualByIndex(uint32 nBeginIndex, const FVector4& v4);

		bool InsertConstantAutoInt(uint32 nRegisterIndex, 
                                   FShaderParamConstantAutoType eShaderParamConstantAuto, 
                                   uint32 nData, 
                                   uint32 nElementSize, 
                                   bool bIsArray = false);
		bool InsertConstantAutoFloat(uint32 nRegisterIndex, 
                                     FShaderParamConstantAutoType eShaderParamConstantAuto, 
                                     float fData, 
                                     uint32 nElementSize);
		bool InsertConstantManual(uint32 nRegisterIndex,
                                  const String& strName,
                                  float* pData,
                                  uint32 nDataCount);
		
		void CopyFrom(const ShaderParameter& src);

	public:
		void UpdateParamAll(const ShaderParamSourceAuto* pParamSourceAuto);
		void UpdateParamRange(const ShaderParamSourceAuto* pParamSourceAuto, uint32 nStartRegisterIndex, uint32 nEndRegisterIndex);

        void UpdateParamAutos(const ShaderParamSourceAuto* pParamSourceAuto, uint32 nBeginIndex = 0, uint32 nEndIndex = (uint32)(-1));
		void UpdateParamManuals(uint32 nBeginIndex = 0, uint32 nEndIndex = (uint32)(-1));

	private:	
		void clearConstantManuals();
	};

}; //LostPeterEngine

#endif