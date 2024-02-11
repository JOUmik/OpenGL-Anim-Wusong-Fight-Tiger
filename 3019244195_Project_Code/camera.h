#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN,
};

class Camera
{
public:
	glm::vec3 Position;
	glm::vec3 Forward;
	glm::vec3 Right;
	glm::vec3 Up;
	glm::vec3 WorldUp;

	GLuint WIDTH = 800, HEIGHT = 600;
	GLfloat zNear = 0.1, zFar = 150.0;

	float Pitch = -18.8f;
	float Yaw = 144.8f;
	float Fov = 40.0f;

	Camera(glm::vec3 position, glm::vec3 target, glm::vec3 worldup);
	Camera(glm::vec3 position, glm::vec3 worldup);
	~Camera();
	glm::mat4 GetViewMatrix();

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessLeftMouseMovement(double deltaX, double deltaY);
	void ProcessRightMouseMovement(double deltaX, double deltaY);
	void UpdateCameraVectors();
};