#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

//glad
#include <glad/glad.h>

//glfw
#include <glfw/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"
#include "stb_image.h"
#include "objModel.h"      //read obj model
#include "shader.h"        //shader
#include "camera.h"        //camera
#include "skybox.h"        //skybox
#include "wuSong.h"        //hierarchical modeling of Wu Song
#include "tiger.h"         //hierarchical modeling of tiger

//function declaration
void changeWithBezier();
double getAngle(glm::vec3 direction);                                                                                                      //get vector angle
void repeatPaintTree1(ObjModel* tree1, Shader* shader);
void repeatPaintTree2(ObjModel* tree1, Shader* shader);
void repeatPaintTree3(ObjModel* tree1, Shader* shader);
void repeatPaintStone1(ObjModel* tree1, Shader* shader);
void repeatPaintStone2(ObjModel* tree1, Shader* shader);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);                                                        //keyboard response
void processInputs(GLFWwindow* window);                                                                                                    //continuous response


GLuint program; // Shader Procedure Object  

glm::vec3 cameraPosition(-0.98, 5.64, -17.0f);      // camera position
glm::vec3 cameraDirection(2, -1, 2);    // camera direction
glm::vec3 cameraUp(0, 1, 0);            // vertical up vector in world space

Mesh* painter = new Mesh();

Camera camera(cameraPosition, cameraUp);

//parameters used by the director
GLfloat time_1 = 0;  //Time used in Preparation 1
GLfloat time_2 = 0;  //Time used in Preparation 2
GLfloat time1 = 0;   //Time used in Act 1
GLfloat time2 = 0;   //Time used in Act 2
GLfloat time3 = 0;   //Time used in Act 3
GLfloat time4 = 0;   //Time used in Act 4
GLfloat time5 = 0;   //Time used in Act 5

/*
*
* The index indicates the act of the performance
* Index=0: Preparation 1: Camera rotation
* Index=1: Preparation 2: Camera movement
* Index=2: Act 1: Wu Song and Tiger get close to each other
* index=3: Act 2: Wu Song Beats the Tiger with a Stick
* Index=4: Act 3: Wu Song and Tiger Leave Each Other
* Index=5: Act 4: Wu Song and Tiger get close to each other again
* Index=6: Act 5: Wu Song fights the tiger to win
*/
GLint index = 0;

//Wu Song
WuSong* wuSong = new WuSong();
GLfloat wuSongRotate = 20.0;
Model*& WuSongCore = wuSong->parts["core"];

//Tiger
Tiger* tiger = new Tiger();
GLfloat tigerRotate = 210.0;
Model*& TigerCore = tiger->parts["core"];

//illuminant 
GLfloat light_x = -6.0, light_y = 12.0, light_z = -10.0, light_scale = 0.4;

//deltatime
GLfloat deltaTime = 0.0f;    //time between current frame and last frame
GLfloat lastFrame = 0.0f;      //time of last frame

//view volume parameters
GLfloat left = -1, right = 1, bottom = -1, top = 1, zNear = 0.1, zFar = 150.0;

//viewport
const GLuint WIDTH = 1000, HEIGHT = 800;

//Mouse operation
bool l_firstMouse = true, r_firstMouse = true;
GLfloat lastX, lastY;

//Parameters required for shadow mapping
const GLuint SHADOW_WIDTH = 4096, SHADOW_HEIGHT = 4096;
unsigned int depthMapFBO;
unsigned int depthMap;
GLboolean shadows = true;
GLfloat near_plane = 0.01f, far_plane = 30.0f;

//Operation object
int object = 1;
float speed = 1.0;

//Location coordinate of trees
const std::vector<vec2> treeBias1 = {
    vec2(2.5, -6),vec2(7, -8),vec2(0,0),vec2(-9,3.5), vec2(6,4)
};

const std::vector<vec2> treeBias2 = {
    vec2(-4.5, -8),vec2(5, -7),vec2(-2,-6),vec2(7,-6.5), vec2(7,5), vec2(-10, -10), vec2(-7, -6.5)
};

const std::vector<vec2> treeBias3 = {
    vec2(-6, -3.5),vec2(-10.5, -17.5),vec2(-1.5, -4)
};

const std::vector<vec2> stoneBias1 = {
    vec2(-5.5, -2.5), vec2(6.5, 8)
};

const std::vector<vec2> stoneBias2 = {
    vec2(0.7, 3.4),vec2(-8, -9),vec2(2,-5)
};

//Bezier curve for control
glm::vec3 cameraControl1[] = {
        glm::vec3{-0.98, 5.64, -17.0},
        glm::vec3{-9.0, 5.64, -18.0},
        glm::vec3{-16.0, 5.64, -10.0},
        glm::vec3{-14.35, 5.64 ,-4.7}
};

glm::vec3 cameraControl2[] = {
        glm::vec3{-14.35, 5.64 ,-4.7},
        glm::vec3{-18.0, 5.44, 2.0},
        glm::vec3{-10.0, 5.2, 16.0},
        glm::vec3{-7.23, 4.94 ,14.67}
};

glm::vec3 cameraControl3[] = {
        glm::vec3{-7.23, 4.94 ,14.67},
        glm::vec3{-8.0, 4.07, 12.0},
        glm::vec3{-4.0, 3.2, 10.0},
        glm::vec3{-1.70, 2.32 ,8.26}
};

glm::vec3 wuSongVertices1[] = {
        glm::vec3{-8, 0, -4},
        glm::vec3{-9.0, 0, -2.5},
        glm::vec3{-7.0, 0, -0.5},
        glm::vec3{-5.6, 0 ,0}
};

glm::vec3 tigerVertices1[] = {
        glm::vec3{-2, 0, 4},
        glm::vec3{-2.2, 0, 3},
        glm::vec3{-3.4, 0, 1},
        glm::vec3{-4.6, 0 ,0}
};

glm::vec3 wuSongVertices2[] = {
        glm::vec3{-5.2, 0 ,0},
        glm::vec3{-7, 0, 2},
        glm::vec3{-6.5, 0, 5},
        glm::vec3{-3, 0 ,6}
};

glm::vec3 tigerVertices2[] = {
        glm::vec3{-4.6, 0 ,0},
        glm::vec3{-6, 0, -2},
        glm::vec3{2, 0, -4},
        glm::vec3{4, 0 ,-2}
};

glm::vec3 wuSongVertices3[] = {
        glm::vec3{-3, 0 ,6},
        glm::vec3{4.5, 0, -1},
        glm::vec3{6, 0, 2.5},
        glm::vec3{5.7, 0 ,3.2}
};

glm::vec3 tigerVertices3[] = {
        glm::vec3{4, 0 ,-2},
        glm::vec3{0, 0, 14},
        glm::vec3{5.8, 0, 5},
        glm::vec3{5.0, 0 ,4.0}
};

// --------------- end of global variable definition --------------- //

// Read the file and return a long string to indicate the file content
std::string readShaderFile(std::string filepath)
{
    std::string res, line;
    std::ifstream fin(filepath);
    if (!fin.is_open())
    {
        std::cout << "文件 " << filepath << " 打开失败" << std::endl;
        exit(-1);
    }
    while (std::getline(fin, line))
    {
        res += line + '\n';
    }
    fin.close();
    return res;
}


// -------------------------------- main -------------------------------- //

int main(int argc, char** argv)
{
    //glfw initialization and configuration
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);    //one more step configuration for Apple users
#endif

    //create a window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL Project: Wu Song Da Hu", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //glad: load all OpenGL function pointers
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    //Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    //open depth test
    glEnable(GL_DEPTH_TEST);
    // Enable Blending
    //glEnable(GL_BLEND);

    //Generate shader program object
    Shader skyboxShader("skybox.vert.glsl", "skybox.frag.glsl");
    Shader shader("shadow_mapping.vert.glsl", "shadow_mapping.frag.glsl");
    Shader simpleDepthShader("shadow_mapping_depth.vert.glsl", "shadow_mapping_depth.frag.glsl");


    wuSong->parts["core"]->setTranslation(vec3(-8, 0, -4));
    wuSong->parts["core"]->setRotation(vec3(0, wuSongRotate, 0));

    tiger->parts["core"]->setTranslation(vec3(-2, 0, 4));
    tiger->parts["core"]->setRotation(vec3(0, tigerRotate, 0));

    camera.WIDTH = WIDTH;
    camera.HEIGHT = HEIGHT;
    camera.zNear = zNear;
    camera.zFar = zFar;


    //initialization of used models
    ObjModel tree1("models\\tree1\\tree1.obj");
    ObjModel tree2("models\\tree2\\tree2.obj");
    ObjModel tree3("models\\tree3\\tree3.obj");
    ObjModel stone1("models\\stone1\\stone1.obj");
    ObjModel stone2("models\\stone2\\stone2.obj");
    tree1.scale = glm::vec3(2.5, 2.5, 2.5);
    tree2.scale = glm::vec3(3.5, 3.5, 3.5);
    tree3.scale = glm::vec3(4.0, 4.0, 4.0);
    stone1.scale = glm::vec3(0.8, 0.8, 0.8);
    stone2.scale = glm::vec3(0.6, 0.6, 0.6);

    Skybox* skybox = new Skybox();

    // Bind Wu Song
    for (auto it = wuSong->parts.begin(); it != wuSong->parts.end(); it++)
    {
        painter->addMesh(it->second);
    }

    // Bind Tiger
    for (auto it = tiger->parts.begin(); it != tiger->parts.end(); it++)
    {
        painter->addMesh(it->second);
    }

    //ground

    Model* plane = new Model();
    plane->setNormalize(false);
    plane->setTranslation(vec3(0, 0, 0));
    plane->setRotation(vec3(-90, 0, 0));
    plane->generateSquare(Red, vec3(20, 20, 20), 10);
    plane->texture_path = "assets/grass.jpg";
    plane->reflectFactor = 0.0;
    painter->addMesh(plane);

    glGenFramebuffers(1, &depthMapFBO);    // Create Frame Buffer
    // Create the depth texture
    glGenTextures(1, &depthMap);    // Create Texture
    glBindTexture(GL_TEXTURE_2D, depthMap);    // Generate depth texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);    // Configure Textures
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    // depth texture attached to frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);      // Bind Frame Buffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    shader.Use();
    glUniform1i(glGetUniformLocation(shader.Program, "diffuseTexture"), 0);
    glUniform1i(glGetUniformLocation(shader.Program, "shadowMap"), 1);

    //debugDepthQuad.Use();
    //glUniform1i(glGetUniformLocation(debugDepthQuad.Program, "depthMap"), 0);

    //Set the required callback functions
    glfwSetKeyCallback(window, key_callback);

    //enter cycle
    while (!glfwWindowShouldClose(window))
    {
        std::vector<ObjModel> models;

        //Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        processInputs(window);

        if (deltaTime < 1.0) {
            changeWithBezier();
        }

        // Update Model Actions
        wuSong->playMotion();
        tiger->playMotion();

        GLuint vlocation;
        GLuint plocation;
        glm::vec3 lightPos(light_x, light_y, light_z);

        //Render
        //Clear the colorbuffer
        glClearColor(0.486f, 0.804f, 0.486f, 0.7f);
        glClearDepth(2.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 1. render depth of scene to texture (from light's perspective)
        // --------------------------------------------------------------
        glm::mat4 lightProjection, lightView;
        glm::mat4 lightSpaceMatrix;
        lightProjection = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, near_plane, far_plane);
        lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
        lightSpaceMatrix = lightProjection * lightView;
        // render scene from light's point of view
        simpleDepthShader.Use();
        glUniformMatrix4fv(glGetUniformLocation(simpleDepthShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        painter->drawMeshes(&simpleDepthShader);
        repeatPaintTree1(&tree1, &simpleDepthShader);
        repeatPaintTree2(&tree2, &simpleDepthShader);
        repeatPaintTree3(&tree3, &simpleDepthShader);
        repeatPaintStone1(&stone1, &simpleDepthShader);
        repeatPaintStone2(&stone2, &simpleDepthShader);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        // 2. render scene as normal using the generated depth/shadow map  
        // --------------------------------------------------------------
        glViewport(0, 0, WIDTH, HEIGHT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.Use();
        //view matrix
        glm::mat4 view = camera.GetViewMatrix();
        //projection matrix
        glm::mat4 projection = glm::perspective(glm::radians(camera.Fov), (GLfloat)WIDTH / (GLfloat)HEIGHT, zNear, zFar);

        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
        // set light uniforms
        glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, glm::value_ptr(camera.Position));
        glUniform3fv(glGetUniformLocation(shader.Program, "lightPos"), 1, glm::value_ptr(lightPos));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
        glUniform1i(glGetUniformLocation(shader.Program, "shadows"), shadows);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        painter->drawMeshes(&shader);
        repeatPaintTree1(&tree1, &shader);
        repeatPaintTree2(&tree2, &shader);
        repeatPaintTree3(&tree3, &shader);
        repeatPaintStone1(&stone1, &shader);
        repeatPaintStone2(&stone2, &shader);

        /*---------------------------skybox--------------------------*/
        glDepthFunc(GL_LEQUAL);
        skyboxShader.Use();

        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));

        //transmit view matrix
        vlocation = glGetUniformLocation(skyboxShader.Program, "view");
        glUniformMatrix4fv(vlocation, 1, GL_FALSE, glm::value_ptr(view));

        //transmit projection matrix
        plocation = glGetUniformLocation(skyboxShader.Program, "projection");
        glUniformMatrix4fv(plocation, 1, GL_FALSE, glm::value_ptr(projection));

        skybox->Draw();

        glDepthFunc(GL_LESS);

        //Swap the screen buffers
        glfwSwapBuffers(window);
    }
    //Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void changeWithBezier() {
    /*---------------------------------------------Preparation 1: Camera rotation------------------------------------------*/
    if (index == 0)
    {
        if (time_1 >= (1 - deltaTime * (1 / 4.0)))
        {
            index++;
            return;
        }

        time_1 += (deltaTime * (1 / 4.0));

        camera.Yaw += (deltaTime * (1 / 4.0)) * 75.2;

        camera.UpdateCameraVectors();
    }

    /*---------------------------------------------Preparation 2: Camera movement------------------------------------------*/

    if (index == 1)
    {
        if (time_2 >= (1 - deltaTime * (1 / 4.5)))
        {
            index++;
            return;
        }

        time_2 += (deltaTime * (1 / 4.5));

        GLfloat b0 = (1.0 - time_2 + (deltaTime * (1 / 4.5))) * (1.0 - time_2 + (deltaTime * (1 / 4.5))) * (1.0 - time_2 + (deltaTime * (1 / 4.5)));
        GLfloat b1 = 3.0 * (time_2 - (deltaTime * (1 / 4.5))) * (1.0 - time_2 + (deltaTime * (1 / 4.5))) * (1.0 - time_2 + (deltaTime * (1 / 4.5)));
        GLfloat b2 = 3.0 * (time_2 - (deltaTime * (1 / 4.5))) * (time_2 - (deltaTime * (1 / 4.5))) * (1.0 - time_2 + (deltaTime * (1 / 4.5)));
        GLfloat b3 = (time_2 - (deltaTime * (1 / 4.5))) * (time_2 - (deltaTime * (1 / 4.5))) * (time_2 - (deltaTime * (1 / 4.5)));

        glm::vec3 camera_position_past = b0 * cameraControl1[0] + b1 * cameraControl1[1] + b2 * cameraControl1[2] + b3 * cameraControl1[3];

        b0 = (1.0 - time_2) * (1.0 - time_2) * (1.0 - time_2);
        b1 = 3.0 * time_2 * (1.0 - time_2) * (1.0 - time_2);
        b2 = 3.0 * time_2 * time_2 * (1.0 - time_2);
        b3 = time_2 * time_2 * time_2;

        glm::vec3 camera_position_current = b0 * cameraControl1[0] + b1 * cameraControl1[1] + b2 * cameraControl1[2] + b3 * cameraControl1[3];

        glm::vec3 Direction = camera_position_current - camera_position_past;

        camera.Position += Direction;

        camera.Yaw += (deltaTime * (1 / 4.5)) * (-105.2);
        camera.Pitch += (deltaTime * (1 / 4.5)) * (-10.0);

        camera.UpdateCameraVectors();
    }

    /*---------------------------------------------Act 1: Wu Song and Tiger get close to each other------------------------------------------*/
    if (index == 2)
    {
        if (time1 >= (1 - deltaTime * (1 / 3.0)))
        {
            wuSong->endCycle("stay");
            tiger->endCycle("stay");
            index++;
            return;
        }

        if (time1 == 0)
        {
            wuSong->startCycle({ "run1", "run2" });
            tiger->startCycle({ "run1", "run2" });
        }

        time1 += (deltaTime * (1 / 3.0));

        GLfloat b0 = (1.0 - time1 + (deltaTime * (1 / 3.0))) * (1.0 - time1 + (deltaTime * (1 / 3.0))) * (1.0 - time1 + (deltaTime * (1 / 3.0)));
        GLfloat b1 = 3.0 * (time1 - (deltaTime * (1 / 3.0))) * (1.0 - time1 + (deltaTime * (1 / 3.0))) * (1.0 - time1 + (deltaTime * (1 / 3.0)));
        GLfloat b2 = 3.0 * (time1 - (deltaTime * (1 / 3.0))) * (time1 - (deltaTime * (1 / 3.0))) * (1.0 - time1 + (deltaTime * (1 / 3.0)));
        GLfloat b3 = (time1 - (deltaTime * (1 / 3.0))) * (time1 - (deltaTime * (1 / 3.0))) * (time1 - (deltaTime * (1 / 3.0)));

        glm::vec3 wuSong_position_past = b0 * wuSongVertices1[0] + b1 * wuSongVertices1[1] + b2 * wuSongVertices1[2] + b3 * wuSongVertices1[3];
        glm::vec3 tiger_position_past = b0 * tigerVertices1[0] + b1 * tigerVertices1[1] + b2 * tigerVertices1[2] + b3 * tigerVertices1[3];

        b0 = (1.0 - time1) * (1.0 - time1) * (1.0 - time1);
        b1 = 3.0 * time1 * (1.0 - time1) * (1.0 - time1);
        b2 = 3.0 * time1 * time1 * (1.0 - time1);
        b3 = time1 * time1 * time1;

        glm::vec3 wuSong_position_current = b0 * wuSongVertices1[0] + b1 * wuSongVertices1[1] + b2 * wuSongVertices1[2] + b3 * wuSongVertices1[3];
        glm::vec3 tiger_position_current = b0 * tigerVertices1[0] + b1 * tigerVertices1[1] + b2 * tigerVertices1[2] + b3 * tigerVertices1[3];

        glm::vec3 wuSongDirection = wuSong_position_current - wuSong_position_past;
        glm::vec3 tigerDirection = tiger_position_current - tiger_position_past;

        WuSongCore->setTranslation(WuSongCore->getTranslation() + wuSongDirection);
        TigerCore->setTranslation(TigerCore->getTranslation() + tigerDirection);

        wuSong->parts["core"]->setRotation(vec3(0, getAngle(wuSongDirection), 0));
        tiger->parts["core"]->setRotation(vec3(0, getAngle(tigerDirection), 0));
    }

    /*---------------------------------------------Act 2: Wu Song Beats the Tiger with a Stick------------------------------------------*/
    if (index == 3)
    {
        if (time2 >= (1 - deltaTime * (1 / 8.0)))
        {
            wuSong->endCycle("stay");
            index++;
            wuSong->parts["weapon"]->symbol = 1;  //When symbol is 1, it will not be drawn, which means Wu Song breaks the stick when he fights the tiger
            return;
        }

        if (time2 == 0)
        {
            wuSong->startCycle({ "attack", "stay" });
        }

        time2 += (deltaTime * (1 / 8.0));

        if (time2 < 0.4)  camera.Fov += 2.5 * (deltaTime * (1 / 8.0)) * (-20);
        else if (time2 > 0.6)camera.Fov += 2.5 * (deltaTime * (1 / 8.0)) * 20;

        //std::cout << camera.Fov << std::endl;
        camera.UpdateCameraVectors();
    }

    /*---------------------------------------------Act 3: Wu Song and Tiger Leave Each Other------------------------------------------*/
    if (index == 4)
    {
        if (time3 >= (1 - deltaTime * (1 / 4.5)))
        {
            wuSong->endCycle("stay");
            tiger->endCycle("stay");
            index++;
            return;
        }

        if (time3 == 0)
        {
            wuSong->startCycle({ "run1", "run2" });
            tiger->startCycle({ "run1", "run2" });
        }

        time3 += (deltaTime * (1 / 4.5));

        GLfloat b0 = (1.0 - time3 + (deltaTime * (1 / 4.5))) * (1.0 - time3 + (deltaTime * (1 / 4.5))) * (1.0 - time3 + (deltaTime * (1 / 4.5)));
        GLfloat b1 = 3.0 * (time3 - (deltaTime * (1 / 4.5))) * (1.0 - time3 + (deltaTime * (1 / 4.5))) * (1.0 - time3 + (deltaTime * (1 / 4.5)));
        GLfloat b2 = 3.0 * (time3 - (deltaTime * (1 / 4.5))) * (time3 - (deltaTime * (1 / 4.5))) * (1.0 - time3 + (deltaTime * (1 / 4.5)));
        GLfloat b3 = (time3 - (deltaTime * (1 / 4.5))) * (time3 - (deltaTime * (1 / 4.5))) * (time3 - (deltaTime * (1 / 4.5)));

        glm::vec3 wuSong_position_past = b0 * wuSongVertices2[0] + b1 * wuSongVertices2[1] + b2 * wuSongVertices2[2] + b3 * wuSongVertices2[3];
        glm::vec3 tiger_position_past = b0 * tigerVertices2[0] + b1 * tigerVertices2[1] + b2 * tigerVertices2[2] + b3 * tigerVertices2[3];
        glm::vec3 camera_position_past = b0 * cameraControl2[0] + b1 * cameraControl2[1] + b2 * cameraControl2[2] + b3 * cameraControl2[3];

        b0 = (1.0 - time3) * (1.0 - time3) * (1.0 - time3);
        b1 = 3.0 * time3 * (1.0 - time3) * (1.0 - time3);
        b2 = 3.0 * time3 * time3 * (1.0 - time3);
        b3 = time3 * time3 * time3;

        glm::vec3 wuSong_position_current = b0 * wuSongVertices2[0] + b1 * wuSongVertices2[1] + b2 * wuSongVertices2[2] + b3 * wuSongVertices2[3];
        glm::vec3 tiger_position_current = b0 * tigerVertices2[0] + b1 * tigerVertices2[1] + b2 * tigerVertices2[2] + b3 * tigerVertices2[3];
        glm::vec3 camera_position_current = b0 * cameraControl2[0] + b1 * cameraControl2[1] + b2 * cameraControl2[2] + b3 * cameraControl2[3];

        glm::vec3 wuSongDirection = wuSong_position_current - wuSong_position_past;
        glm::vec3 tigerDirection = tiger_position_current - tiger_position_past;
        glm::vec3 Direction = camera_position_current - camera_position_past;


        camera.Yaw += (deltaTime * (1 / 4.5)) * (-76.0);
        camera.Pitch += (deltaTime * (1 / 4.5)) * (15.6);
        camera.Position += Direction;

        camera.UpdateCameraVectors();

        WuSongCore->setTranslation(WuSongCore->getTranslation() + wuSongDirection);
        TigerCore->setTranslation(TigerCore->getTranslation() + tigerDirection);

        wuSong->parts["core"]->setRotation(vec3(0, getAngle(wuSongDirection), 0));
        tiger->parts["core"]->setRotation(vec3(0, getAngle(tigerDirection), 0));
    }

    /*---------------------------------------------Act 4: Wu Song and Tiger Get Close Again------------------------------------------*/
    if (index == 5)
    {
        if (time4 >= (1 - deltaTime * (1 / 5.5)))
        {
            wuSong->endCycle("stay");
            tiger->endCycle("stay");
            index++;
            return;
        }

        if (time4 == 0)
        {
            wuSong->startCycle({ "run1", "run2" });
            tiger->startCycle({ "run1", "run2" });
        }

        time4 += (deltaTime * (1 / 5.5));

        GLfloat b0 = (1.0 - time4 + (deltaTime * (1 / 5.5))) * (1.0 - time4 + (deltaTime * (1 / 5.5))) * (1.0 - time4 + (deltaTime * (1 / 5.5)));
        GLfloat b1 = 3.0 * (time4 - (deltaTime * (1 / 5.5))) * (1.0 - time4 + (deltaTime * (1 / 5.5))) * (1.0 - time4 + (deltaTime * (1 / 5.5)));
        GLfloat b2 = 3.0 * (time4 - (deltaTime * (1 / 5.5))) * (time4 - (deltaTime * (1 / 5.5))) * (1.0 - time4 + (deltaTime * (1 / 5.5)));
        GLfloat b3 = (time4 - (deltaTime * (1 / 5.5))) * (time4 - (deltaTime * (1 / 5.5))) * (time4 - (deltaTime * (1 / 5.5)));

        glm::vec3 wuSong_position_past = b0 * wuSongVertices3[0] + b1 * wuSongVertices3[1] + b2 * wuSongVertices3[2] + b3 * wuSongVertices3[3];
        glm::vec3 tiger_position_past = b0 * tigerVertices3[0] + b1 * tigerVertices3[1] + b2 * tigerVertices3[2] + b3 * tigerVertices3[3];
        glm::vec3 camera_position_past = b0 * cameraControl3[0] + b1 * cameraControl3[1] + b2 * cameraControl3[2] + b3 * cameraControl3[3];

        b0 = (1.0 - time4) * (1.0 - time4) * (1.0 - time4);
        b1 = 3.0 * time4 * (1.0 - time4) * (1.0 - time4);
        b2 = 3.0 * time4 * time4 * (1.0 - time4);
        b3 = time4 * time4 * time4;

        glm::vec3 wuSong_position_current = b0 * wuSongVertices3[0] + b1 * wuSongVertices3[1] + b2 * wuSongVertices3[2] + b3 * wuSongVertices3[3];
        glm::vec3 tiger_position_current = b0 * tigerVertices3[0] + b1 * tigerVertices3[1] + b2 * tigerVertices3[2] + b3 * tigerVertices3[3];
        glm::vec3 camera_position_current = b0 * cameraControl3[0] + b1 * cameraControl3[1] + b2 * cameraControl3[2] + b3 * cameraControl3[3];

        glm::vec3 wuSongDirection = wuSong_position_current - wuSong_position_past;
        glm::vec3 tigerDirection = tiger_position_current - tiger_position_past;
        glm::vec3 Direction = camera_position_current - camera_position_past;


        camera.Yaw += (deltaTime * (1 / 5.5)) * (23.16);
        camera.Pitch += (deltaTime * (1 / 5.5)) * (5.6);
        camera.Position += Direction;

        camera.UpdateCameraVectors();

        WuSongCore->setTranslation(WuSongCore->getTranslation() + wuSongDirection);
        TigerCore->setTranslation(TigerCore->getTranslation() + tigerDirection);

        wuSong->parts["core"]->setRotation(vec3(0, getAngle(wuSongDirection), 0));
        tiger->parts["core"]->setRotation(vec3(0, getAngle(tigerDirection), 0));
    }

    /*---------------------------------------------Act 5: Wu Song fights the tiger and wins------------------------------------------*/
    if (index == 6)
    {
        if (time5 >= (1 - deltaTime * (1 / 4.0)))
        {
            wuSong->endCycle("stay");
            tiger->parts["core"]->setRotation(vec3(0, tiger->parts["core"]->getRotation().y, -90));
            tiger->parts["core"]->setTranslation(tiger->parts["core"]->getTranslation() + glm::vec3(0, 0.3, 0));
            index++;
            return;
        }

        if (time5 == 0)
        {
            wuSong->startCycle({ "attack", "stay" });
        }

        time5 += (deltaTime * (1 / 4.0));

        camera.Fov += (deltaTime * (1 / 4.0)) * (-18);
        camera.UpdateCameraVectors();
    }
}

double getAngle(glm::vec3 direction)
{
    glm::vec3 init(0, 0, 1);
    glm::vec3 cross(init.y * direction.z - init.z * direction.y, init.z * direction.x - init.x * direction.z, init.x * direction.y - init.y * direction.x);
    double radian_angle = atan2(sqrt(cross.x * cross.x + cross.y * cross.y + cross.z * cross.z), init.x * direction.x + init.y * direction.y + init.z * direction.z);
    if (cross.y < 0) {
        radian_angle = 2 * M_PI - radian_angle;
    }

    return radian_angle * 180 / M_PI;
}

void repeatPaintTree1(ObjModel* tree1, Shader* shader)
{
    for (int i = 0; i < treeBias1.size(); i++)
    {
        tree1->translate = glm::vec3(treeBias1[i].x, 0, treeBias1[i].y);
        //treePainter->getMeshes()[0]->setRotation(vec3(0, i * 60, 0));
        tree1->obj_rotateY = i * 30.0;
        tree1->Draw(shader);
    }
}

void repeatPaintTree2(ObjModel* tree1, Shader* shader)
{
    for (int i = 0; i < treeBias2.size(); i++)
    {
        tree1->translate = glm::vec3(treeBias2[i].x, 0, treeBias2[i].y);
        //tree1->obj_rotateY = i * 30.0;
        tree1->Draw(shader);
    }
}

void repeatPaintTree3(ObjModel* tree3, Shader* shader)
{
    for (int i = 0; i < treeBias3.size(); i++)
    {
        tree3->translate = glm::vec3(treeBias3[i].x, 0, treeBias3[i].y);
        tree3->obj_rotateY = i * 30.0;
        tree3->Draw(shader);
    }
}

void repeatPaintStone1(ObjModel* stone1, Shader* shader)
{
    for (int i = 0; i < stoneBias1.size(); i++)
    {
        stone1->translate = glm::vec3(stoneBias1[i].x, 0, stoneBias1[i].y);
        stone1->obj_rotateY = i * 30.0;
        stone1->Draw(shader);
    }
}

void repeatPaintStone2(ObjModel* stone2, Shader* shader)
{
    for (int i = 0; i < stoneBias2.size(); i++)
    {
        stone2->translate = glm::vec3(stoneBias2[i].x, 0, stoneBias2[i].y);
        stone2->obj_rotateY = i * 40.0;
        stone2->Draw(shader);
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        shadows == true ? shadows = false : shadows = true;

    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        wuSong->parts["weapon"]->symbol = 0;

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        wuSong->parts["weapon"]->symbol = 1;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        object = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        object = 2;
    }
}

void processInputs(GLFWwindow* window)
{
    //Camera controls
    GLfloat cameraSpeed = 5.0f * deltaTime;
    //Camera movement
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    //Object movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        {
            if (object == 1) WuSongCore->setTranslation(WuSongCore->getTranslation() + vec3(0, 0, 0.1));
            if (object == 2) TigerCore->setTranslation(TigerCore->getTranslation() + vec3(0, 0, 0.1));
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        {
            if (object == 1) WuSongCore->setTranslation(WuSongCore->getTranslation() + vec3(0, 0, -0.1));
            if (object == 2) TigerCore->setTranslation(TigerCore->getTranslation() + vec3(0, 0, -0.1));
        }
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            if (object == 1) WuSongCore->setTranslation(WuSongCore->getTranslation() + vec3(-0.1, 0, 0));
            if (object == 2) TigerCore->setTranslation(TigerCore->getTranslation() + vec3(-0.1, 0, 0));
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            if (object == 1) WuSongCore->setTranslation(WuSongCore->getTranslation() + vec3(0.1, 0, 0));
            if (object == 2) TigerCore->setTranslation(TigerCore->getTranslation() + vec3(0.1, 0, 0));
        }
    }

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)   //counterclockwise
    {
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)   //clockwise
    {
    }

    //Left mouse button (fov changes)
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        if (l_firstMouse) // The bool variable is initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            l_firstMouse = false;
        }
        else {
            double deltaX = xpos - lastX;
            double deltaY = ypos - lastY;

            lastX = xpos;
            lastY = ypos;

            camera.ProcessLeftMouseMovement(deltaX, deltaY);
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
        l_firstMouse = true;
    }

    //Right mouse button (viewpoint change) (adjust yaw and pitch)
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        double xpos, ypos;

        glfwGetCursorPos(window, &xpos, &ypos);

        if (r_firstMouse) // The bool variable is initially set to true
        {
            lastX = xpos;
            lastY = ypos;
            r_firstMouse = false;
        }
        else {
            double deltaX = xpos - lastX;
            double deltaY = ypos - lastY;

            lastX = xpos;
            lastY = ypos;

            camera.ProcessRightMouseMovement(deltaX, deltaY);
        }
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_RELEASE)
    {
        r_firstMouse = true;
        return;
    }
}