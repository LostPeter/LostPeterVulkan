#include "../include/preinclude.h"
#include "../include/geometryutil.h"

namespace LibUtil
{
    float GeometryUtil::s_fPI_Half = glm::pi<float>() / 2.0f;
    float GeometryUtil::s_fPI = glm::pi<float>();
    float GeometryUtil::s_fPI_Two = glm::pi<float>() * 2.0f;

    void GeometryUtil::CreateQuad(MeshData& meshData, 
								  float x, 
								  float y, 
								  float w, 
								  float h, 
								  float depth)
	{
		meshData.vertices.resize(4);
		meshData.indices32.resize(6);

		// Position coordinates specified in NDC space.
		meshData.vertices[0] = Vertex(
			x, y - h, depth,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f);

		meshData.vertices[1] = Vertex(
			x, y, depth,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			0.0f, 0.0f);

		meshData.vertices[2] = Vertex(
			x + w, y, depth,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f);

		meshData.vertices[3] = Vertex(
			x + w, y - h, depth,
			0.0f, 0.0f, -1.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 1.0f);

		meshData.indices32[0] = 0;
		meshData.indices32[1] = 1;
		meshData.indices32[2] = 2;

		meshData.indices32[3] = 0;
		meshData.indices32[4] = 2;
		meshData.indices32[5] = 3;
	}

	void GeometryUtil::CreateGrid(MeshData& meshData, 
								  float width, 
								  float depth, 
								  uint32 m, 
								  uint32 n)
	{
		uint32 vertexCount = m * n;
		uint32 faceCount = (m - 1) * (n - 1) * 2;

		//
		// Create the vertices.
		//

		float halfWidth = 0.5f * width;
		float halfDepth = 0.5f * depth;

		float dx = width / (n - 1);
		float dz = depth / (m - 1);

		float du = 1.0f / (n - 1);
		float dv = 1.0f / (m - 1);

		meshData.vertices.resize(vertexCount);
		for (uint32 i = 0; i < m; ++i)
		{
			float z = halfDepth - i * dz;
			for (uint32 j = 0; j < n; ++j)
			{
				float x = -halfWidth + j * dx;

				meshData.vertices[i * n + j].pos = glm::vec3(x, 0.0f, z);
				meshData.vertices[i * n + j].normal = glm::vec3(0.0f, 1.0f, 0.0f);
				meshData.vertices[i * n + j].tangent = glm::vec3(1.0f, 0.0f, 0.0f);

				// Stretch texture over grid.
				meshData.vertices[i * n + j].texCoord.x = j * du;
				meshData.vertices[i * n + j].texCoord.y = i * dv;
			}
		}

		//
		// Create the indices.
		//

		meshData.indices32.resize(faceCount * 3); // 3 indices per face

		// Iterate over each quad and compute indices.
		uint32 k = 0;
		for (uint32 i = 0; i < m - 1; ++i)
		{
			for (uint32 j = 0; j < n - 1; ++j)
			{
				meshData.indices32[k] = i * n + j;
				meshData.indices32[k + 1] = i * n + j + 1;
				meshData.indices32[k + 2] = (i + 1) * n + j;

				meshData.indices32[k + 3] = (i + 1) * n + j;
				meshData.indices32[k + 4] = i * n + j + 1;
				meshData.indices32[k + 5] = (i + 1) * n + j + 1;

				k += 6; // next quad
			}
		}
	}

	void GeometryUtil::CreateBox(MeshData& meshData, 
								 float width, 
								 float height, 
								 float depth, 
								 uint32 numSubdivisions)
	{
		//
	   // Create the vertices.
	   //

		Vertex v[24];

		float w2 = 0.5f * width;
		float h2 = 0.5f * height;
		float d2 = 0.5f * depth;

		// Fill in the front face vertex data.
		v[0] = Vertex(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[1] = Vertex(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[2] = Vertex(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[3] = Vertex(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the back face vertex data.
		v[4] = Vertex(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[5] = Vertex(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[6] = Vertex(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[7] = Vertex(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// Fill in the top face vertex data.
		v[8] = Vertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[9] = Vertex(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[10] = Vertex(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
		v[11] = Vertex(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);

		// Fill in the bottom face vertex data.
		v[12] = Vertex(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
		v[13] = Vertex(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
		v[14] = Vertex(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		v[15] = Vertex(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

		// Fill in the left face vertex data.
		v[16] = Vertex(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[17] = Vertex(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[18] = Vertex(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[19] = Vertex(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		// Fill in the right face vertex data.
		v[20] = Vertex(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
		v[21] = Vertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
		v[22] = Vertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);
		v[23] = Vertex(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);

		meshData.vertices.assign(&v[0], &v[24]);

		//
		// Create the indices.
		//

		uint32 i[36];

		// Fill in the front face index data
		i[0] = 0; i[1] = 1; i[2] = 2;
		i[3] = 0; i[4] = 2; i[5] = 3;

		// Fill in the back face index data
		i[6] = 4; i[7] = 5; i[8] = 6;
		i[9] = 4; i[10] = 6; i[11] = 7;

		// Fill in the top face index data
		i[12] = 8; i[13] = 9; i[14] = 10;
		i[15] = 8; i[16] = 10; i[17] = 11;

		// Fill in the bottom face index data
		i[18] = 12; i[19] = 13; i[20] = 14;
		i[21] = 12; i[22] = 14; i[23] = 15;

		// Fill in the left face index data
		i[24] = 16; i[25] = 17; i[26] = 18;
		i[27] = 16; i[28] = 18; i[29] = 19;

		// Fill in the right face index data
		i[30] = 20; i[31] = 21; i[32] = 22;
		i[33] = 20; i[34] = 22; i[35] = 23;

		meshData.indices32.assign(&i[0], &i[36]);

		// Put a cap on the number of subdivisions.
		numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

		for (uint32 i = 0; i < numSubdivisions; ++i)
			subdivide(meshData);
	}

	void GeometryUtil::CreateSphere(MeshData& meshData, 
									float radius, 
									uint32 sliceCount, 
									uint32 stackCount)
	{
		//
		// Compute the vertices stating at the top pole and moving down the stacks.
		//

		// Poles: note that there will be texture coordinate distortion as there is
		// not a unique point on the texture map to assign to the pole when mapping
		// a rectangular texture onto a sphere.
		Vertex topVertex(0.0f, +radius, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
		Vertex bottomVertex(0.0f, -radius, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		meshData.vertices.push_back(topVertex);

		float phiStep = s_fPI / stackCount;
		float thetaStep = 2.0f * s_fPI / sliceCount;

		// Compute vertices for each stack ring (do not count the poles as rings).
		for (uint32 i = 1; i <= stackCount - 1; ++i)
		{
			float phi = i * phiStep;

			// Vertices of ring.
			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				float theta = j * thetaStep;

				Vertex v;

				// spherical to cartesian
				v.pos.x = radius * sinf(phi) * cosf(theta);
				v.pos.y = radius * cosf(phi);
				v.pos.z = radius * sinf(phi) * sinf(theta);

				// Partial derivative of P with respect to theta
				v.tangent.x = -radius * sinf(phi) * sinf(theta);
				v.tangent.y = 0.0f;
				v.tangent.z = +radius * sinf(phi) * cosf(theta);
                v.tangent = glm::normalize(v.tangent);
                v.normal = glm::normalize(v.pos);

				v.texCoord.x = theta / s_fPI_Two;
				v.texCoord.y = phi / s_fPI;

				meshData.vertices.push_back(v);
			}
		}

		meshData.vertices.push_back(bottomVertex);

		//
		// Compute indices for top stack.  The top stack was written first to the vertex buffer
		// and connects the top pole to the first ring.
		//

		for (uint32 i = 1; i <= sliceCount; ++i)
		{
			meshData.indices32.push_back(0);
			meshData.indices32.push_back(i + 1);
			meshData.indices32.push_back(i);
		}

		//
		// Compute indices for inner stacks (not connected to poles).
		//

		// Offset the indices to the index of the first vertex in the first ring.
		// This is just skipping the top pole vertex.
		uint32 baseIndex = 1;
		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 i = 0; i < stackCount - 2; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				meshData.indices32.push_back(baseIndex + i * ringVertexCount + j);
				meshData.indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);

				meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j);
				meshData.indices32.push_back(baseIndex + i * ringVertexCount + j + 1);
				meshData.indices32.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			}
		}

		//
		// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
		// and connects the bottom pole to the bottom ring.
		//

		// South pole vertex was added last.
		uint32 southPoleIndex = (uint32)meshData.vertices.size() - 1;

		// Offset the indices to the index of the first vertex in the last ring.
		baseIndex = southPoleIndex - ringVertexCount;

		for (uint32 i = 0; i < sliceCount; ++i)
		{
			meshData.indices32.push_back(southPoleIndex);
			meshData.indices32.push_back(baseIndex + i);
			meshData.indices32.push_back(baseIndex + i + 1);
		}
	}

	void GeometryUtil::CreateGeosphere(MeshData& meshData, 
									   float radius, 
									   uint32 numSubdivisions)
	{
		// Put a cap on the number of subdivisions.
		numSubdivisions = std::min<uint32>(numSubdivisions, 6u);

		// Approximate a sphere by tessellating an icosahedron.

		const float X = 0.525731f;
		const float Z = 0.850651f;

		glm::vec3 pos[12] =
		{
			glm::vec3(-X, 0.0f, Z),  glm::vec3(X, 0.0f, Z),
			glm::vec3(-X, 0.0f, -Z), glm::vec3(X, 0.0f, -Z),
			glm::vec3(0.0f, Z, X),   glm::vec3(0.0f, Z, -X),
			glm::vec3(0.0f, -Z, X),  glm::vec3(0.0f, -Z, -X),
			glm::vec3(Z, X, 0.0f),   glm::vec3(-Z, X, 0.0f),
			glm::vec3(Z, -X, 0.0f),  glm::vec3(-Z, -X, 0.0f)
		};

		uint32 k[60] =
		{
			1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
			1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
			3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
			10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
		};

		meshData.vertices.resize(12);
		meshData.indices32.assign(&k[0], &k[60]);

		for (uint32 i = 0; i < 12; ++i)
			meshData.vertices[i].pos = pos[i];

		for (uint32 i = 0; i < numSubdivisions; ++i)
			subdivide(meshData);

		// Project vertices onto sphere and scale.
		for (uint32 i = 0; i < meshData.vertices.size(); ++i)
		{
			// Project onto unit sphere.
			meshData.vertices[i].normal = glm::normalize(meshData.vertices[i].pos);

			// Project onto sphere.
			meshData.vertices[i].pos = radius * meshData.vertices[i].normal;

			// Derive texture coordinates from spherical coordinates.
			float theta = atan2f(meshData.vertices[i].pos.z, meshData.vertices[i].pos.x);

			// Put in [0, 2pi].
			if (theta < 0.0f)
				theta += s_fPI_Two;

			float phi = acosf(meshData.vertices[i].pos.y / radius);

			meshData.vertices[i].texCoord.x = theta / s_fPI_Two;
			meshData.vertices[i].texCoord.y = phi / s_fPI;

			// Partial derivative of P with respect to theta
			meshData.vertices[i].tangent.x = -radius * sinf(phi) * sinf(theta);
			meshData.vertices[i].tangent.y = 0.0f;
			meshData.vertices[i].tangent.z = +radius * sinf(phi) * cosf(theta);
            meshData.vertices[i].tangent = glm::normalize(meshData.vertices[i].tangent);
		}
	}

	void GeometryUtil::CreateCylinder(MeshData& meshData, 
									  float bottomRadius, 
									  float topRadius, 
									  float height, 
									  uint32 sliceCount, 
									  uint32 stackCount)
	{
		//
		// Build Stacks.
		// 

		float stackHeight = height / stackCount;

		// Amount to increment radius as we move up each stack level from bottom to top.
		float radiusStep = (topRadius - bottomRadius) / stackCount;

		uint32 ringCount = stackCount + 1;

		// Compute vertices for each stack ring starting at the bottom and moving up.
		for (uint32 i = 0; i < ringCount; ++i)
		{
			float y = -0.5f * height + i * stackHeight;
			float r = bottomRadius + i * radiusStep;

			// vertices of ring
			float dTheta = 2.0f * s_fPI / sliceCount;
			for (uint32 j = 0; j <= sliceCount; ++j)
			{
				Vertex vertex;

				float c = cosf(j * dTheta);
				float s = sinf(j * dTheta);

				vertex.pos = glm::vec3(r * c, y, r * s);

				vertex.texCoord.x = (float)j / sliceCount;
				vertex.texCoord.y = 1.0f - (float)i / stackCount;

				// Cylinder can be parameterized as follows, where we introduce v
				// parameter that goes in the same direction as the v tex-coord
				// so that the bitangent goes in the same direction as the v tex-coord.
				//   Let r0 be the bottom radius and let r1 be the top radius.
				//   y(v) = h - hv for v in [0,1].
				//   r(v) = r1 + (r0-r1)v
				//
				//   x(t, v) = r(v)*cos(t)
				//   y(t, v) = h - hv
				//   z(t, v) = r(v)*sin(t)
				// 
				//  dx/dt = -r(v)*sin(t)
				//  dy/dt = 0
				//  dz/dt = +r(v)*cos(t)
				//
				//  dx/dv = (r0-r1)*cos(t)
				//  dy/dv = -h
				//  dz/dv = (r0-r1)*sin(t)

				// This is unit length.
				vertex.tangent = glm::vec3(-s, 0.0f, c);

				float dr = bottomRadius - topRadius;
				glm::vec3 bitangent(dr * c, -height, dr * s);

                glm::vec3 T = vertex.tangent;
                glm::vec3 B = bitangent;
				vertex.normal = glm::normalize(glm::cross(T, B));

				meshData.vertices.push_back(vertex);
			}
		}

		// Add one because we duplicate the first and last vertex per ring
		// since the texture coordinates are different.
		uint32 ringVertexCount = sliceCount + 1;

		// Compute indices for each stack.
		for (uint32 i = 0; i < stackCount; ++i)
		{
			for (uint32 j = 0; j < sliceCount; ++j)
			{
				meshData.indices32.push_back(i * ringVertexCount + j);
				meshData.indices32.push_back((i + 1) * ringVertexCount + j);
				meshData.indices32.push_back((i + 1) * ringVertexCount + j + 1);

				meshData.indices32.push_back(i * ringVertexCount + j);
				meshData.indices32.push_back((i + 1) * ringVertexCount + j + 1);
				meshData.indices32.push_back(i * ringVertexCount + j + 1);
			}
		}

		buildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
		buildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount, meshData);
	}

	void GeometryUtil::subdivide(MeshData& meshData)
	{
		// Save a copy of the input geometry.
		MeshData inputCopy = meshData;


		meshData.vertices.resize(0);
		meshData.indices32.resize(0);

		//       v1
		//       *
		//      / \
		//     /   \
		//  m0*-----*m1
		//   / \   / \
		//  /   \ /   \
		// *-----*-----*
		// v0    m2     v2

		uint32 numTris = (uint32)inputCopy.indices32.size() / 3;
		for (uint32 i = 0; i < numTris; ++i)
		{
			Vertex v0 = inputCopy.vertices[inputCopy.indices32[i * 3 + 0]];
			Vertex v1 = inputCopy.vertices[inputCopy.indices32[i * 3 + 1]];
			Vertex v2 = inputCopy.vertices[inputCopy.indices32[i * 3 + 2]];

			//
			// Generate the midpoints.
			//

			Vertex m0 = midPoint(v0, v1);
			Vertex m1 = midPoint(v1, v2);
			Vertex m2 = midPoint(v0, v2);

			//
			// Add new geometry.
			//

			meshData.vertices.push_back(v0); // 0
			meshData.vertices.push_back(v1); // 1
			meshData.vertices.push_back(v2); // 2
			meshData.vertices.push_back(m0); // 3
			meshData.vertices.push_back(m1); // 4
			meshData.vertices.push_back(m2); // 5

			meshData.indices32.push_back(i * 6 + 0);
			meshData.indices32.push_back(i * 6 + 3);
			meshData.indices32.push_back(i * 6 + 5);

			meshData.indices32.push_back(i * 6 + 3);
			meshData.indices32.push_back(i * 6 + 4);
			meshData.indices32.push_back(i * 6 + 5);

			meshData.indices32.push_back(i * 6 + 5);
			meshData.indices32.push_back(i * 6 + 4);
			meshData.indices32.push_back(i * 6 + 2);

			meshData.indices32.push_back(i * 6 + 3);
			meshData.indices32.push_back(i * 6 + 1);
			meshData.indices32.push_back(i * 6 + 4);
		}
	}

	Vertex GeometryUtil::midPoint(const Vertex& v0, const Vertex& v1)
	{
		glm::vec3 p0 = v0.pos;
		glm::vec3 p1 = v1.pos;

		glm::vec3 n0 = v0.normal;
		glm::vec3 n1 = v1.normal;

		glm::vec3 tan0 = v0.tangent;
		glm::vec3 tan1 = v1.tangent;

		glm::vec2 tex0 = v0.texCoord;
		glm::vec2 tex1 = v1.texCoord;

		// Compute the midpoints of all the attributes.  Vectors need to be normalized
		// since linear interpolating can make them not unit length.  
		glm::vec3 pos = 0.5f * (p0 + p1);
		glm::vec3 normal = glm::normalize(0.5f * (n0 + n1));
		glm::vec3 tangent = glm::normalize(0.5f * (tan0 + tan1));
		glm::vec2 tex = 0.5f * (tex0 + tex1);

		Vertex v;
        v.pos= pos;
        v.normal= normal;
        v.tangent= tangent;
        v.texCoord= tex;

		return v;
	}

	void GeometryUtil::buildCylinderTopCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
	{
		uint32 baseIndex = (uint32)meshData.vertices.size();

		float y = 0.5f * height;
		float dTheta = 2.0f * s_fPI / sliceCount;

		// Duplicate cap ring vertices because the texture coordinates and normals differ.
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			float x = topRadius * cosf(i * dTheta);
			float z = topRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.vertices.push_back(Vertex(x, y, z, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
		}

		// Cap center vertex.
		meshData.vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

		// Index of center vertex.
		uint32 centerIndex = (uint32)meshData.vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i)
		{
			meshData.indices32.push_back(centerIndex);
			meshData.indices32.push_back(baseIndex + i + 1);
			meshData.indices32.push_back(baseIndex + i);
		}
	}

	void GeometryUtil::buildCylinderBottomCap(float bottomRadius, float topRadius, float height, uint32 sliceCount, uint32 stackCount, MeshData& meshData)
	{
		// 
		// Build bottom cap.
		//

		uint32 baseIndex = (uint32)meshData.vertices.size();
		float y = -0.5f * height;

		// vertices of ring
		float dTheta = 2.0f * s_fPI / sliceCount;
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			float x = bottomRadius * cosf(i * dTheta);
			float z = bottomRadius * sinf(i * dTheta);

			// Scale down by the height to try and make top cap texture coord area
			// proportional to base.
			float u = x / height + 0.5f;
			float v = z / height + 0.5f;

			meshData.vertices.push_back(Vertex(x, y, z, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, u, v));
		}

		// Cap center vertex.
		meshData.vertices.push_back(Vertex(0.0f, y, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.5f, 0.5f));

		// Cache the index of center vertex.
		uint32 centerIndex = (uint32)meshData.vertices.size() - 1;

		for (uint32 i = 0; i < sliceCount; ++i)
		{
			meshData.indices32.push_back(centerIndex);
			meshData.indices32.push_back(baseIndex + i);
			meshData.indices32.push_back(baseIndex + i + 1);
		}
	}

}; //LibUtil