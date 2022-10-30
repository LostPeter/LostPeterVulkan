// author: LostPeter
// time:   2022-10-30

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "predefine.h"

namespace LibUtil
{
    class utilExport Camera
	{
	public:
		Camera();
		~Camera();

    private:
		// Camera coordinate system with coordinates relative to world space.
		glm::vec3 position;
		glm::vec3 right;
		glm::vec3 up;
		glm::vec3 look;

		// Cache frustum properties.
		float nearZ;
		float farZ;
		float aspect;
		float fovY;
		float nearWindowHeight;
		float farWindowHeight;

		bool isViewDirty;

		// Cache View/Proj matrices.
		glm::mat4 matView;
		glm::mat4 matProj;

    public:
		// Get/Set world camera position.
		glm::vec3 GetPosition()const;
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& v);

		// Get camera basis vectors.
		glm::vec3 GetRight()const;
		glm::vec3 GetUp()const;
		glm::vec3 GetLook()const;

		// Get frustum properties.
		float GetNearZ()const;
		float GetFarZ()const;
		float GetAspect()const;
		float GetFovY()const;
		float GetFovX()const;

		// Get near and far plane dimensions in view space coordinates.
		float GetNearWindowWidth()const;
		float GetNearWindowHeight()const;
		float GetFarWindowWidth()const;
		float GetFarWindowHeight()const;

		// Set frustum.
		void SetLens(float fovY, float aspect, float zn, float zf);

		// Define camera space via LookAt parameters.
		void LookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& up);

		// Get View/Proj matrices.
		glm::mat4 GetView()const;
		glm::mat4 GetProj()const;

		// Strafe/Walk the camera a distance d.
		void Strafe(float d);
		void Walk(float d);

		// Rotate the camera.
		void Pitch(float angle);
		void RotateY(float angle);

		// After modifying camera position/orientation, call to rebuild the view matrix.
		void UpdateViewMatrix();
    };

}; //LibUtil

#endif