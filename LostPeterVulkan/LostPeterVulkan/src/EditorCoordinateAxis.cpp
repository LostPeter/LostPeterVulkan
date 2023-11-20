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

#include "../include/EditorCoordinateAxis.h"
#include "../include/VulkanWindow.h"
#include "../include/Mesh.h"
#include "../include/MeshSub.h"
#include "../include/VKPipelineGraphics.h"

namespace LostPeterVulkan
{
    size_t EditorCoordinateAxis::s_nMeshQuadIndex = 0;
    size_t EditorCoordinateAxis::s_nMeshQuadLineIndex = 1;
    size_t EditorCoordinateAxis::s_nMeshCylinderIndex = 2;
    size_t EditorCoordinateAxis::s_nMeshConeIndex = 3;
    size_t EditorCoordinateAxis::s_nMeshTorusIndex = 4;
    size_t EditorCoordinateAxis::s_nMeshAABBIndex = 5;
    size_t EditorCoordinateAxis::s_nMeshCoordinateAxisCount = 6;
    const String EditorCoordinateAxis::s_strNameShader_CoordinateAxis_Vert = "vert_editor_coordinate_axis";
    const String EditorCoordinateAxis::s_strNameShader_CoordinateAxis_Frag = "frag_editor_coordinate_axis";
    const String EditorCoordinateAxis::s_strNameShader_CoordinateAxisLine_Vert = "vert_editor_coordinate_axis_line";
    const String EditorCoordinateAxis::s_strNameShader_CoordinateAxisLine_Frag = "frag_editor_coordinate_axis_line";
    float EditorCoordinateAxis::s_fScale_Quad = 0.3f;
    float EditorCoordinateAxis::s_fScale_Cylinder = 0.01f;
    float EditorCoordinateAxis::s_fScale_Cone = 0.1f;
    float EditorCoordinateAxis::s_fScale_Torus = 1.0f;
    float EditorCoordinateAxis::s_fScale_AABB = 0.1f;
    float EditorCoordinateAxis::s_fScale_AABBCenter = 0.1f * 1.5f;
    FMatrix4 EditorCoordinateAxis::s_aMatrix4Transforms[19] = 
    {
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(s_fScale_Quad, s_fScale_Quad, 1.0f)), //Quad XY+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f, -90.0f,  0.0f), FVector3(s_fScale_Quad, s_fScale_Quad, 1.0f)), //Quad YZ+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3( 90.0f,   0.0f,  0.0f), FVector3(s_fScale_Quad, s_fScale_Quad, 1.0f)), //Quad ZX+

        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(s_fScale_Quad, s_fScale_Quad, 1.0f)), //Quad Line XY+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f, -90.0f,  0.0f), FVector3(s_fScale_Quad, s_fScale_Quad, 1.0f)), //Quad Line YZ+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3( 90.0f,   0.0f,  0.0f), FVector3(s_fScale_Quad, s_fScale_Quad, 1.0f)), //Quad Line ZX+

        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f, -90.0f), FVector3(s_fScale_Cylinder, 1.0f, s_fScale_Cylinder)), //Cylinder X+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_Cylinder, 1.0f, s_fScale_Cylinder)), //Cylinder Y+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3( 90.0f,  0.0f,   0.0f), FVector3(s_fScale_Cylinder, 1.0f, s_fScale_Cylinder)), //Cylinder Z+

        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f, -90.0f), FVector3(s_fScale_Cone, s_fScale_Cone, s_fScale_Cone)), //Cone X+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_Cone, s_fScale_Cone, s_fScale_Cone)), //Cone Y+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3( 90.0f,  0.0f,   0.0f), FVector3(s_fScale_Cone, s_fScale_Cone, s_fScale_Cone)), //Cone Z+

        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f, -90.0f), FVector3(s_fScale_Torus, 1.0f, s_fScale_Torus)), //Torus X+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_Torus, 1.0f, s_fScale_Torus)), //Torus Y+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3( 90.0f,  0.0f,   0.0f), FVector3(s_fScale_Torus, 1.0f, s_fScale_Torus)), //Torus Z+

        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_AABB, s_fScale_AABB, s_fScale_AABB)), //AABB X+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_AABB, s_fScale_AABB, s_fScale_AABB)), //AABB Y+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_AABB, s_fScale_AABB, s_fScale_AABB)), //AABB Z+
        FMath::FromTRS(FVector3( 0.0f,  0.0f,  0.0f), FVector3(  0.0f,  0.0f,   0.0f), FVector3(s_fScale_AABBCenter, s_fScale_AABBCenter, s_fScale_AABBCenter)), //AABB XYZ+
    };
    FColor EditorCoordinateAxis::s_aColors_Default[19] = 
    {
        FColor(0.0f, 0.0f, 0.3f, 0.5f), //Quad XY+
        FColor(0.3f, 0.0f, 0.0f, 0.5f), //Quad YZ+
        FColor(0.0f, 0.3f, 0.0f, 0.5f), //Quad ZX+

        FColor(0.0f, 0.0f, 1.0f, 1.0f), //Quad Line XY+
        FColor(1.0f, 0.0f, 0.0f, 1.0f), //Quad Line YZ+
        FColor(0.0f, 1.0f, 0.0f, 1.0f), //Quad Line ZX+

        FColor(1.0f, 0.0f, 0.0f, 1.0f), //Cylinder X+
        FColor(0.0f, 1.0f, 0.0f, 1.0f), //Cylinder Y+
        FColor(0.0f, 0.0f, 1.0f, 1.0f), //Cylinder Z+

        FColor(1.0f, 0.0f, 0.0f, 1.0f), //Cone X+
        FColor(0.0f, 1.0f, 0.0f, 1.0f), //Cone Y+
        FColor(0.0f, 0.0f, 1.0f, 1.0f), //Cone Z+

        FColor(1.0f, 0.0f, 0.0f, 1.0f), //Torus X+
        FColor(0.0f, 1.0f, 0.0f, 1.0f), //Torus Y+
        FColor(0.0f, 0.0f, 1.0f, 1.0f), //Torus Z+

        FColor(1.0f, 0.0f, 0.0f, 1.0f), //AABB X+
        FColor(0.0f, 1.0f, 0.0f, 1.0f), //AABB Y+
        FColor(0.0f, 0.0f, 1.0f, 1.0f), //AABB Z+
        FColor(0.5f, 0.5f, 0.5f, 1.0f), //AABB XYZ+
    };
    FColor EditorCoordinateAxis::s_aColors_Hover[19] = 
    {
        FColor(0.0f, 0.0f, 1.0f, 0.5f), //Quad XY+
        FColor(1.0f, 0.0f, 0.0f, 0.5f), //Quad YZ+
        FColor(0.0f, 1.0f, 0.0f, 0.5f), //Quad ZX+

        FColor(0.0f, 0.0f, 1.0f, 1.0f), //Quad Line XY+
        FColor(1.0f, 0.0f, 0.0f, 1.0f), //Quad Line YZ+
        FColor(0.0f, 1.0f, 0.0f, 1.0f), //Quad Line ZX+

        FColor(1.0f, 0.0f, 0.0f, 0.8f), //Cylinder X+
        FColor(0.0f, 1.0f, 0.0f, 0.8f), //Cylinder Y+
        FColor(0.0f, 0.0f, 1.0f, 0.8f), //Cylinder Z+

        FColor(1.0f, 0.0f, 0.0f, 0.8f), //Cone X+
        FColor(0.0f, 1.0f, 0.0f, 0.8f), //Cone Y+
        FColor(0.0f, 0.0f, 1.0f, 0.8f), //Cone Z+

        FColor(1.0f, 0.0f, 0.0f, 0.8f), //Torus X+
        FColor(0.0f, 1.0f, 0.0f, 0.8f), //Torus Y+
        FColor(0.0f, 0.0f, 1.0f, 0.8f), //Torus Z+

        FColor(1.0f, 0.0f, 0.0f, 0.8f), //AABB X+
        FColor(0.0f, 1.0f, 0.0f, 0.8f), //AABB Y+
        FColor(0.0f, 0.0f, 1.0f, 0.8f), //AABB Z+
        FColor(0.5f, 0.5f, 0.5f, 0.8f), //AABB XYZ+
    };
    FColor EditorCoordinateAxis::s_aColors_Select[19] = 
    {
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Quad XY+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Quad YZ+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Quad ZX+

        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Quad Line XY+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Quad Line YZ+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Quad Line ZX+

        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Cylinder X+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Cylinder Y+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Cylinder Z+

        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Cone X+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Cone Y+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Cone Z+

        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Torus X+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Torus Y+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //Torus Z+

        FColor(1.0f, 1.0f, 0.0f, 1.0f), //AABB X+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //AABB Y+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //AABB Z+
        FColor(1.0f, 1.0f, 0.0f, 1.0f), //AABB XYZ+
    };
    FColor EditorCoordinateAxis::s_aColors_UnSelect[19] = 
    {
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Quad XY+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Quad YZ+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Quad ZX+

        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Quad Line XY+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Quad Line YZ+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Quad Line ZX+

        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Cylinder X+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Cylinder Y+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Cylinder Z+

        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Cone X+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Cone Y+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Cone Z+

        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Torus X+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Torus Y+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //Torus Z+

        FColor(0.5f, 0.5f, 0.5f, 0.3f), //AABB X+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //AABB Y+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //AABB Z+
        FColor(0.5f, 0.5f, 0.5f, 0.3f), //AABB XYZ+
    };
    const float EditorCoordinateAxis::s_fScale_Distance = 8.0f;
    const float EditorCoordinateAxis::s_fScale_WhenSelect_Axis = 2.0f;
    const float EditorCoordinateAxis::s_fScale_WhenSelect_AxisCone = 1.2f;
    const float EditorCoordinateAxis::s_fScale_WhenSelect_AxisTorus = 1.05f;
    const float EditorCoordinateAxis::s_fScale_WhenSelect_AxisAABB = 1.2f;
    
    EditorCoordinateAxis::EditorCoordinateAxis()
        : EditorBase("EditorCoordinateAxis")

        , poBuffers_ObjectCB(VK_NULL_HANDLE)
        , poBuffersMemory_ObjectCB(VK_NULL_HANDLE)

        , scaleCoordinate(1.0f)
        , vPos(FMath::ms_v3Zero)
        , mat4Trans(FMath::ms_mat4Unit)

        , typeState(CoordinateState_Select)
        , typeElementSelect(CoordinateElement_None)

        , isButtonLeftDown(false)
        , ptLastX(0)
        , ptLastY(0)

        , pEntityCone(nullptr)
    {
        this->pCamera = Base::GetWindowPtr()->GetCamera();
        this->vViewport = Base::GetWindowPtr()->GetViewportVector4();
        ClearSelectState();
    }
    EditorCoordinateAxis::~EditorCoordinateAxis()
    {
        Destroy();
    }
    void EditorCoordinateAxis::Destroy()
    {
        destroyBufferUniforms();
        destroyShaders();
        destroyMeshes();
    }
    void EditorCoordinateAxis::Init()
    {
        EditorBase::Init();
    }
    void EditorCoordinateAxis::SetPos(const FVector3& vP) 
    {
        this->vPos = vP; 
        this->mat4Trans = FMath::Translate(this->vPos);
    }
    bool EditorCoordinateAxis::IsAxisSelected() const 
    {
        if (this->typeElementSelect == CoordinateElement_Axis_X ||
            this->typeElementSelect == CoordinateElement_Axis_Y ||
            this->typeElementSelect == CoordinateElement_Axis_Z)
        {
            return true;
        }
        return false;
    }
    bool EditorCoordinateAxis::IsAxisSelectedByIndex(int index)  const
    {
        if (index == 0)
            return IsAxisSelectedByType(CoordinateElement_Axis_X);
        else if (index == 1)
            return IsAxisSelectedByType(CoordinateElement_Axis_Y);
        else if (index == 2)
            return IsAxisSelectedByType(CoordinateElement_Axis_Z);
        return false;
    }
    bool EditorCoordinateAxis::IsAxisSelectedByType(CoordinateElementType type)  const
    {
        return this->typeElementSelect == type;
    }
    bool EditorCoordinateAxis::IsAxisXSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_X;
    }
    bool EditorCoordinateAxis::IsAxisYSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_Y;
    }
    bool EditorCoordinateAxis::IsAxisZSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_Z;
    }   
    EditorCoordinateAxis::CoordinateElementType EditorCoordinateAxis::GetAxisSelected() const
    {
        if (IsAxisSelected())
            return this->typeElementSelect;
        return CoordinateElement_None;
    }
    void EditorCoordinateAxis::SetAxisSelected(CoordinateElementType type)
    {
        F_Assert((type == CoordinateElement_Axis_X || type == CoordinateElement_Axis_Y || type == CoordinateElement_Axis_Z) && "EditorCoordinateAxis::SetAxisSelected")
        this->typeElementSelect = type;
    }
    bool EditorCoordinateAxis::IsQuadSelected() const
    {
        if (this->typeElementSelect == CoordinateElement_Quad_XY ||
            this->typeElementSelect == CoordinateElement_Quad_YZ ||
            this->typeElementSelect == CoordinateElement_Quad_ZX)
        {
            return true;
        }
        return false;
    }
    bool EditorCoordinateAxis::IsQuadSelectedByIndex(int index) const
    {
        if (index == 0)
            return IsQuadSelectedByType(CoordinateElement_Quad_XY);
        else if (index == 1)
            return IsQuadSelectedByType(CoordinateElement_Quad_YZ);
        else if (index == 2)
            return IsQuadSelectedByType(CoordinateElement_Quad_ZX);
        return false;
    }
    bool EditorCoordinateAxis::IsQuadSelectedByType(CoordinateElementType type) const
    {
        return this->typeElementSelect == type;
    }
    bool EditorCoordinateAxis::IsQuadXYSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Quad_XY;
    }
    bool EditorCoordinateAxis::IsQuadYZSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Quad_YZ;
    }
    bool EditorCoordinateAxis::IsQuadZXSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Quad_ZX;
    }
    EditorCoordinateAxis::CoordinateElementType EditorCoordinateAxis::GetQuadSelected() const
    {
        if (IsQuadSelected())
            return this->typeElementSelect;
        return CoordinateElement_None;
    }   
    void EditorCoordinateAxis::SetQuadSelected(CoordinateElementType type)
    {
        F_Assert((type == CoordinateElement_Quad_XY || type == CoordinateElement_Quad_YZ || type == CoordinateElement_Quad_ZX) && "EditorCoordinateAxis::SetQuadSelected")
        this->typeElementSelect = type;
    }
    void EditorCoordinateAxis::ClearSelectState()
    {
        this->typeElementSelect = CoordinateElement_None;
    }
    bool EditorCoordinateAxis::IsScaleAABBSelected() const
    {
        if (this->typeElementSelect == CoordinateElement_Axis_X ||
            this->typeElementSelect == CoordinateElement_Axis_Y ||
            this->typeElementSelect == CoordinateElement_Axis_Z ||
            this->typeElementSelect == CoordinateElement_Axis_XYZ)
        {
            return true;
        }
        return false;
    }
    bool EditorCoordinateAxis::IsScaleAABBSelectedByIndex(int index) const
    {
        if (index == 0)
            return IsScaleAABBSelectedByType(CoordinateElement_Axis_X);
        else if (index == 1)
            return IsScaleAABBSelectedByType(CoordinateElement_Axis_Y);
        else if (index == 2)
            return IsScaleAABBSelectedByType(CoordinateElement_Axis_Z);
        else if (index == 3)
            return IsScaleAABBSelectedByType(CoordinateElement_Axis_XYZ);
        return false;
    }
    bool EditorCoordinateAxis::IsScaleAABBSelectedByType(CoordinateElementType type) const
    {
        return this->typeElementSelect == type;
    }
    bool EditorCoordinateAxis::IsScaleAABBXSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_X;
    }
    bool EditorCoordinateAxis::IsScaleAABBYSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_Y;
    }
    bool EditorCoordinateAxis::IsScaleAABBZSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_Z;
    }
    bool EditorCoordinateAxis::IsScaleAABBXYZSelected() const
    {
        return this->typeElementSelect == CoordinateElement_Axis_XYZ;
    }
    EditorCoordinateAxis::CoordinateElementType EditorCoordinateAxis::GetScaleAABBSelected() const
    {
        if (IsScaleAABBSelected())
            return this->typeElementSelect;
        return CoordinateElement_None;
    }
    void EditorCoordinateAxis::SetScaleAABBSelected(CoordinateElementType type)
    {
        F_Assert((type == CoordinateElement_Axis_X || type == CoordinateElement_Axis_Y || type == CoordinateElement_Axis_Z || type == CoordinateElement_Axis_XYZ) && "EditorCoordinateAxis::SetScaleAABBSelected")
        this->typeElementSelect = type;
    }
    void EditorCoordinateAxis::UpdateCBs()
    {
        float fDis = FMath::Length(this->pCamera->GetPos() - this->vPos);
        this->scaleCoordinate = FMath::Max(1.0f, fDis / s_fScale_Distance);
        
        switch ((int)this->typeState)
        {
        case CoordinateState_Select:
        case CoordinateState_Move:
            {
                UpdateCBs_Move();
                break;
            }
        case CoordinateState_Rotate:
            {
                UpdateCBs_Rotate();
                break;
            }
        case CoordinateState_Scale:
            {
                UpdateCBs_Scale();
                break;
            }
        }
        
        size_t bufSize = sizeof(CoordinateAxisObjectConstants) * this->coordinateAxisObjectCBs.size();
        void* pBuf = &this->coordinateAxisObjectCBs[0];
        Base::GetWindowPtr()->updateVKBuffer(0, bufSize, pBuf, this->poBuffersMemory_ObjectCB);
    }
        void EditorCoordinateAxis::UpdateCBs_Move()
        {
            int countStart = 0;
            int countNumber = 3;

            //Sequence
            const FVector3& vCameraPos = this->pCamera->GetPos();
            FVector3 vCenter = FMath::Transform(this->mat4Trans, FMath::ms_v3Zero);
            float fLength = this->scaleCoordinate * s_fScale_Quad;
            //X
            FVector3 vAxisX = FMath::Transform(this->mat4Trans, FVector3(this->scaleCoordinate, 0.0f, 0.0f));
            this->aAxis[0].m_pt0 = vCenter;
            this->aAxis[0].m_pt1 = vAxisX;
            //Y
            FVector3 vAxisY = FMath::Transform(this->mat4Trans, FVector3(0.0f, this->scaleCoordinate, 0.0f)); 
            this->aAxis[1].m_pt0 = vCenter; 
            this->aAxis[1].m_pt1 = vAxisY;
            //Z
            FVector3 vAxisZ = FMath::Transform(this->mat4Trans, FVector3(0.0f, 0.0f, this->scaleCoordinate));
            this->aAxis[2].m_pt0 = vCenter; 
            this->aAxis[2].m_pt1 = vAxisZ;
            
            //Distance 
            float aDistances[3] = 
            {
                FMath::Length2(vCameraPos - vAxisX),
                FMath::Length2(vCameraPos - vAxisY),
                FMath::Length2(vCameraPos - vAxisZ),
            };
            int aSequences[3] = { 0, 1, 2 };
            FUtil::SortBubble(3, aDistances, aSequences);

            //State
            bool bHasAxisSelected = false;
            bool aAxisIsSelected[3] = {false, false, false};
            bool bHasQuadSelected = false;
            bool aQuadIsSelected[3] =  {false, false, false};
            for (int i = 0; i < countNumber; i++)
            {
                if (IsAxisSelectedByIndex(i))
                {
                    bHasAxisSelected = true;
                    aAxisIsSelected[i] = true;

                    bHasQuadSelected = false;
                    break;
                }
                else if (IsQuadSelectedByIndex(i))
                {
                    bHasQuadSelected = true;
                    aQuadIsSelected[i] = true;

                    bHasAxisSelected = false;
                    break;
                }
            }
            
            //Quad (0-2)
            {
                FMatrix4 aWorldQuads[3] = 
                {   
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, this->scaleCoordinate, this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 0], //XY+
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, this->scaleCoordinate, this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 1], //YZ+
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, this->scaleCoordinate, this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 2], //ZX+
                };
                float fQuadLenHalf = 0.5f * s_fScale_Quad * this->scaleCoordinate;
                //Quad AABB
                this->aQuadAABB[0].SetCenterExtents(vCenter + FVector3(fQuadLenHalf, fQuadLenHalf, 0.0f), FVector3(fQuadLenHalf, fQuadLenHalf, 0.0001f)); //XY+
                this->aQuadAABB[1].SetCenterExtents(vCenter + FVector3(0.0f, fQuadLenHalf, fQuadLenHalf), FVector3(0.0001f, fQuadLenHalf, fQuadLenHalf)); //YZ+
                this->aQuadAABB[2].SetCenterExtents(vCenter + FVector3(fQuadLenHalf, 0.0f, fQuadLenHalf), FVector3(fQuadLenHalf, 0.0001f, fQuadLenHalf)); //ZX+
                
                for (int i = 0; i < countNumber; i++)
                {
                    CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                    int index = aSequences[2 - i];
                    objConsts.g_MatWorld = aWorldQuads[index];

                    if (bHasQuadSelected)
                    {
                        if (aQuadIsSelected[index])
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_Select[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Hover[countStart + index];
                            }
                        }
                        else
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_UnSelect[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Default[countStart + index];
                            }
                        }
                    }
                    else
                    {
                        if (bHasAxisSelected && IsButtonLeftDown())
                        {
                            objConsts.color = s_aColors_UnSelect[countStart + index];
                        }
                        else
                        {
                            objConsts.color = s_aColors_Default[countStart + index];
                        }   
                    }
                }
                countStart += countNumber; //3
            }
            //Quad Line (3-5)
            {
                FMatrix4 aWorldQuadLines[3] = 
                {   
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, this->scaleCoordinate, this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 0], //XY+
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, this->scaleCoordinate, this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 1], //YZ+
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, this->scaleCoordinate, this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 2], //ZX+
                };
                for (int i = 0; i < countNumber; i++)
                {
                    CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                    int index = aSequences[2 - i];
                    objConsts.g_MatWorld = aWorldQuadLines[index];

                    if (bHasQuadSelected)
                    {
                        if (aQuadIsSelected[index])
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_Select[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Hover[countStart + index];
                            }
                        }
                        else
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_UnSelect[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Default[countStart + index];
                            }
                        }
                    }
                    else
                    {
                        if (bHasAxisSelected && IsButtonLeftDown())
                        {
                            objConsts.color = s_aColors_UnSelect[countStart + index];
                        }
                        else
                        {
                            objConsts.color = s_aColors_Default[countStart + index];
                        }
                    }
                }
                countStart += countNumber; //6
            }
            //Cylinder (6-8) - Cone (9-11)
            {
                float scaleAxis[3] = 
                {
                    this->scaleCoordinate,
                    this->scaleCoordinate,
                    this->scaleCoordinate,
                };
                float scaleCones[3] = 
                {
                    this->scaleCoordinate,
                    this->scaleCoordinate,
                    this->scaleCoordinate,
                };
                for (int i = 0; i < 3; i++)
                {
                    if (IsAxisSelectedByIndex(i))
                    {
                        scaleAxis[i] *= s_fScale_WhenSelect_Axis;
                        scaleCones[i] *= s_fScale_WhenSelect_AxisCone;
                        break;
                    }
                }

                //Cylinder (6-8)
                FMatrix4 aWorldCylinders[3] = 
                {
                    this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, scaleAxis[0], scaleAxis[0])) * s_aMatrix4Transforms[countStart + 0],
                    this->mat4Trans * FMath::Scale(FVector3(scaleAxis[1], this->scaleCoordinate, scaleAxis[1])) * s_aMatrix4Transforms[countStart + 1],
                    this->mat4Trans * FMath::Scale(FVector3(scaleAxis[2], scaleAxis[2], this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 2],
                };
                for (int i = 0; i < countNumber; i++)
                {
                    CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                    int index = aSequences[2 - i];
                    objConsts.g_MatWorld = aWorldCylinders[index];

                    if (bHasAxisSelected)
                    {
                        if (aAxisIsSelected[index])
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_Select[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Hover[countStart + index];
                            }
                        }
                        else
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_UnSelect[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Default[countStart + index];
                            }
                        }
                    }
                    else
                    {
                        if (bHasQuadSelected && IsButtonLeftDown())
                        {
                            objConsts.color = s_aColors_UnSelect[countStart + index];
                        }
                        else
                        {
                            objConsts.color = s_aColors_Default[countStart + index];
                        }
                    }
                }
                countStart += countNumber; //9

                //Cone (9-11)
                FMatrix4 aWorldCones[3] = 
                {
                    this->mat4Trans * FMath::FromTRS(FVector3(this->scaleCoordinate, 0.0f, 0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleCones[0], scaleCones[0], scaleCones[0])) * s_aMatrix4Transforms[countStart + 0], //X+
                    this->mat4Trans * FMath::FromTRS(FVector3(0.0f, this->scaleCoordinate, 0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleCones[1], scaleCones[1], scaleCones[1])) * s_aMatrix4Transforms[countStart + 1], //Y+
                    this->mat4Trans * FMath::FromTRS(FVector3(0.0f, 0.0f, this->scaleCoordinate), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleCones[2], scaleCones[2], scaleCones[2])) * s_aMatrix4Transforms[countStart + 2], //Z+
                };
                const FVector3& vCenter = this->pEntityCone->GetCenter();
                FVector3 vTop = this->pEntityCone->GetTop();
                float fRadius = this->pEntityCone->GetRadius();
                float fHeight = this->pEntityCone->GetHeight();
                //X
                this->aCone[0].SetConeParam(FMath::Transform(aWorldCones[0], vCenter), FMath::Transform(aWorldCones[0], vTop), fRadius * scaleCones[0] * s_fScale_Cone, fHeight * scaleCones[0] * s_fScale_Cone);
                //Y
                this->aCone[1].SetConeParam(FMath::Transform(aWorldCones[1], vCenter), FMath::Transform(aWorldCones[1], vTop), fRadius * scaleCones[1] * s_fScale_Cone, fHeight * scaleCones[1] * s_fScale_Cone);
                //Z
                this->aCone[2].SetConeParam(FMath::Transform(aWorldCones[2], vCenter), FMath::Transform(aWorldCones[2], vTop), fRadius * scaleCones[0] * s_fScale_Cone, fHeight * scaleCones[2] * s_fScale_Cone);
                
                for (int i = 0; i < countNumber; i++)
                {
                    CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                    int index = aSequences[2 - i];
                    objConsts.g_MatWorld = aWorldCones[index];

                    if (bHasAxisSelected)
                    {
                        if (aAxisIsSelected[index])
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_Select[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Hover[countStart + index];
                            }
                        }
                        else
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_UnSelect[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Default[countStart + index];
                            }
                        }
                    }
                    else
                    {
                        if (bHasQuadSelected && IsButtonLeftDown())
                        {
                            objConsts.color = s_aColors_UnSelect[countStart + index];
                        }
                        else
                        {
                            objConsts.color = s_aColors_Default[countStart + index];
                        }
                    }
                }
                countStart += countNumber; //12
            }
        }
        void EditorCoordinateAxis::UpdateCBs_Rotate()
        {
            FVector3 vCenter = FMath::Transform(this->mat4Trans, FMath::ms_v3Zero);

            //State
            bool bHasAxisSelected = false;
            bool aAxisIsSelected[3] =  {false, false, false};

            //Torus (12-14)
            float scaleToruses[3] = 
            {
                this->scaleCoordinate,
                this->scaleCoordinate,
                this->scaleCoordinate,
            };
            for (int i = 0; i < 3; i++)
            {
                if (IsAxisSelectedByIndex(i))
                {
                    scaleToruses[i] *= s_fScale_WhenSelect_AxisTorus;

                    bHasAxisSelected = true;
                    aAxisIsSelected[i] = true;
                    break;
                }
            }

            int countStart = 12;
            int countNumber = 3;
            FMatrix4 aWorldToruses[3] = 
            {
                this->mat4Trans * FMath::Scale(FVector3(scaleToruses[0], scaleToruses[0], scaleToruses[0])) * s_aMatrix4Transforms[countStart + 0],
                this->mat4Trans * FMath::Scale(FVector3(scaleToruses[1], scaleToruses[1], scaleToruses[1])) * s_aMatrix4Transforms[countStart + 1],
                this->mat4Trans * FMath::Scale(FVector3(scaleToruses[2], scaleToruses[2], scaleToruses[2])) * s_aMatrix4Transforms[countStart + 2],
            };
            float fLengthX = 0.5f * s_fScale_Torus * scaleToruses[0];
            float fLengthY = 0.5f * s_fScale_Torus * scaleToruses[1];
            float fLengthZ = 0.5f * s_fScale_Torus * scaleToruses[2];
            int nIndexCnt = 60;
	        int nStartIndex = 0;
            float fPerAngle = 360.0f / 30;
            for (int i = 0; i < 30; ++i)
            {
                float fRad = i * fPerAngle * FMath::ms_fDeg2Rad;
                float fCos = cos(fRad);
                float fSin = sin(fRad);
                this->aTorusX[i] = vCenter + FVector3(0.0f, fCos,  fSin) * fLengthY;
                this->aTorusY[i] = vCenter + FVector3(fCos, 0.0f, -fSin) * fLengthZ;
                this->aTorusZ[i] = vCenter + FVector3(fCos, fSin,  0.0f) * fLengthX;
            }
            for (int i = 0; i < countNumber; i++)
            {
                CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                objConsts.g_MatWorld = aWorldToruses[i];

                if (bHasAxisSelected)
                {
                    if (aAxisIsSelected[i])
                    {
                        if (IsButtonLeftDown())
                        {
                            objConsts.color = s_aColors_Select[countStart + i];
                        }
                        else
                        {
                            objConsts.color = s_aColors_Hover[countStart + i];
                        }
                    }
                    else
                    {
                        if (IsButtonLeftDown())
                        {
                            objConsts.color = s_aColors_UnSelect[countStart + i];
                        }
                        else
                        {
                            objConsts.color = s_aColors_Default[countStart + i];
                        }
                    }
                }
                else
                {
                    objConsts.color = s_aColors_Default[countStart + i];
                }
            }
        }
        void EditorCoordinateAxis::UpdateCBs_Scale()
        {
            //Sequence
            const FVector3& vCameraPos = this->pCamera->GetPos();
            FVector3 vCenter = FMath::Transform(this->mat4Trans, FMath::ms_v3Zero);
            float fLength = this->scaleCoordinate * s_fScale_Quad;
            //X
            FVector3 vAxisX = FMath::Transform(this->mat4Trans, FVector3(this->scaleCoordinate, 0.0f, 0.0f));
            this->aAxis[0].m_pt0 = vCenter;
            this->aAxis[0].m_pt1 = vAxisX;
            //Y
            FVector3 vAxisY = FMath::Transform(this->mat4Trans, FVector3(0.0f, this->scaleCoordinate, 0.0f)); 
            this->aAxis[1].m_pt0 = vCenter;
            this->aAxis[1].m_pt1 = vAxisY;
            //Z
            FVector3 vAxisZ = FMath::Transform(this->mat4Trans, FVector3(0.0f, 0.0f, this->scaleCoordinate));
            this->aAxis[2].m_pt0 = vCenter;
            this->aAxis[2].m_pt1 = vAxisZ;

            //Distance 
            float aDistances[3] = 
            {
                FMath::Length2(vCameraPos - vAxisX),
                FMath::Length2(vCameraPos - vAxisY),
                FMath::Length2(vCameraPos - vAxisZ),
            };
            int aSequences[3] = { 0, 1, 2 };
            FUtil::SortBubble(3, aDistances, aSequences);

            //State
            bool bHasScaleAABBSelected = false;
            bool bScaleAABBIsSelectedCenter = false;
            bool aScaleAABBIsSelected[4] =  {false, false, false, false};

            //Cylinder (6-8)
            float scaleAxis[3] = 
            {
                this->scaleCoordinate,
                this->scaleCoordinate,
                this->scaleCoordinate,
            };
            for (int i = 0; i < 3; i++)
            {
                if (IsAxisSelectedByIndex(i))
                {
                    scaleAxis[i] *= s_fScale_WhenSelect_Axis;
                    break;
                }
            }

            //AABB (15-18)
            float scaleAABBs[4] = 
            {
                this->scaleCoordinate,
                this->scaleCoordinate,
                this->scaleCoordinate,
                this->scaleCoordinate,
            };
            for (int i = 0; i < 4; i++)
            {
                if (IsScaleAABBSelectedByIndex(i))
                {
                    scaleAABBs[i] *= s_fScale_WhenSelect_AxisAABB;

                    aScaleAABBIsSelected[i] = true;
                    bHasScaleAABBSelected = true;
                    if (i == 3)
                        bScaleAABBIsSelectedCenter = true;
                    break;
                }
            }

            int countStart = 6;
            int countNumber = 3;
            //Cylinder (6-8)
            FMatrix4 aWorldCylinders[3] = 
            {
                this->mat4Trans * FMath::Scale(FVector3(this->scaleCoordinate, scaleAxis[0], scaleAxis[0])) * s_aMatrix4Transforms[countStart + 0],
                this->mat4Trans * FMath::Scale(FVector3(scaleAxis[1], this->scaleCoordinate, scaleAxis[1])) * s_aMatrix4Transforms[countStart + 1],
                this->mat4Trans * FMath::Scale(FVector3(scaleAxis[2], scaleAxis[2], this->scaleCoordinate)) * s_aMatrix4Transforms[countStart + 2],
            };
            for (int i = 0; i < countNumber; i++)
            {
                CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                int index = aSequences[2 - i];
                objConsts.g_MatWorld = aWorldCylinders[index];

                if (bHasScaleAABBSelected)
                {
                    if (bScaleAABBIsSelectedCenter && IsButtonLeftDown())
                    {
                        objConsts.color = s_aColors_Select[countStart + index];
                    }
                    else
                    {
                        if (aScaleAABBIsSelected[index])
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_Select[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Hover[countStart + index];
                            }
                        }
                        else
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_UnSelect[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Default[countStart + index];
                            }
                        }
                    }
                }
                else
                {
                    objConsts.color = s_aColors_Default[countStart + index];   
                }
            }

            //AABB (15-18)
            countStart = 15;
            FMatrix4 aWorldAABBs[4] = 
            {
                this->mat4Trans * FMath::FromTRS(FVector3(this->scaleCoordinate, 0.0f, 0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleAABBs[0], scaleAABBs[0], scaleAABBs[0])) * s_aMatrix4Transforms[countStart + 0], //X+
                this->mat4Trans * FMath::FromTRS(FVector3(0.0f, this->scaleCoordinate, 0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleAABBs[1], scaleAABBs[1], scaleAABBs[1])) * s_aMatrix4Transforms[countStart + 1], //Y+
                this->mat4Trans * FMath::FromTRS(FVector3(0.0f, 0.0f, this->scaleCoordinate), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleAABBs[2], scaleAABBs[2], scaleAABBs[2])) * s_aMatrix4Transforms[countStart + 2], //Z+
                this->mat4Trans * FMath::FromTRS(FVector3(0.0f, 0.0f,                  0.0f), FVector3(  0.0f,   0.0f,  0.0f), FVector3(scaleAABBs[3], scaleAABBs[3], scaleAABBs[3])) * s_aMatrix4Transforms[countStart + 3], //XYZ+
            };
            float aExtentAABBs[4] = 
            {
                0.5f * s_fScale_AABB * scaleAABBs[0],
                0.5f * s_fScale_AABB * scaleAABBs[1],
                0.5f * s_fScale_AABB * scaleAABBs[2],
                0.5f * s_fScale_AABBCenter * scaleAABBs[3],
            };
            //aScaleAABB
            this->aScaleAABB[0].SetCenterExtents(FMath::Transform(aWorldAABBs[0], FMath::ms_v3Zero), FVector3(aExtentAABBs[0], aExtentAABBs[0], aExtentAABBs[0]));
            this->aScaleAABB[1].SetCenterExtents(FMath::Transform(aWorldAABBs[1], FMath::ms_v3Zero), FVector3(aExtentAABBs[1], aExtentAABBs[1], aExtentAABBs[1]));
            this->aScaleAABB[2].SetCenterExtents(FMath::Transform(aWorldAABBs[2], FMath::ms_v3Zero), FVector3(aExtentAABBs[2], aExtentAABBs[2], aExtentAABBs[2]));
            this->aScaleAABB[3].SetCenterExtents(FMath::Transform(aWorldAABBs[3], FMath::ms_v3Zero), FVector3(aExtentAABBs[3], aExtentAABBs[3], aExtentAABBs[3]));

            for (int i = 0; i < countNumber + 1; i++)
            {
                CoordinateAxisObjectConstants& objConsts = this->coordinateAxisObjectCBs[countStart + i];
                int index = i;
                if (i < countNumber)
                {
                    index = aSequences[2 - i];
                }
                objConsts.g_MatWorld = aWorldAABBs[index];

                if (bHasScaleAABBSelected)
                {
                    if (bScaleAABBIsSelectedCenter && IsButtonLeftDown())
                    {
                        objConsts.color = s_aColors_Select[countStart + index];
                    }
                    else
                    {
                        if (aScaleAABBIsSelected[index])
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_Select[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Hover[countStart + index];
                            }
                        }
                        else
                        {
                            if (IsButtonLeftDown())
                            {
                                objConsts.color = s_aColors_UnSelect[countStart + index];
                            }
                            else
                            {
                                objConsts.color = s_aColors_Default[countStart + index];
                            }
                        }
                    }
                }
                else
                {
                    objConsts.color = s_aColors_Default[countStart + index];
                }
            }
        }

    void EditorCoordinateAxis::Draw(VkCommandBuffer& commandBuffer)
    {
        Base::GetWindowPtr()->bindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipelineLayout, 0, 1, &this->pPipelineGraphics->poDescriptorSets[Base::GetWindowPtr()->poSwapChainImageIndex], 0, nullptr);
        
        switch ((int)this->typeState)
        {
        case CoordinateState_Select:
        case CoordinateState_Move:
            {
                Draw_Move(commandBuffer);
                break;
            }
        case CoordinateState_Rotate:
            {
                Draw_Rotate(commandBuffer);
                break;
            }

        case CoordinateState_Scale:
            {
                Draw_Scale(commandBuffer);
                break;
            }
        }
        
    }
        void EditorCoordinateAxis::Draw_Move(VkCommandBuffer& commandBuffer)
        {
            int instanceStart = 0;
            int instanceGap = 3;
            //Quad - QuadLine
            {
                Mesh* pMesh_Quad = this->aMeshes[s_nMeshQuadIndex];
                MeshSub* pMeshSub_Quad = pMesh_Quad->aMeshSubs[0];
                Mesh* pMesh_QuadLine = this->aMeshes[s_nMeshQuadLineIndex];
                MeshSub* pMeshSub_QuadLine = pMesh_QuadLine->aMeshSubs[0];
                //Quad - QuadLine  - 0
                DrawQuad(commandBuffer, pMeshSub_Quad, instanceStart + 0);
                DrawQuadLine(commandBuffer, pMeshSub_QuadLine, instanceStart + 0 + instanceGap);
                //Quad - QuadLine  - 1
                DrawQuad(commandBuffer, pMeshSub_Quad, instanceStart + 1);
                DrawQuadLine(commandBuffer, pMeshSub_QuadLine, instanceStart + 1 + instanceGap);
                //Quad - QuadLine  - 2
                DrawQuad(commandBuffer, pMeshSub_Quad, instanceStart + 2);
                DrawQuadLine(commandBuffer, pMeshSub_QuadLine, instanceStart + 2 + instanceGap);
            }

            //Cylinder - Cone
            instanceStart = 6;
            {
                if (Base::GetWindowPtr()->cfg_isWireFrame)
                    Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline_WireFrame);
                else
                    Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline);

                Mesh* pMesh_Cylinder = this->aMeshes[s_nMeshCylinderIndex];
                MeshSub* pMeshSub_Cylinder = pMesh_Cylinder->aMeshSubs[0];
                Mesh* pMesh_Cone = this->aMeshes[s_nMeshConeIndex];
                MeshSub* pMeshSub_Cone = pMesh_Cone->aMeshSubs[0];
                //Cylinder - Cone - 0
                DrawShape(commandBuffer, pMeshSub_Cylinder, instanceStart + 0);
                DrawShape(commandBuffer, pMeshSub_Cone, instanceStart + 0 + instanceGap);
                //Cylinder - Cone - 1
                DrawShape(commandBuffer, pMeshSub_Cylinder, instanceStart + 1);
                DrawShape(commandBuffer, pMeshSub_Cone, instanceStart + 1 + instanceGap);
                //Cylinder - Cone - 2
                DrawShape(commandBuffer, pMeshSub_Cylinder, instanceStart + 2);
                DrawShape(commandBuffer, pMeshSub_Cone, instanceStart + 2 + instanceGap);
            }
        }
        void EditorCoordinateAxis::Draw_Rotate(VkCommandBuffer& commandBuffer)
        {
            int instanceStart = 12;
            //Torus
            {
                if (Base::GetWindowPtr()->cfg_isWireFrame)
                    Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline_WireFrame);
                else
                    Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline);

                Mesh* pMesh_Torus = this->aMeshes[s_nMeshTorusIndex];
                MeshSub* pMeshSub_Torus = pMesh_Torus->aMeshSubs[0];
                //Torus - 0
                DrawShape(commandBuffer, pMeshSub_Torus, instanceStart + 0);
                //Torus - 1
                DrawShape(commandBuffer, pMeshSub_Torus, instanceStart + 1);
                //Torus - 2
                DrawShape(commandBuffer, pMeshSub_Torus, instanceStart + 2);
            }
        }
        void EditorCoordinateAxis::Draw_Scale(VkCommandBuffer& commandBuffer)
        {
            if (Base::GetWindowPtr()->cfg_isWireFrame)
                Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline_WireFrame);
            else
                Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline);

            //Cylinder - Cone
            //AABB
            Mesh* pMesh_Cylinder = this->aMeshes[s_nMeshCylinderIndex];
            MeshSub* pMeshSub_Cylinder = pMesh_Cylinder->aMeshSubs[0];
            Mesh* pMesh_AABB = this->aMeshes[s_nMeshAABBIndex];
            MeshSub* pMeshSub_AABB = pMesh_AABB->aMeshSubs[0];
            int instanceStart_Cone = 6;
            int instanceStart_AABB = 15;
            //Cylinder - 0
            DrawShape(commandBuffer, pMeshSub_Cylinder, instanceStart_Cone + 0);
            //AABB - 0
            DrawShape(commandBuffer, pMeshSub_AABB, instanceStart_AABB + 0);
            //Cylinder - 1
            DrawShape(commandBuffer, pMeshSub_Cylinder, instanceStart_Cone + 1);
            //AABB - 1
            DrawShape(commandBuffer, pMeshSub_AABB, instanceStart_AABB + 1);
            //Cylinder - 2
            DrawShape(commandBuffer, pMeshSub_Cylinder, instanceStart_Cone + 2);
            //AABB - 2
            DrawShape(commandBuffer, pMeshSub_AABB, instanceStart_AABB + 2);
            //AABB - 3
            DrawShape(commandBuffer, pMeshSub_AABB, instanceStart_AABB + 3);
        }
        void EditorCoordinateAxis::DrawQuad(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart)
        {
            if (Base::GetWindowPtr()->cfg_isWireFrame)
                Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline_WireFrame);
            else
                Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline);

            VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            Base::GetWindowPtr()->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            Base::GetWindowPtr()->bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            Base::GetWindowPtr()->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, instanceStart);
        }
        void EditorCoordinateAxis::DrawQuadLine(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart)
        {
            Base::GetWindowPtr()->bindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->pPipelineGraphics->poPipeline_WireFrame2);

            VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            Base::GetWindowPtr()->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            Base::GetWindowPtr()->bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            Base::GetWindowPtr()->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, instanceStart);
        }
        void EditorCoordinateAxis::DrawShape(VkCommandBuffer& commandBuffer, MeshSub* pMeshSub, int instanceStart)
        {
            VkBuffer vertexBuffers[] = { pMeshSub->poVertexBuffer };
            VkDeviceSize offsets[] = { 0 };
            Base::GetWindowPtr()->bindVertexBuffer(commandBuffer, 0, 1, vertexBuffers, offsets);
            Base::GetWindowPtr()->bindIndexBuffer(commandBuffer, pMeshSub->poIndexBuffer, 0, VK_INDEX_TYPE_UINT32);
            
            Base::GetWindowPtr()->drawIndexed(commandBuffer, pMeshSub->poIndexCount, pMeshSub->instanceCount, 0, 0, instanceStart);
        }

    void EditorCoordinateAxis::CheckStateMove(double x, double y)
    {
        FVector3 vInter;
        FRay ray;
        this->pCamera->ConvertScreenPos2ToWorldRay((float)x, (float)y, this->vViewport, &ray);
        if (FMath::Intersects_RayAABB_Test(ray, this->aQuadAABB[0]))
        {
            SetQuadSelected(CoordinateElement_Quad_XY);
        }
        else if (FMath::Intersects_RayAABB_Test(ray, this->aQuadAABB[1]))
        {
            SetQuadSelected(CoordinateElement_Quad_YZ);
        }
        else if (FMath::Intersects_RayAABB_Test(ray, this->aQuadAABB[2]))
        {
            SetQuadSelected(CoordinateElement_Quad_ZX);
        }
        else if (FMath::Intersects_RaySegment_Test(ray, this->aAxis[0], 0.01f) || FMath::Intersects_RayCone_Test(ray, this->aCone[0]))
        {
            SetAxisSelected(CoordinateElement_Axis_X);
        }
        else if (FMath::Intersects_RaySegment_Test(ray, this->aAxis[1], 0.01f) || FMath::Intersects_RayCone_Test(ray, this->aCone[1]))
        {
            SetAxisSelected(CoordinateElement_Axis_Y);
        }
        else if (FMath::Intersects_RaySegment_Test(ray, this->aAxis[2], 0.01f) || FMath::Intersects_RayCone_Test(ray, this->aCone[2]))
        {
            SetAxisSelected(CoordinateElement_Axis_Z);
        }
        else
        {
            ClearSelectState();
        }
    }
    void EditorCoordinateAxis::CheckStateRotate(double x, double y)
    {
        FVector3 vInter;
        if (FUtil::IntersectLines((float)x, (float)y, this->aTorusX, 30, this->pCamera, this->vViewport, vInter, false))
        {
            SetAxisSelected(CoordinateElement_Axis_X);
        }
        else if (FUtil::IntersectLines((float)x, (float)y, this->aTorusY, 30, this->pCamera, this->vViewport, vInter, false))
        {
            SetAxisSelected(CoordinateElement_Axis_Y);
        }
        else if (FUtil::IntersectLines((float)x, (float)y, this->aTorusZ, 30, this->pCamera, this->vViewport, vInter, false))
        {
            SetAxisSelected(CoordinateElement_Axis_Z);
        }
        else
        {
            ClearSelectState();
        }
    }
    void EditorCoordinateAxis::CheckStateScale(double x, double y)
    {
        FVector3 vInter;
        FRay ray;
        this->pCamera->ConvertScreenPos2ToWorldRay((float)x, (float)y, this->vViewport, &ray);
        if (FMath::Intersects_RayAABB_Test(ray, this->aScaleAABB[3]))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_XYZ);
        }
        else if (FMath::Intersects_RayAABB_Test(ray, this->aScaleAABB[0]))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_X);
        }
        else if (FMath::Intersects_RayAABB_Test(ray, this->aScaleAABB[1]))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_Y);
        }
        else if (FMath::Intersects_RayAABB_Test(ray, this->aScaleAABB[2]))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_Z);
        }
        else if (FMath::Intersects_RaySegment_Test(ray, this->aAxis[0], 0.01f))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_X);
        }
        else if (FMath::Intersects_RaySegment_Test(ray, this->aAxis[1], 0.01f))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_Y);
        }
        else if (FMath::Intersects_RaySegment_Test(ray, this->aAxis[2], 0.01f))
        {
            SetScaleAABBSelected(CoordinateElement_Axis_Z);
        }
        else
        {
            ClearSelectState();
        }
    }

    void EditorCoordinateAxis::Move(double deltaX, double deltaY)
    {

    }
    void EditorCoordinateAxis::Rotate(double deltaX, double deltaY)
    {

    }
    void EditorCoordinateAxis::Scale(double deltaX, double deltaY)
    {

    }

    void EditorCoordinateAxis::MouseLeftDown(double x, double y)
    {
        this->isButtonLeftDown = true;
        {
            switch ((int)this->typeState)
            {
            case CoordinateState_Select:
            case CoordinateState_Move:
                {
                    CheckStateMove(x, y);
                    break;
                }
            case CoordinateState_Rotate:
                {
                    CheckStateRotate(x, y);
                    break;
                }

            case CoordinateState_Scale:
                {
                    CheckStateScale(x, y);
                    break;
                }
            }
        }
        this->ptLastX = x;
        this->ptLastY = y;
    }
    void EditorCoordinateAxis::MouseMove(double x, double y)
    {
        double deltaX = x - this->ptLastX;
        double deltaY = y - this->ptLastY;
        {
            switch ((int)this->typeState)
            {
            case CoordinateState_Select:
                {
                    break;
                }
            case CoordinateState_Move:
                {
                    Move(deltaX, deltaY);
                    break;
                }
            case CoordinateState_Rotate:
                {
                    Rotate(deltaX, deltaY);
                    break;
                }

            case CoordinateState_Scale:
                {
                    Scale(deltaX, deltaY);
                    break;
                }
            }
        }
        this->ptLastX = x;
        this->ptLastY = y;
    }
    void EditorCoordinateAxis::MouseLeftUp(double x, double y)
    {
        this->isButtonLeftDown = false;
        double deltaX = x - this->ptLastX;
        double deltaY = y - this->ptLastY;
        {
            switch ((int)this->typeState)
            {
            case CoordinateState_Select:
                {
                    break;
                }
            case CoordinateState_Move:
                {
                    Move(deltaX, deltaY);
                    break;
                }
            case CoordinateState_Rotate:
                {
                    Rotate(deltaX, deltaY);
                    break;
                }

            case CoordinateState_Scale:
                {
                    Scale(deltaX, deltaY);
                    break;
                }
            }
        }
        this->ptLastX = 0;
        this->ptLastY = 0;
    }
    void EditorCoordinateAxis::MouseHover(double x, double y)
    {
        if (this->isButtonLeftDown)
           return;

        switch ((int)this->typeState)
        {
        case CoordinateState_Select:
        case CoordinateState_Move:
            {
                CheckStateMove(x, y);
                break;
            }
        case CoordinateState_Rotate:
            {
                CheckStateRotate(x, y);
                break;
            }

        case CoordinateState_Scale:
            {
                CheckStateScale(x, y);
                break;
            }
        }
    }
    void EditorCoordinateAxis::initConfigs()
    {
        //1> Mesh
        {
            //0: Quad
            MeshInfo* pMIQuad = new MeshInfo("EditorCameraAxis_Quad",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityQuad,
                                             new FMeshCreateParam_EntityQuad(0.5f, 0.5f, 1.0f, 1.0f, 0.0f, false, false),
                                             true,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIQuad);
            //1: QuadLine
            MeshInfo* pMIQuadLine = new MeshInfo("EditorCameraAxis_QuadLine",
                                                 "",
                                                 F_Mesh_Geometry,
                                                 F_MeshVertex_Pos3Color4,
                                                 F_MeshGeometry_LineQuad3D,
                                                 new FMeshCreateParam_LineQuad3D(FVector3(0, 1, 0), FVector3(0, 0, 0), FVector3(1, 0, 0), FVector3(1, 1, 0)),
                                                 true,
                                                 false,
                                                 FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIQuadLine);
            //2: Cylinder
            MeshInfo* pMICylinder = new MeshInfo("EditorCoordinateAxis_Cylinder",
                                                 "",
                                                 F_Mesh_Geometry,
                                                 F_MeshVertex_Pos3Color4Tex2,
                                                 F_MeshGeometry_EntityCylinder,
                                                 new FMeshCreateParam_EntityCylinder(0.5f, 0.5f, 1.0f, 0.5f, 5, 1, false, false),
                                                 false,
                                                 false,
                                                 FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMICylinder);
            //3: Cone
            this->pEntityCone = new FMeshCreateParam_EntityCone(0.5f, 2.0f, 0.0f, 16, 1, false, false);
            MeshInfo* pMICone = new MeshInfo("EditorCoordinateAxis_Cone",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityCone,
                                             this->pEntityCone,
                                             false,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMICone);
            //4: Torus
            MeshInfo* pMITorus = new MeshInfo("EditorCoordinateAxis_Torus",
                                              "",
                                              F_Mesh_Geometry,
                                              F_MeshVertex_Pos3Color4Tex2,
                                              F_MeshGeometry_EntityTorus,
                                              new FMeshCreateParam_EntityTorus(0.5f, 0.005f, 5, 100, false, false),
                                              false,
                                              false,
                                              FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMITorus);    
            //5: AABB
            MeshInfo* pMIAABB = new MeshInfo("EditorCoordinateAxis_AABB",
                                             "",
                                             F_Mesh_Geometry,
                                             F_MeshVertex_Pos3Color4Tex2,
                                             F_MeshGeometry_EntityAABB,
                                             nullptr,
                                             false,
                                             false,
                                             FMath::ms_mat4Unit);
            this->aMeshInfos.push_back(pMIAABB);                      
        }
        //2> Shader
        {
            //CoordinateAxis
            {
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_CoordinateAxis_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = "Assets/Shader/editor_coordinate_axis.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_CoordinateAxis_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/editor_coordinate_axis.frag.spv";
                this->aShaderModuleInfos.push_back(siFrag);
            }
            //CoordinateQuadLine
            {   
                //Vert
                ShaderModuleInfo siVert;
                siVert.nameShader = s_strNameShader_CoordinateAxisLine_Vert;
                siVert.nameShaderType = "vert";
                siVert.pathShader = "Assets/Shader/editor_coordinate_axis_line.vert.spv";
                this->aShaderModuleInfos.push_back(siVert);
                //Frag
                ShaderModuleInfo siFrag;
                siFrag.nameShader = s_strNameShader_CoordinateAxisLine_Frag;
                siFrag.nameShaderType = "frag";
                siFrag.pathShader = "Assets/Shader/editor_coordinate_axis_line.frag.spv";
                this->aShaderModuleInfos.push_back(siFrag);
            }
        }
        //3> BufferUniform
        {

        }
        //4> DescriptorSetLayout
        {
            //CoordinateAxis
            {
                this->nameDescriptorSetLayout = "Pass-ObjectCoordinateAxis";
                this->aNameDescriptorSetLayouts = FUtilString::Split(this->nameDescriptorSetLayout, "-");
            }
        }
    }
    void EditorCoordinateAxis::initBufferUniforms()
    {
        //CoordinateAxis
        {
            this->coordinateAxisObjectCBs.clear();
            //Axis Quad 
            //Axis Quad Line
            //Axis Cylinder
            //Axis Cone
            //Axis Torus
            //Axis AABB
            for (int i = 0; i < 19; i++)
            {
                CoordinateAxisObjectConstants consts;
                consts.g_MatWorld = s_aMatrix4Transforms[i];
                consts.color = s_aColors_Default[i];
                this->coordinateAxisObjectCBs.push_back(consts);
            }
            
            VkDeviceSize bufferSize = sizeof(CoordinateAxisObjectConstants) * this->coordinateAxisObjectCBs.size();
            Base::GetWindowPtr()->createVkBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, this->poBuffers_ObjectCB, this->poBuffersMemory_ObjectCB);

            //Quad
            Mesh* pMesh = this->aMeshes[s_nMeshQuadIndex]; 
            MeshSub* pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
            //Quad Line
            pMesh = this->aMeshes[s_nMeshQuadLineIndex]; 
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
            //Cylinder
            pMesh = this->aMeshes[s_nMeshCylinderIndex]; 
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
            //Cone
            pMesh = this->aMeshes[s_nMeshConeIndex]; 
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
            //Torus
            pMesh = this->aMeshes[s_nMeshTorusIndex]; 
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
            //AABB
            pMesh = this->aMeshes[s_nMeshAABBIndex]; 
            pMeshSub = pMesh->aMeshSubs[0];
            pMeshSub->instanceCount = 1;
        }
    }
    void EditorCoordinateAxis::initPipelineGraphics()
    {
        //0> Pipeline Graphics/Pipeline GraphicsQuadLine
        this->pPipelineGraphics = new VKPipelineGraphics("PipelineG-EditorCoordinateAxis");
        this->pPipelineGraphics->nameDescriptorSetLayout = this->nameDescriptorSetLayout;
        this->pPipelineGraphics->poDescriptorSetLayoutNames = &this->aNameDescriptorSetLayouts;
        //1> DescriptorSetLayout 
        this->pPipelineGraphics->poDescriptorSetLayout = this->poDescriptorSetLayout;
        //2> DescriptorSets
        Base::GetWindowPtr()->createVkDescriptorSets(this->pPipelineGraphics->poDescriptorSetLayout, this->pPipelineGraphics->poDescriptorSets);
        updateDescriptorSets_Graphics();
        //3> PipelineLayout
        this->pPipelineGraphics->poPipelineLayout = this->poPipelineLayout;
        //4> Pipeline
        {
            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics;
            if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_CoordinateAxis_Vert,
                                                                            "",
                                                                            "",
                                                                            "",
                                                                            s_strNameShader_CoordinateAxis_Frag,
                                                                            this->mapShaderModules,
                                                                            aShaderStageCreateInfos_Graphics))
            {
                String msg = "*********************** EditorCoordinateAxis::initPipelineGraphics: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }

            VkViewportVector aViewports;
            aViewports.push_back(Base::GetWindowPtr()->poViewport);
            VkRect2DVector aScissors;
            aScissors.push_back(Base::GetWindowPtr()->poScissor);

            VkStencilOpState stencilOpFront; 
            VkStencilOpState stencilOpBack; 

            //pPipelineGraphics->poPipeline
            this->pPipelineGraphics->poPipeline = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                 false, 0, 3,
                                                                                                 Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                 Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                 Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                                 VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, 1.0f,
                                                                                                 VK_TRUE, VK_FALSE, VK_COMPARE_OP_ALWAYS,
                                                                                                 VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                 VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                 VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                 VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->pPipelineGraphics->poPipeline == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorCoordinateAxis::initPipelineGraphics: Failed to create pipeline graphics for [EditorCoordinateAxis] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorCoordinateAxis::initPipelineGraphics: [EditorCoordinateAxis] Create pipeline graphics success !");

            //pPipelineGraphics->poPipeline_WireFrame
            this->pPipelineGraphics->poPipeline_WireFrame = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics,
                                                                                                           false, 0, 3,
                                                                                                           Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2), 
                                                                                                           Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4Tex2),
                                                                                                           Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                                           VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, 1.0f,
                                                                                                           VK_TRUE, VK_FALSE, VK_COMPARE_OP_ALWAYS,
                                                                                                           VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                           VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                           VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                           VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->pPipelineGraphics->poPipeline_WireFrame == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorCoordinateAxis::initPipelineGraphics: Failed to create pipeline graphics wire frame for [EditorCoordinateAxis] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorCoordinateAxis::initPipelineGraphics: [EditorCoordinateAxis] Create pipeline graphics wire frame success !");

            //pPipelineGraphics->poPipeline_WireFrame2
            VkPipelineShaderStageCreateInfoVector aShaderStageCreateInfos_Graphics_Wire;
            if (!Base::GetWindowPtr()->CreatePipelineShaderStageCreateInfos(s_strNameShader_CoordinateAxisLine_Vert,
                                                                            "",
                                                                            "",
                                                                            "",
                                                                            s_strNameShader_CoordinateAxisLine_Frag,
                                                                            this->mapShaderModules,
                                                                            aShaderStageCreateInfos_Graphics_Wire))
            {
                String msg = "*********************** EditorCoordinateAxis::initPipelineGraphics: Can not find shader used !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            this->pPipelineGraphics->poPipeline_WireFrame2 = Base::GetWindowPtr()->createVkGraphicsPipeline(aShaderStageCreateInfos_Graphics_Wire,
                                                                                                            false, 0, 3,
                                                                                                            Util_GetVkVertexInputBindingDescriptionVectorPtr(F_MeshVertex_Pos3Color4), 
                                                                                                            Util_GetVkVertexInputAttributeDescriptionVectorPtr(F_MeshVertex_Pos3Color4),
                                                                                                            Base::GetWindowPtr()->poRenderPass, this->pPipelineGraphics->poPipelineLayout, aViewports, aScissors,
                                                                                                            VK_PRIMITIVE_TOPOLOGY_LINE_LIST, VK_FRONT_FACE_CLOCKWISE, VK_POLYGON_MODE_LINE, VK_CULL_MODE_NONE, 1.0f,
                                                                                                            VK_TRUE, VK_FALSE, VK_COMPARE_OP_ALWAYS,
                                                                                                            VK_FALSE, stencilOpFront, stencilOpBack, 
                                                                                                            VK_TRUE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA, VK_BLEND_OP_ADD,
                                                                                                            VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
                                                                                                            VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT);
            if (this->pPipelineGraphics->poPipeline_WireFrame2 == VK_NULL_HANDLE)
            {
                String msg = "*********************** EditorCoordinateAxis::initPipelineGraphics: Failed to create pipeline graphics wire frame 2 for [EditorCoordinateAxis] !";
                F_LogError(msg.c_str());
                throw std::runtime_error(msg.c_str());
            }
            F_LogInfo("EditorCoordinateAxis::initPipelineGraphics: [EditorCoordinateAxis] Create pipeline graphics wire frame 2 success !");
        }
    }
    void EditorCoordinateAxis::updateDescriptorSets_Graphics()
    {
        StringVector* pDescriptorSetLayoutNames = this->pPipelineGraphics->poDescriptorSetLayoutNames;
        F_Assert(pDescriptorSetLayoutNames != nullptr && "EditorCoordinateAxis::updateDescriptorSets_Graphics")
        size_t count_ds = this->pPipelineGraphics->poDescriptorSets.size();
        for (size_t i = 0; i < count_ds; i++)
        {
            VkWriteDescriptorSetVector descriptorWrites;

            size_t count_names = pDescriptorSetLayoutNames->size();
            for (size_t j = 0; j < count_names; j++)
            {
                String& nameDescriptorSet = (*pDescriptorSetLayoutNames)[j];
                if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_Pass)) //Pass
                {
                    VkDescriptorBufferInfo bufferInfo_Pass = {};
                    bufferInfo_Pass.buffer = Base::GetWindowPtr()->poBuffers_PassCB[i];
                    bufferInfo_Pass.offset = 0;
                    bufferInfo_Pass.range = sizeof(PassConstants);
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->pPipelineGraphics->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_Pass);
                }
                else if (nameDescriptorSet == Util_GetDescriptorSetTypeName(Vulkan_DescriptorSet_ObjectCoordinateAxis)) //ObjectCoordinateAxis
                {
                    VkDescriptorBufferInfo bufferInfo_ObjectCameraAxis = {};
                    bufferInfo_ObjectCameraAxis.buffer = this->poBuffers_ObjectCB;
                    bufferInfo_ObjectCameraAxis.offset = 0;
                    bufferInfo_ObjectCameraAxis.range = sizeof(CoordinateAxisObjectConstants) * this->coordinateAxisObjectCBs.size();
                    Base::GetWindowPtr()->pushVkDescriptorSet_Uniform(descriptorWrites,
                                                                      this->pPipelineGraphics->poDescriptorSets[i],
                                                                      j,
                                                                      0,
                                                                      1,
                                                                      bufferInfo_ObjectCameraAxis);
                }
                else
                {
                    String msg = "*********************** EditorCoordinateAxis::updateDescriptorSets_Graphics: Graphics: Wrong DescriptorSetLayout type: " + nameDescriptorSet;
                    F_LogError(msg.c_str());
                    throw std::runtime_error(msg.c_str());
                }
            }
            Base::GetWindowPtr()->updateVkDescriptorSets(descriptorWrites);
        }
    }
    void EditorCoordinateAxis::destroyBufferUniforms()
    {
        if (this->poBuffers_ObjectCB != VK_NULL_HANDLE)
        {
            Base::GetWindowPtr()->destroyVkBuffer(this->poBuffers_ObjectCB, this->poBuffersMemory_ObjectCB);
        }
        this->poBuffers_ObjectCB = VK_NULL_HANDLE;
        this->poBuffersMemory_ObjectCB = VK_NULL_HANDLE;
        this->coordinateAxisObjectCBs.clear();
    }
    void EditorCoordinateAxis::destroyPipelineGraphics()
    {
        EditorBase::destroyPipelineGraphics();
    }
    void EditorCoordinateAxis::CleanupSwapChain()
    {
        EditorBase::CleanupSwapChain();

    }
    void EditorCoordinateAxis::RecreateSwapChain()
    {
        EditorBase::RecreateSwapChain();

        this->vViewport = Base::GetWindowPtr()->GetViewportVector4();
    }    

}; //LostPeterVulkan