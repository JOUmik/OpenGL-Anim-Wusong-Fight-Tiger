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

	// 获取和设置物体的旋转平移变化
	glm::vec3 getTranslation();
	glm::vec3 getRotation();
	glm::vec3 getScale();
	glm::mat4 getModelMatrix();

	void setTranslation(glm::vec3 translation);
	void setRotation(glm::vec3 rotation);
	void setScale(glm::vec3 scale);

	void setNormalize(bool do_norm);

	// 创建立方体、平面模型的函数
	void generateCube(glm::vec3 _color, glm::vec3 _scale);
	void generateSquare(glm::vec3 color, glm::vec3 _scale, float texScale);

	// 将读取的顶点根据三角面片上的顶点下标逐个加入
	// 要传递给GPU的points等容器内
	void storeFacesPoints();

	// 清除数据
	void cleanData();

	GLuint VAO, VBO, texture, textureType = GL_TEXTURE_2D;
	std::string texture_path;
	glm::vec3 rotatePoint = glm::vec3(0.0, 0.0, 0.0);
	void bindData();
	float reflectFactor = 0.0;
	int symbol = 0;  //symbol = 1时不对相应模型进行绘制

protected:
	Model* parent;
	glm::vec3 connect_position;

	std::vector<glm::vec3> vertex_positions;	// 顶点坐标
	std::vector<glm::vec3> vertex_colors;	// 顶点颜色
	std::vector<glm::vec3> vertex_normals;	// 顶点法向量
	std::vector<glm::vec2> vertex_textures;	// 顶点纹理坐标

	std::vector<glm::ivec3> faces;	// 三角面片上每个顶点对应的顶点坐标的下标
	std::vector<glm::ivec3> normal_index;	// 每个三角面片的顶点对应的顶点法向量的下标
	std::vector<glm::ivec3> color_index;		// 每个三角面片的顶点对应的顶点颜色的下标
	std::vector<glm::ivec3> texture_index;	// 每个三角面片的顶点对应纹理坐标的下标

	std::vector<glm::vec3> face_normals;	// 每个三角面片的法向量

	std::vector<glm::vec3> points;	// 传入着色器的绘制点
	std::vector<glm::vec3> colors;	// 传入着色器的颜色
	std::vector<glm::vec3> normals;	// 传入着色器的法向量
	std::vector<glm::vec2> textures;	// 传入着色器的纹理坐标

	bool do_normalize_size;        // 是否将物体大小归一化
	float diagonal_length;      // 物体包围盒对角线长度，作为物体归一化系数
	glm::vec3 up_corner;				// 物体包围盒的上对角顶点
	glm::vec3 down_corner;           // 物体包围盒的下对角顶点
	glm::vec3 center;                // 物体的包围盒中心

	glm::vec3 translation = glm::vec3(0, 0, 0);			// 物体的平移参数
	glm::vec3 rotation = glm::vec3(0, 0, 0);				// 物体的旋转参数
	glm::vec3 scale = glm::vec3(1, 1, 1);					// 物体的缩放参数
};