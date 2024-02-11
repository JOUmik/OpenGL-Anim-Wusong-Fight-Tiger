#pragma once
#pragma once
#include <iostream>
#include <vector>
#include <cmath>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#define M_PI  3.14159265358979323846

using namespace glm;

class Model {
public:
	Model();
	~Model();

	void setParent(Model* parent);

	std::vector<glm::ivec3> getFaces();
	std::vector<glm::vec3> getPoints();
	std::vector<glm::vec3> getColors();
	std::vector<glm::vec3> getNormals();
	std::vector<glm::vec2> getTextures();

	void computeTriangleNormals();
	void computeVertexNormals();

	// ��ȡ�������������תƽ�Ʊ仯
	glm::vec3 getTranslation();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::mat4 getModelMatrix();

	void setTranslation(glm::vec3 translation);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	void setNormalize(bool do_norm);

	// ���������塢ƽ��ģ�͵ĺ���
	void generateCube(glm::vec3 _color, glm::vec3 _scale);
	void generateSquare(glm::vec3 color, glm::vec3 _scale, float texScale);

	// ����ȡ�Ķ������������Ƭ�ϵĶ����±��������
	// Ҫ���ݸ�GPU��points��������
	void storeFacesPoints();

	// �������
	void cleanData();

	GLuint VAO, VBO, texture, textureType = GL_TEXTURE_2D;
	std::string texture_path;
	glm::vec3 rotatePoint = glm::vec3(0.0, 0.0, 0.0);
	void bindData();
	float reflectFactor = 0.0;
	int symbol = 0;  //symbol = 1ʱ������Ӧģ�ͽ��л���

protected:
	Model* parent;
	glm::vec3 connect_position;

	std::vector<glm::vec3> vertex_positions;	// ��������
	std::vector<glm::vec3> vertex_colors;	// ������ɫ
	std::vector<glm::vec3> vertex_normals;	// ���㷨����
	std::vector<glm::vec2> vertex_textures;	// ������������

	std::vector<glm::ivec3> faces;	// ������Ƭ��ÿ�������Ӧ�Ķ���������±�
	std::vector<glm::ivec3> normal_index;	// ÿ��������Ƭ�Ķ����Ӧ�Ķ��㷨�������±�
	std::vector<glm::ivec3> color_index;		// ÿ��������Ƭ�Ķ����Ӧ�Ķ�����ɫ���±�
	std::vector<glm::ivec3> texture_index;	// ÿ��������Ƭ�Ķ����Ӧ����������±�

	std::vector<glm::vec3> face_normals;	// ÿ��������Ƭ�ķ�����

	std::vector<glm::vec3> points;	// ������ɫ���Ļ��Ƶ�
	std::vector<glm::vec3> colors;	// ������ɫ������ɫ
	std::vector<glm::vec3> normals;	// ������ɫ���ķ�����
	std::vector<glm::vec2> textures;	// ������ɫ������������

	bool do_normalize_size;        // �Ƿ������С��һ��
	float diagonal_length;      // �����Χ�жԽ��߳��ȣ���Ϊ�����һ��ϵ��
	glm::vec3 up_corner;				// �����Χ�е��϶ԽǶ���
	glm::vec3 down_corner;           // �����Χ�е��¶ԽǶ���
	glm::vec3 center;                // ����İ�Χ������

	glm::vec3 translation = glm::vec3(0, 0, 0);			// �����ƽ�Ʋ���
	glm::vec3 rotation = glm::vec3(0, 0, 0);				// �������ת����
	glm::vec3 scale = glm::vec3(1, 1, 1);					// ��������Ų���
};