/****************************************************************************
* LostPeterVulkan - Copyright (C) 2022 by LostPeter
* 
* Author:   LostPeter
* Time:     2023-10-04
* Github:   https://github.com/LostPeter/LostPeterVulkan
* Document: https://www.zhihu.com/people/lostpeter/posts
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#ifndef _EDITOR_LINE_FLAT_3D_COLLECTOR_H_
#define _EDITOR_LINE_FLAT_3D_COLLECTOR_H_

#include "EditorBase.h"

namespace LostPeterVulkan
{
    class vulkanExport EditorLineFlat3DCollector : public EditorBase
    {
    public:
        EditorLineFlat3DCollector();
        virtual ~EditorLineFlat3DCollector();

    public:
        //Line
        static const String c_strLine3D_Line;
        static const String c_strLine3D_Triangle;
        static const String c_strLine3D_Quad;
        static const String c_strLine3D_Grid;
        static const String c_strLine3D_Quad_Convex;
        static const String c_strLine3D_Quad_Concave;
        static const String c_strLine3D_Circle;
        static const String c_strLine3D_AABB;
        static const String c_strLine3D_Sphere;
        static const String c_strLine3D_Cylinder;
        static const String c_strLine3D_Capsule;
        static const String c_strLine3D_Cone;
        static const String c_strLine3D_Torus;

        //Flat
        static const String c_strFlat3D_Triangle;
        static const String c_strFlat3D_Quad;
        static const String c_strFlat3D_Quad_Convex;
        static const String c_strFlat3D_Quad_Concave;
        static const String c_strFlat3D_Circle;
        static const String c_strFlat3D_AABB;
        static const String c_strFlat3D_Sphere;
        static const String c_strFlat3D_Cylinder;
        static const String c_strFlat3D_Capsule;
        static const String c_strFlat3D_Cone;
        static const String c_strFlat3D_Torus;

    public:
        static const String s_strNameShader_LineFlat3D_Vert;
        static const String s_strNameShader_LineFlat3D_Frag;
        static const String s_strNameShader_LineFlat3D_Ext_Vert;
        static const String s_strNameShader_LineFlat3D_Ext_Frag;

    public:
        MeshPtrMap mapName2Mesh;

    protected:
        bool isBufferUniform;

    protected:
        ////////////////////// BufferBaseLineFlat3D /////////////////////////
        class BufferBaseLineFlat3D
        {
        public:
            BufferBaseLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh* p, bool bIsLineList);
            virtual ~BufferBaseLineFlat3D();

        public:
            EditorLineFlat3DCollector* pLineFlat3DCollector;
            Mesh* pMesh;
            int nObjectCount;

            VkDescriptorSetVector poDescriptorSets;
            bool isLineList;

        public:
            virtual void Destroy() = 0;
            virtual void Init() = 0;

        public:
            virtual void Clear() = 0;

            virtual PointerBuffer* AddLineFlat3DObject(const FMatrix4& mat, const FColor& color, bool isUpdateBuffer = true) = 0;
            virtual PointerBuffer* AddLineFlat3DObject(const LineFlat3DObjectConstants& object, bool isUpdateBuffer = true) = 0;
            virtual PointerBuffer* AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants>& objects, bool isUpdateBuffer = true) = 0;

            virtual void RemoveLineFlat3DObject(PointerBuffer* pPointer, bool isUpdateBuffer = true) = 0;
            virtual void UpdateBuffer() = 0;

            virtual void CleanupSwapChain() = 0;
            virtual void RecreateSwapChain() = 0;
            virtual void UpdateDescriptorSets() = 0;
        };


        ////////////////////// BufferStorageLineFlat3D //////////////////////
        class BufferStorageLineFlat3D : public BufferBaseLineFlat3D
        {
            friend class EditorLineFlat3DCollector;

        public:
            BufferStorageLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh* p, bool bIsLineList);
            virtual ~BufferStorageLineFlat3D();

        public:
            static const int s_nStepCount;

        public:
            BufferStorage* pBufferStorage;
            int nObjectCountMax;

        protected:
            ObjectManagedPool<PointerBuffer>* pPointerBufferPool;

        public:
            virtual void Destroy();
            virtual void Init();

        public:
            virtual void Clear();

            virtual PointerBuffer* AddLineFlat3DObject(const FMatrix4& mat, const FColor& color, bool isUpdateBuffer = true);
            virtual PointerBuffer* AddLineFlat3DObject(const LineFlat3DObjectConstants& object, bool isUpdateBuffer = true);
            virtual PointerBuffer* AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants>& objects, bool isUpdateBuffer = true);

            virtual void RemoveLineFlat3DObject(PointerBuffer* pPointer, bool isUpdateBuffer = true);
            virtual void UpdateBuffer();

            virtual void CleanupSwapChain();
            virtual void RecreateSwapChain();
            virtual void UpdateDescriptorSets();

        protected:
            BufferStorage* createBufferStorage(int count);
            void increaseBufferStorage();
        };
        typedef std::vector<BufferStorageLineFlat3D*> BufferStorageLineFlat3DPtrVector;
        typedef std::map<Mesh*, BufferStorageLineFlat3D*> Mesh2BufferStorageLineFlat3DPtrMap;

        BufferStorageLineFlat3DPtrVector aBufferLineFlat3D;
        Mesh2BufferStorageLineFlat3DPtrMap mapMesh2BufferLineFlat3D;

        //PipelineGraphics-Uniform
        String nameDescriptorSetLayout_Uniform;
        StringVector aNamesDescriptorSetLayout_Uniform;
        VkDescriptorSetLayout poDescriptorSetLayout_Uniform;
        VkPipelineLayout poPipelineLayout_Uniform;
        VkPipeline poPipeline_Uniform_Line;
        VkPipeline poPipeline_Uniform_Flat;
        VkPipeline poPipeline_Uniform_Flat_Wire;

        //PipelineGraphics-Storage
        String nameDescriptorSetLayout_Storage;
        StringVector aNamesDescriptorSetLayout_Storage;
        VkDescriptorSetLayout poDescriptorSetLayout_Storage;
        VkPipelineLayout poPipelineLayout_Storage;
        VkPipeline poPipeline_Storage_Line;
        VkPipeline poPipeline_Storage_Flat;
        VkPipeline poPipeline_Storage_Flat_Wire;
        
    public:
        virtual void Destroy();

        virtual void UpdateCBs();
        virtual void Draw(VkCommandBuffer& commandBuffer);

        virtual void CleanupSwapChain();
        virtual void RecreateSwapChain();

    public:
        bool IsBufferUniform() { return this->isBufferUniform; }
        virtual void ChangeBufferMode(bool isUniform);

    public:
        //Line 3D
        virtual PointerBuffer* AddLine3D_Line(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Line(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Triangle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Triangle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Quad(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Quad(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Grid(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Grid(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Quad_Convex(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Quad_Convex(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Quad_Concave(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Quad_Concave(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Circle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Circle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_AABB(const FMatrix4& vMatWorld, const FAABB& aabbObject, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_AABB(const FAABB& aabbWorld, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_AABB(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_AABB(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Sphere(const FMatrix4& vMatWorld, const FSphere& sphereObject, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Sphere(const FSphere& sphere, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Sphere(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Sphere(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Cylinder(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Cylinder(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Capsule(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Capsule(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Cone(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Cone(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Torus(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddLine3D_Torus(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);

        //Flat 3D
        virtual PointerBuffer* AddFlat3D_Triangle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Triangle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Quad(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Quad(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Quad_Convex(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Quad_Convex(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Quad_Concave(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Quad_Concave(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Circle(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Circle(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_AABB(const FMatrix4& vMatWorld, const FAABB& aabbObject, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_AABB(const FAABB& aabbWorld, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_AABB(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_AABB(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Sphere(const FMatrix4& vMatWorld, const FSphere& sphereObject, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Sphere(const FSphere& sphereWorld, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Sphere(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Sphere(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Cylinder(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Cylinder(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Capsule(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Capsule(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Cone(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Cone(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Torus(const FVector3& vPos, const FVector3& vRotAngle, const FVector3& vScale, const FColor& color, bool isUpdateBuffer = true);
        virtual PointerBuffer* AddFlat3D_Torus(const FMatrix4& vMat, const FColor& color, bool isUpdateBuffer = true);

        virtual void RemoveLineFlat3D(PointerBuffer* pPointer, bool isUpdateBuffer = true);
        virtual void UpdateBuffer(BufferStorageLineFlat3D* pBufferLineFlat3D);
        virtual void UpdateBuffer(const String& nameMesh);

    protected:
        virtual void initConfigs();
        virtual void initBufferUniforms();
            virtual BufferStorageLineFlat3D* initBuffer(const String& nameMesh, bool isBindDescriptor, bool bIsLineList);
        virtual void initDescriptorSetLayout();
        virtual void initPipelineLayout();
        virtual void initPipelineGraphics();
        virtual void destroyDescriptorSets_Graphics();
        virtual void createDescriptorSets_Graphics();
        virtual void updateDescriptorSets_Graphics();
            void updateDescriptorSets(const StringVector& aNamesDescriptorSetLayout, const VkDescriptorSetVector& vkDescriptorSets, BufferStorage* pBufferStorage);
        virtual void destroyMeshes();
        virtual void destroyBufferUniforms();
        virtual void destroyPipelineGraphics();
        virtual void destroyPipelineLayout();
        virtual void destroyDescriptorSetLayout();

    protected:
        Mesh* getMesh(const String& nameMesh);

        bool hasBufferLineFlat3D(const String& nameMesh);
        BufferStorageLineFlat3D* getBufferLineFlat3D(const String& nameMesh);
        BufferStorageLineFlat3D* getOrCreateBufferLineFlat3D(const String& nameMesh, bool bIsLineList);
        BufferStorageLineFlat3D* insertBufferLineFlat3D(const String& nameMesh, bool bIsLineList);
        void removeBufferLineFlat3D(const String& nameMesh);
        void removeBufferLineFlat3DAll();
    };

}; //LostPeterVulkan

#endif