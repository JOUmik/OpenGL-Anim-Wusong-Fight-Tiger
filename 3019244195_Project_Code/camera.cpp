#include "camera.h"
Camera::Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;
	Forward = glm::normalize(target - position);
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::Camera(glm::vec3 position, glm::vec3 worldup)
{
	Position = position;
	WorldUp = worldup;

	//计算欧拉角以确定相机朝向
	Forward.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Forward.y = sin(glm::radians(Pitch));
	Forward.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw)); //相机看向z轴负方向
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Forward, Right));
}

Camera::~Camera()
{
}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(Position, Position + Forward, WorldUp);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = 5.0f * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Forward * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Forward * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

void Camera::ProcessLeftMouseMovement(double deltaX, double deltaY)
{
	if (deltaX < 0 && Fov < 160)
	{
		Fov += 1;
	}
	else if (deltaX > 0 && Fov > 20)
	{
		Fov -= 1;
	}

	this->UpdateCameraVectors();
}

void Camera::ProcessRightMouseMovement(double deltaX, double deltaY)
{
	if (deltaX > 0)
	{
		Yaw += 0.4;
	}
	else if (deltaX < 0)
	{
		Yaw -= 0.4;
	}
	if (deltaY > 0)
	{
		Pitch -= 0.4;
	}
	else if (deltaY < 0)
	{
		Pitch += 0.4;
	}

	this->UpdateCameraVectors();
}

void Camera::UpdateCameraVectors()
{
	Forward.x = cos(glm::radians(Pitch)) * sin(glm::radians(Yaw));
	Forward.y = sin(glm::radians(Pitch));
	Forward.z = -cos(glm::radians(Pitch)) * cos(glm::radians(Yaw));
	Right = glm::normalize(glm::cross(Forward, WorldUp));
	Up = glm::normalize(glm::cross(Forward, Right));
}
