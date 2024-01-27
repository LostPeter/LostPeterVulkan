/****************************************************************************
* LostPeterEngine - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _SHADER_H_
#define _SHADER_H_

#include "Resource.h"

namespace LostPeterEngine
{
    class engineExport Shader : public Resource
    {
        friend class ShaderManager;

    public:
        Shader(ResourceManager* pResourceManager,
			   uint32 nGroup, 
			   const String& strName,
			   const String& strGroupName,
               ResourceHandle nHandle,
               bool bIsManualLoad = false,
               ResourceManualLoader* pResourceManualLoader = nullptr);
        virtual ~Shader();

    public:
		static const String ms_nameShader;

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

        
    protected:
        virtual void loadImpl();
		virtual void unloadImpl();
		virtual uint32 calculateSize() const;

    protected:
        virtual void destroyInternalResources();
			virtual void destroyInternalResourcesImpl() = 0;

		virtual bool createInternalResources();
			virtual void createInternalResourcesImpl() = 0;

	protected:
        virtual void addParameterBase();
    };

}; //LostPeterEngine

#endif