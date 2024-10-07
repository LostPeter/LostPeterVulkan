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

    public:
    protected:
        class BufferUniformLineFlat3D
        {
        public:
            BufferUniformLineFlat3D(Mesh* p);
            ~BufferUniformLineFlat3D();

        public:
            Mesh* pMesh;
            BufferUniform* pBufferUniform;

            LineFlat3DObjectConstants* pLineFlat3DObject;
            int nObjectCount;

        public:
            void Destroy();
            void Init();

        public:
            void Clear();

            void AddLineFlat3DObject(const FMatrix4& mat, const FColor& color, bool isUpdateBuffer = true);
            void AddLineFlat3DObject(const LineFlat3DObjectConstants& object, bool isUpdateBuffer = true);
            void AddLineFlat3DObjects(const std::vector<LineFlat3DObjectConstants>& objects, bool isUpdateBuffer = true);
        };
        typedef std::vector<BufferUniformLineFlat3D*> BufferUniformLineFlat3DPtrVector;
        typedef std::map<Mesh*, BufferUniformLineFlat3D*> Mesh2BufferUniformLineFlat3DPtrMap;

        BufferUniformLineFlat3DPtrVector aBufferUniformLineFlat3D;
        Mesh2BufferUniformLineFlat3DPtrMap mapMesh2BufferUniformLineFlat3D;

        MeshPtrMap mapName2Mesh;
        
    public:
        virtual void Destroy();

        virtual void UpdateCBs();

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
            virtual void initMesh(const String& nameMesh);
        virtual void initShaders();
        virtual void initBufferUniforms();
            virtual void initBufferUniform(const String& nameMesh);
        virtual void initDescriptorSetLayout();
        virtual void initPipelineLayout();
        virtual void initPipelineGraphics();
        virtual void updateDescriptorSets_Graphics();

        virtual void destroyMeshes();
        virtual void destroyShaders();
        virtual void destroyBufferUniforms();
        virtual void destroyPipelineGraphics();
        virtual void destroyPipelineLayout();
        virtual void destroyDescriptorSetLayout();

    protected:
        Mesh* getMesh(const String& nameMesh);

        bool hasBufferUniformLineFlat3D(const String& nameMesh);
        BufferUniformLineFlat3D* getBufferUniformLineFlat3D(const String& nameMesh);
        BufferUniformLineFlat3D* insertBufferUniformLineFlat3D(const String& nameMesh);
        void removeBufferUniformLineFlat3D(const String& nameMesh);
        void removeBufferUniformLineFlat3DAll();
    };

}; //LostPeterVulkan

#endif