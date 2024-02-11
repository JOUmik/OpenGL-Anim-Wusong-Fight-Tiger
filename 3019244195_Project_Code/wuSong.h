#pragma once
#include <iostream>
#include <vector>
#include <map>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "model.h"

using namespace glm;

#define White	glm::vec3(1.0, 1.0, 1.0)
#define Yellow	glm::vec3(1.0, 1.0, 0.0)
#define Green	glm::vec3(0.0, 1.0, 0.0)
#define Cyan	glm::vec3(0.0, 1.0, 1.0)
#define Magenta	glm::vec3(1.0, 0.0, 1.0)
#define Red		glm::vec3(1.0, 0.0, 0.0)
#define Black	glm::vec3(0.0, 0.0, 0.0)
#define Blue	glm::vec3(0.0, 0.0, 1.0)
#define Brown	glm::vec3(0.5, 0.5, 0.5)

typedef struct WuSongMotion
{
	glm::vec3 translate, rotate, scale;
	WuSongMotion() {}
	WuSongMotion(glm::vec3 t, glm::vec3 r, glm::vec3 s)
	{
		translate = t; rotate = r; scale = s;
	}
}WuSongMotion;

class WuSong {
public:
	std::map<std::string, Model*> parts;
	std::map<std::string, std::map<std::string, WuSongMotion>> motions;
	std::map<std::string, WuSongMotion> currentMotion, lastMotion, nextMotion;
	float timer = 0;
	bool isInCycle = false;
	int cycleIndex = 0;
	std::vector<std::string> cycleList;

	WuSong();
	~WuSong();

	void playMotion();

	void setMotion(std::map<std::string, WuSongMotion>& m);

	void startMotion(std::string next, float speed);

	void startCycle(std::vector<std::string> list);

	void endCycle(std::string mName);
};