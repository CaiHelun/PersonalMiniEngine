#include "Camera.h"

Camera::Camera(glm::vec3 pos, glm::vec3 up, float yaw, float pitch) 
	:mCameraFront(glm::vec3(0.0f, 0.0f, -1.0f))
	, mCameraUp(glm::vec3(.0f))
	, mCameraRight(glm::vec3(.0f))
	, mCameraSpeed(SPEED)
	, mSensitivity(SENSITIVITY)
{
	mCameraPosition = pos;
	mWorldUp = up;
	mYaw = yaw;
	mPitch = pitch;
	_UpdateCamera();
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(mCameraPosition, mCameraPosition + mCameraFront, mCameraUp);
}

void Camera::OnMouseRightButtonDrag(float xOffset, float yOffset, bool constrainPitch)
{
	xOffset *= mSensitivity;
	yOffset *= mSensitivity;
	mYaw += xOffset;
	mPitch += yOffset;
	if (constrainPitch)
	{
		if (mPitch > 89.0f)
			mPitch = 89.0f;
		if (mPitch < -89.0f)
			mPitch = 89.0f;
	}
	_UpdateCamera();
}

void Camera::OnMouseMiddleButtonDrag(float xOffset, float yOffset, bool constrainPitch)
{
	mCameraPosition += glm::vec3(.0f, 1.0f, .0f) * (float)yOffset * mMiddleDragSpeed;
	mCameraPosition -= glm::normalize(glm::cross(mCameraFront, mCameraUp)) * (float)xOffset * mMiddleDragSpeed;
}

void Camera::OnMouseWheel(float wheel)
{
	if (wheel > .0f)
		mCameraPosition += mCameraSpeed * mCameraFront;
	else if (wheel < .0f)
		mCameraPosition -= mCameraSpeed * mCameraFront;
	_UpdateCamera();
}

void Camera::_UpdateCamera()
{
	glm::vec3 front(.0f);
	front.x= cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	front.y = sin(glm::radians(mPitch));
	front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));
	mCameraFront = glm::normalize(front);
	mCameraRight = glm::normalize(glm::cross(mCameraFront, mWorldUp));
	mCameraUp = glm::normalize(glm::cross(mCameraRight, mCameraFront));
}
