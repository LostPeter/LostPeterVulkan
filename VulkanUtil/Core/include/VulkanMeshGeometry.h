// author: LostPeter
// time:   2022-10-30

#ifndef _VULKAN_GEOMETRY_H_
#define _VULKAN_GEOMETRY_H_

#include "PreDefine.h"

namespace LostPeter
{
    class utilExport VulkanMeshGeometry
	{
    public:
        static float s_fPI_Half;
        static float s_fPI;
        static float s_fPI_Two;

	public:
		// Create Quad
		static void CreateQuad(MeshData& meshData,
							   float x,
							   float y,
							   float w,
							   float h,
							   float depth);

		// Create Grid
		static void CreateGrid(MeshData& meshData,
							   float width,
							   float depth,
							   uint32 m,
							   uint32 n);

		// Create Box
		static void CreateBox(MeshData& meshData,
							  float width,
							  float height,
							  float depth,
							  uint32 numSubdivisions);

		// Create Sphere
		static void CreateSphere(MeshData& meshData,
								 float radius,
								 uint32 sliceCount,
								 uint32 stackCount);

		// Create Geosphere
		static void CreateGeosphere(MeshData& meshData,
									float radius,
									uint32 numSubdivisions);

		// Create Cylinder
		static void CreateCylinder(MeshData& meshData,
								   float bottomRadius,
								   float topRadius,
								   float height,
								   uint32 sliceCount,
								   uint32 stackCount);

	private:
		static void subdivide(MeshData& meshData);
		static MeshVertex midPoint(const MeshVertex& v0, const MeshVertex& v1);
		static void buildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
		static void buildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
	};

}; //LostPeter

#endif