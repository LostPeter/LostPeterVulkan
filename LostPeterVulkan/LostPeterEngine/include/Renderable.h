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

#ifndef _RENDERABLE_H_
#define _RENDERABLE_H_

#include "Base.h"

namespace LostPeterEngine
{
    class engineExport Renderable : public Base
    {
    public:
        Renderable(const String& nameRenderable);
        virtual ~Renderable();

    public:
    protected:
        EObjectType m_eObject;
        FRenderQueueType m_eRenderQueue;
        FRenderPassType m_eRenderPass;
        int32 m_nRendOrderID;

        bool m_bIsPolygonModeOverrideable;
		bool m_bIsUseIdentityView;
		bool m_bIsUseIdentityProjection;

        uint32 m_nGroup;
        String m_strMaterialName;
		MaterialInstance* m_pMaterialInstance;
		MaterialManager* m_pMaterialManager;

        FIndex2Vector4Map* m_aShaderParams[F_Shader_Count];

    public:
        F_FORCEINLINE EObjectType GetObjectType() const { return m_eObject; }
        F_FORCEINLINE void SetObjectType(EObjectType eObject) { m_eObject = eObject; }
        F_FORCEINLINE FRenderQueueType GetRenderQueueType() const { return m_eRenderQueue; }
        F_FORCEINLINE void SetRenderQueueType(FRenderQueueType eRenderQueue) { m_eRenderQueue = eRenderQueue; }
        F_FORCEINLINE FRenderPassType GetRenderPassType() const { return m_eRenderPass; }
        F_FORCEINLINE void SetRenderPassType(FRenderPassType eRenderPass) { m_eRenderPass = eRenderPass; }
        F_FORCEINLINE int32 GetRendOrderID() const { return m_nRendOrderID; }
		F_FORCEINLINE void SetRendOrderID(int32 nRendOrderID) { m_nRendOrderID = nRendOrderID; }

        F_FORCEINLINE bool GetIsPolygonModeOverrideable() const	{ return m_bIsPolygonModeOverrideable; }
		F_FORCEINLINE void SetIsPolygonModeOverrideable(bool bIsPolygonModeOverrideable) { m_bIsPolygonModeOverrideable = bIsPolygonModeOverrideable; }
		F_FORCEINLINE bool GetIsUseIdentityView() const	{ return m_bIsUseIdentityView; }
		F_FORCEINLINE void SetIsUseIdentityView(bool bIsUseIdentityView) { m_bIsUseIdentityView = bIsUseIdentityView; }
		F_FORCEINLINE bool GetIsUseIdentityProjection() const { return m_bIsUseIdentityProjection; }
		F_FORCEINLINE void SetIsUseIdentityProjection(bool bIsUseIdentityProjection) { m_bIsUseIdentityProjection = bIsUseIdentityProjection; }

		F_FORCEINLINE uint32 GetGroup() const { return m_nGroup; }
        F_FORCEINLINE const String& GetMaterialName() const { return m_strMaterialName; }

    public:


    public:
        virtual MaterialInstance* GetMaterialInstance();
		virtual const MaterialInstance* GetMaterialInstance() const;
        virtual bool SetMaterialInstance(MaterialInstance* pMaterialInstance);
		virtual bool SetMaterialName(const String& strMaterialName, bool bIsUnique = true);
		virtual bool SetMaterial(const String& strMaterialName, uint32 nGroup, bool bIsUnique = true);
		virtual bool SetMaterial(Material* pMaterial);

        virtual bool GetVertexIndexData(DataVertexIndex& dataVI) = 0;
		virtual bool GetWorldTransforms(FMatrix4* pWorldMatrix, uint32& nCount) const = 0;
		virtual float GetSquaredViewDepth(const ObjectCamera* pCamera) const = 0;

		virtual void UpdateShaderParam(Shader* pShader);
        virtual void UpdateShaderParam(int nIndex, Shader* pShader);
    };

}; //LostPeterEngine

#endif