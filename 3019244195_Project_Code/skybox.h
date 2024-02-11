#pragma once
#include <iostream>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

class Skybox {
public:
    GLfloat obj_rotateX = 0.0;
    GLfloat obj_rotateY = 0.0;
    GLfloat obj_rotateZ = 0.0;


    std::vector<std::string> faces
    {
        "skybox\\right.jpg",
        "skybox\\left.jpg",
        "skybox\\top.jpg",
        "skybox\\bottom.jpg",
        "skybox\\front.jpg",
        "skybox\\back.jpg"
    };

    Skybox();
    ~Skybox();

    void Draw();

private:
    GLuint VAO, VBO;
    GLuint cubemapTexture;
    void setupMesh();
};