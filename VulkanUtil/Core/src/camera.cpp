#include "../include/preinclude.h"
#include "../include/camera.h"

namespace LibUtil
{
    Camera::Camera()
		: position(0.0f, 0.0f, 0.0f)
		, right(1.0f, 0.0f, 0.0f)
		, up(0.0f, 1.0f, 0.0f)
		, look(0.0f, 0.0f, 1.0f)
		, nearZ(0.0f)
		, farZ(0.0f)
		, aspect(0.0f)
		, fovY(0.0f)
		, nearWindowHeight(0.0f)
		, farWindowHeight(0.0f)
		, isViewDirty(true)
		, matView(MathUtil::Identity4x4())
		, matProj(MathUtil::Identity4x4())
	{
		SetLens(0.25f * MathUtil::ms_fPI, 1.0f, 1.0f, 1000.0f);
	}

	Camera::~Camera()
	{

	}

	glm::vec3 Camera::GetPosition()const
	{
		return this->position;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		this->position = glm::vec3(x, y, z);
		this->isViewDirty = true;
	}

	void Camera::SetPosition(const glm::vec3& v)
	{
		this->position = v;
		this->isViewDirty = true;
	}

	glm::vec3 Camera::GetRight()const
	{
		return this->right;
	}

	glm::vec3 Camera::GetUp()const
	{
		return this->up;
	}

	glm::vec3 Camera::GetLook()const
	{
		return this->look;
	}

	float Camera::GetNearZ()const
	{
		return this->nearZ;
	}

	float Camera::GetFarZ()const
	{
		return this->farZ;
	}

	float Camera::GetAspect()const
	{
		return this->aspect;
	}

	float Camera::GetFovY()const
	{
		return this->fovY;
	}

	float Camera::GetFovX()const
	{
		float halfWidth = 0.5f * GetNearWindowWidth();
		return 2.0f * atan(halfWidth / this->nearZ);
	}

	float Camera::GetNearWindowWidth()const
	{
		return this->aspect * this->nearWindowHeight;
	}

	float Camera::GetNearWindowHeight()const
	{
		return this->nearWindowHeight;
	}

	float Camera::GetFarWindowWidth()const
	{
		return this->aspect * this->farWindowHeight;
	}

	float Camera::GetFarWindowHeight()const
	{
		return this->farWindowHeight;
	}

	void Camera::SetLens(float fovY, float aspect, float zn, float zf)
	{
		// cache properties
		this->fovY = fovY;
		this->aspect = aspect;
		this->nearZ = zn;
		this->farZ = zf;

		this->nearWindowHeight = 2.0f * this->nearZ * tanf(0.5f * this->fovY);
		this->farWindowHeight = 2.0f * this->farZ * tanf(0.5f * this->fovY);

		this->matProj = glm::perspective(this->fovY, this->aspect, this->nearZ, this->farZ);
	}

	void Camera::LookAt(const glm::vec3& pos, const glm::vec3& target, const glm::vec3& worldUp)
	{
		glm::mat4 mat = glm::lookAt(pos, target, worldUp);
		
		//pos
		this->position = pos;
		//right
		this->right.x = mat[0][0];
		this->right.y = mat[1][0];
		this->right.z = mat[2][0];
		//up
		this->up.x = mat[0][1];
		this->up.y = mat[1][1];
		this->up.z = mat[2][1];
		//look
		this->look.x = mat[0][2];
		this->look.y = mat[1][2];
		this->look.z = mat[2][2];

		this->isViewDirty = true;
	}

	glm::mat4 Camera::GetView()const
	{
		assert(!this->isViewDirty);
		return this->matView;
	}

	glm::mat4 Camera::GetProj()const
	{
		return this->matProj;
	}

	void Camera::Strafe(float d)
	{

	}

	void Camera::Walk(float d)
	{
		
	}

	void Camera::Pitch(float angle)
	{
		
	}

	void Camera::RotateY(float angle)
	{
		
	}

	void Camera::UpdateViewMatrix()
	{
		if (this->isViewDirty)
		{
			this->isViewDirty = false;
		}
	}

}; //LibUtil