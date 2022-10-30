// author: LostPeter
// time:   2022-10-30

#ifndef _GEOMETRY_UTIL_H_
#define _GEOMETRY_UTIL_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport GeometryUtil
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
		static Vertex midPoint(const Vertex& v0, const Vertex& v1);
		static void buildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
		static void buildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData);
	};

}; //LibUtil

#endif