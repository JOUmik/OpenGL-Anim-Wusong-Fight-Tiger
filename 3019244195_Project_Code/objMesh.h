#pragma once
#include <glm/glm.hpp>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <glfw/glfw3.h>
#include "shader.h"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	std::string type;
	std::string path;   //存储纹理路径，判断要用的纹理是否被加载过，如果对应上则直接用，减少时间开销
};

class ObjMesh
{
public:

	//网格数据
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture> textures;

	ObjMesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);
	~ObjMesh();

	void Draw(Shader* shader);

private:
	GLuint VAO, VBO, EBO;
	void setupMesh();
};