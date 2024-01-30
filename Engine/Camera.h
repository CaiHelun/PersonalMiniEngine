#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;


class Camera
{
public:
	glm::vec3 mCameraPosition;
	glm::vec3 mCameraFront;
	glm::vec3 mCameraUp;
	glm::vec3 mCameraRight;
	glm::vec3 mWorldUp;
	float mYaw;
	float mPitch;
	float mCameraSpeed = 2.5f;
	float mSensitivity = 0.1f;
	float mMiddleDragSpeed = 0.01f;

	Camera(glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
		float yaw = YAW,
		float pitch = PITCH);

	~Camera();

	glm::mat4 GetViewMatrix();

	void OnMouseRightButtonDrag(float xOffset, float yOffset, bool constrainPitch = false);
	void OnMouseMiddleButtonDrag(float xOffset, float yOffset, bool constrainPitch = false);
	void OnMouseWheel(float wheel);

private:
	void _UpdateCamera();

};