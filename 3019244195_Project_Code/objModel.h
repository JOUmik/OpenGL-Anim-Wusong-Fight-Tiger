#pragma once

#include <iostream>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <glfw/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "objMesh.h"
#include "shader.h"

class ObjModel
{
public:
	//模型数据
	std::vector<Texture> textures_loaded;
	std::vector<ObjMesh> meshes;
	std::string directory;
	glm::vec3 translate = glm::vec3(0, 0, 0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	GLfloat obj_rotateX = 0.0;
	GLfloat obj_rotateY = 0.0;
	GLfloat obj_rotateZ = 0.0;

	ObjModel(std::string const path);
	~ObjModel();
	void Draw(Shader* shader);

private:
	void loadModel(std::string const path);
	void processNode(aiNode* node, const aiScene* scene);
	ObjMesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
	GLuint TextureFromFile(const char* path, const std::string& directory);
};