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

#include "model.h"
#include "shader.h"
#include "camera.h"

class Mesh
{

public:
	Mesh();
	~Mesh();

	void load_texture_STBImage(const std::string& file_name, GLuint& m_texName);
	void addMesh(Model* mesh);
	void bindObjectAndData(Model* mesh, GLuint program);
	void drawMesh(Model* mesh, Shader* shader);
	void drawMeshes(Shader* shader);
private:
	std::vector<Model*> meshes;
};