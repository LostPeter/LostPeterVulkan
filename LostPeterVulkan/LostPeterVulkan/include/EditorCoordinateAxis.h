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

#ifndef _EDITOR_COORDINATE_AXIS_H_
#define _EDITOR_COORDINATE_AXIS_H_

#include "EditorBase.h"

namespace LostPeter
{
    class utilExport EditorCoordinateAxis : public EditorBase
    {
    public:
        EditorCoordinateAxis();
        virtual ~EditorCoordinateAxis();

    public:
        static size_t s_nMeshQuadIndex;
        static size_t s_nMeshQuadLineIndex;
        static size_t s_nMeshCylinderIndex;
        static size_t s_nMeshConeIndex;
        static size_t s_nMeshTorusIndex;
        static size_t s_nMeshAABBIndex;
        static size_t s_nMeshCoordinateAxisCount;

        static const String s_strNameShader_CoordinateAxis_Vert;
        static const String s_strNameShader_CoordinateAxis_Frag;
        static const String s_strNameShader_CoordinateAxisLine_Vert;
        static const String s_strNameShader_CoordinateAxisLine_Frag;
        
        static float s_fScale_Quad;
        static float s_fScale_Cylinder;
        static float s_fScale_Cone;
        static float s_fScale_Torus;
        static float s_fScale_AABB;
        static float s_fScale_AABBCenter;

        static FMatrix4 s_aMatrix4Transforms[19];
        static FColor s_aColors_Default[19];
        static FColor s_aColors_Hover[19];
        static FColor s_aColors_Select[19];
        static FColor s_aColors_UnSelect[19];

        static const float s_fScale_Distance;
        static const float s_fScale_WhenSelect_Axis;
        static const float s_fScale_WhenSelect_AxisCone;
        static const float s_fScale_WhenSelect_AxisTorus;
        static const float s_fScale_WhenSelect_AxisAABB;

    public:
        enum CoordinateStateType
        {
            CoordinateState_None = 0,
            CoordinateState_Select,
            CoordinateState_Move,
            CoordinateState_Rotate,
            CoordinateState_Scale,
        };
        enum CoordinateElementType
        {
            CoordinateElement_None = -1,

            CoordinateElement_Axis_X = 0,   //X Axis or X Scale
            CoordinateElement_Axis_Y,       //Y Axis or Y Scale
            CoordinateElement_Axis_Z,       //Y Axis or Z Scale
            CoordinateElement_Axis_XYZ,     //XYZ Scale

            CoordinateElement_Quad_XY,
            CoordinateElement_Quad_YZ,
            CoordinateElement_Quad_ZX,
        };

    public:
        FCamera* pCamera;
        FVector4 vViewport;

    public:
        std::vector<CoordinateAxisObjectConstants> coordinateAxisObjectCBs;
        VkBuffer poBuffers_ObjectCB;
        VkDeviceMemory poBuffersMemory_ObjectCB;

    protected:
        float scaleCoordinate;
        FVector3 vPos;
        FMatrix4 mat4Trans;

        CoordinateStateType typeState; 
        CoordinateElementType typeElementSelect;
        bool isButtonLeftDown;
        double ptLastX;
        double ptLastY;
        
        FSegment aAxis[3];
        FCone aCone[3];
        FAABB aQuadAABB[3];
        FVector3 aTorusX[30];
        FVector3 aTorusY[30];
        FVector3 aTorusZ[30];
        FAABB aScaleAABB[4];

    private:
        FMeshCreateParam_EntityCone* pEntityCone;
        
    public:
        LP_FORCEINLINE float GetScaleCoordinate() const { return this->scaleCoordinate; }
        LP_FORCEINLINE void SetScaleCoordinate(float f) { this->scaleCoordinate = f; }
        LP_FORCEINLINE const FVector3& GetPos() const { return this->vPos; }
        void SetPos(const FVector3& vP);

        LP_FORCEINLINE bool IsButtonLeftDown() const { return this->isButtonLeftDown; }

        LP_FORCEINLINE bool IsStateNone() const { return this->typeState == CoordinateState_None; }
        LP_FORCEINLINE bool IsStateSelect() const  { return this->typeState == CoordinateState_Select; }
        LP_FORCEINLINE bool IsStateMove() const { return this->typeState == CoordinateState_Move; }
        LP_FORCEINLINE bool IsStateRotate() const { return this->typeState == CoordinateState_Rotate; }
        LP_FORCEINLINE bool IsStateScale() const { return this->typeState == CoordinateState_Scale; }
        LP_FORCEINLINE void SetStateType(CoordinateStateType type) { this->typeState = type; }

        bool IsAxisSelected() const;
        bool IsAxisSelectedByIndex(int index) const; //0:X; 1:Y; 2:Z
        bool IsAxisSelectedByType(CoordinateElementType type) const;
        bool IsAxisXSelected() const;
        bool IsAxisYSelected() const;
        bool IsAxisZSelected() const;
        CoordinateElementType GetAxisSelected()  const;
        void SetAxisSelected(CoordinateElementType type);

        bool IsQuadSelected() const;
        bool IsQuadSelectedByIndex(int index) const; //0:XY; 1:YZ; 2:ZX
        bool IsQuadSelectedByType(CoordinateElementType type) const;
        bool IsQuadXYSelected() const;
        bool IsQuadYZSelected() const;
        bool IsQuadZXSelected() const;
        CoordinateElementType GetQuadSelected() const;
        void SetQuadSelected(CoordinateElementType type);
        void ClearSelectState();

        bool IsScaleAABBSelected() const;
        bool IsScaleAABBSelectedByIndex(int index) const; //0:X; 1:Y; 2: Z; 3:XYZ
        bool IsScaleAABBSelectedByType(CoordinateElementType type) const;
        bool IsScaleAABBXSelected() const;
        bool IsScaleAABBYSelected() const;
        bool IsScaleAABBZSelected() const;
        bool IsScaleAABBXYZSelected() const;
        CoordinateElementType GetScaleAABBSelected() const;
        void SetScaleAABBSelected(CoordinateElementType type);

    public:
        virtual void Destroy();

        virtual void Init();

        virtual void UpdateCBs();
            virtual void UpdateCBs_Move();
            virtual void UpdateCBs_Rotate();
            virtual void UpdateCBs_Scale();

        virtual void Draw(VkCommandBuffer& commandBuffer);
            virtual void Draw_Move(VkCommandBuffer& commandBuffer);
            virtual void Draw_Rotate(VkCommandBuffer& commandBuffer);
            virtual void Draw_Scale(VkCommandBuffer& commandBuffer);

            virtual void DrawQuad(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart);
            virtual void DrawQuadLine(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart);
            virtual void DrawShape(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart);


        virtual void CheckStateMove(double x, double y);
        virtual void CheckStateRotate(double x, double y);
        virtual void CheckStateScale(double x, double y);

        virtual void Move(double deltaX, double deltaY);
        virtual void Rotate(double deltaX, double deltaY);
        virtual void Scale(double deltaX, double deltaY);

        virtual void MouseLeftDown(double x, double y);
        virtual void MouseMove(double x, double y);
        virtual void MouseLeftUp(double x, double y);
        virtual void MouseHover(double x, double y);

    public:
        virtual void CleanupSwapChain();
        virtual void RecreateSwapChain();

    protected:
        virtual void initConfigs();
        virtual void initBufferUniforms();
        virtual void initPipelineGraphics();
        virtual void updateDescriptorSets_Graphics();

        virtual void destroyBufferUniforms();
        virtual void destroyPipelineGraphics();
    };

}; //LostPeter

#endif