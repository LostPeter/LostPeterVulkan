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
        bool isBufferUniform;


    protected:
        ////////////////////// BufferBaseLineFlat3D /////////////////////////
        class BufferBaseLineFlat3D
        {
        public:
            BufferBaseLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh* p);
            virtual ~BufferBaseLineFlat3D();

        public:
            EditorLineFlat3DCollector* pLineFlat3DCollector;
            Mesh* pMesh;
            LineFlat3DObjectConstants* pLineFlat3DObject;
            int nObjectCount;

        public:
            virtual void Destroy() = 0;
            virtual void Init() = 0;

        public:
            virtual void Clear() = 0;

            virtual void AddLineFlat3DObject(const FMatrix4& mat, const FColor& color, bool isUpdateBuffer = true) = 0;
            virtual void AddLineFlat3DObject(const LineFlat3DObjectConstants& object, bool isUpdateBuffer = true) = 0;
            virtual void AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants>& objects, bool isUpdateBuffer = true) = 0;
        };


        ////////////////////// BufferStorageLineFlat3D //////////////////////
        class BufferStorageLineFlat3D : public BufferBaseLineFlat3D
        {
        public:
            BufferStorageLineFlat3D(EditorLineFlat3DCollector* pCollector, Mesh* p);
            virtual ~BufferStorageLineFlat3D();

        public:
            static const int s_nStepCount;

        public:
            BufferStorage* pBufferStorage;
            int nObjectCountMax;
        

        public:
            virtual void Destroy();
            virtual void Init();

        public:
            virtual void Clear();

            virtual void AddLineFlat3DObject(const FMatrix4& mat, const FColor& color, bool isUpdateBuffer = true);
            virtual void AddLineFlat3DObject(const LineFlat3DObjectConstants& object, bool isUpdateBuffer = true);
            virtual void AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants>& objects, bool isUpdateBuffer = true);

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
        StringVector* poDescriptorSetLayoutNames_Uniform;
        VkDescriptorSetLayout poDescriptorSetLayout_Uniform;
        VkPipelineLayout poPipelineLayout_Uniform;
        VkPipeline poPipeline_Uniform;
        VkDescriptorSet poDescriptorSet_Uniform;

        //PipelineGraphics-Storage
        String nameDescriptorSetLayout_Storage;
        StringVector* poDescriptorSetLayoutNames_Storage;
        VkDescriptorSetLayout poDescriptorSetLayout_Storage;
        VkPipelineLayout poPipelineLayout_Storage;
        VkPipeline poPipeline_Storage;
        VkDescriptorSet poDescriptorSet_Storage;

        
    public:
        virtual void Destroy();

        virtual void UpdateCBs();
        virtual void Draw(VkCommandBuffer& commandBuffer);

    public:
        //Line 3D
        virtual bool AddLine3D_Line();
        virtual bool AddLine3D_Triangle();
        virtual bool AddLine3D_Quad();
        virtual bool AddLine3D_Grid();
        virtual bool AddLine3D_Quad_Convex();
        virtual bool AddLine3D_Quad_Concave();
        virtual bool AddLine3D_Circle();
        virtual bool AddLine3D_AABB();
        virtual bool AddLine3D_Sphere();
        virtual bool AddLine3D_Cylinder();
        virtual bool AddLine3D_Capsule();
        virtual bool AddLine3D_Cone();
        virtual bool AddLine3D_Torus();

        //Flat 3D
        virtual bool AddFlat3D_Triangle();
        virtual bool AddFlat3D_Quad();
        virtual bool AddFlat3D_Quad_Convex();
        virtual bool AddFlat3D_Quad_Concave();
        virtual bool AddFlat3D_Circle();
        virtual bool AddFlat3D_AABB();
        virtual bool AddFlat3D_Sphere();
        virtual bool AddFlat3D_Cylinder();
        virtual bool AddFlat3D_Capsule();
        virtual bool AddFlat3D_Cone();
        virtual bool AddFlat3D_Torus();

    protected:
        virtual void initConfigs();
        virtual void initMeshes();
        virtual void initShaders();
        virtual void initBufferUniforms();
            virtual void initBuffer(const String& nameMesh);
        virtual void initDescriptorSetLayout();
        virtual void initPipelineLayout();
        virtual void initPipelineGraphics();
        virtual void updateDescriptorSets_Graphics();

        virtual void destroyMeshes();
        virtual void destroyBufferUniforms();
        virtual void destroyPipelineGraphics();
        virtual void destroyPipelineLayout();
        virtual void destroyDescriptorSetLayout();

    protected:
        Mesh* getMesh(const String& nameMesh);

        bool hasBufferLineFlat3D(const String& nameMesh);
        BufferStorageLineFlat3D* getBufferLineFlat3D(const String& nameMesh);
        BufferStorageLineFlat3D* insertBufferLineFlat3D(const String& nameMesh);
        void removeBufferLineFlat3D(const String& nameMesh);
        void removeBufferLineFlat3DAll();
    };

}; //LostPeterVulkan

#endif