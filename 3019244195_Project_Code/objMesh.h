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
	std::string path;   //�洢����·�����ж�Ҫ�õ������Ƿ񱻼��ع��������Ӧ����ֱ���ã�����ʱ�俪��
};

class ObjMesh
{
public:

	//��������
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