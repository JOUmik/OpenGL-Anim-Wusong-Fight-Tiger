#include "shader.h"

Shader::Shader()
{
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath)
{
    //���ļ�
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    try
    {
        //��Ӳ�̽��ļ���ȡ���ڴ棬תΪ�ļ���fileStream
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);

        //�ж��ļ��Ƿ�����֮������
        // vShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
        // fShaderFile.exceptions(std::ifstream::failbit || std::ifstream::badbit);
        vShaderFile.exceptions(std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::badbit);

        //����ת��ΪStringStream��StringBuffer��
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        //fileStreamʹ����ϣ��ر�
        vShaderFile.close();
        fShaderFile.close();

        //��StringStreamת��ΪString
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();

    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    //��Stringת��Ϊchar���飬��ΪCPUֻ����char���鲻����String
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();

    //������ɫ��
    GLuint vertex, fragment;

    //������ɫ�� 
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    //�жϱ����Ƿ�ɹ�
    checkCompileErrors(vertex, "VERTEX");

    //Ƭ����ɫ��
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    //�жϱ����Ƿ�ɹ�
    checkCompileErrors(fragment, "FRAGMENT");

    //����Shader���򣨽�������ɫ����Ƭ����ɫ��link��һ��
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    //�жϱ����Ƿ�ɹ�
    checkCompileErrors(this->Program, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

Shader::Shader(const GLchar* vertexPath, const GLchar* tcsPath, const GLchar* tesPath, const GLchar* geoPath, const GLchar* fragmentPath)
{
    // 1. Retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string tcsCode;
    std::string tesCode;
    std::string geoCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream tcShaderFile;
    std::ifstream teShaderFile;
    std::ifstream gShaderFile;
    std::ifstream fShaderFile;
    // ensures ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::badbit);
    tcShaderFile.exceptions(std::ifstream::badbit);
    teShaderFile.exceptions(std::ifstream::badbit);
    gShaderFile.exceptions(std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::badbit);
    try
    {
        // Open files
        vShaderFile.open(vertexPath);
        tcShaderFile.open(tcsPath);
        teShaderFile.open(tesPath);
        gShaderFile.open(geoPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, tcShaderStream, teShaderStream, fShaderStream, gShaderStream;
        // Read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        tcShaderStream << tcShaderFile.rdbuf();
        teShaderStream << teShaderFile.rdbuf();
        gShaderStream << gShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        tcShaderFile.close();
        teShaderFile.close();
        gShaderFile.close();
        fShaderFile.close();
        // Convert stream into string
        vertexCode = vShaderStream.str();
        tcsCode = tcShaderStream.str();
        tesCode = teShaderStream.str();
        geoCode = gShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const GLchar* vShaderCode = vertexCode.c_str();
    const GLchar* tcShaderCode = tcsCode.c_str();
    const GLchar* teShaderCode = tesCode.c_str();
    const GLchar* gShaderCode = geoCode.c_str();
    const GLchar* fShaderCode = fragmentCode.c_str();
    // 2. Compile shaders
    GLuint vertex, tcs, tes, geo, fragment;
    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    // TCS Shader
    tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tcs, 1, &tcShaderCode, NULL);
    glCompileShader(tcs);
    checkCompileErrors(tcs, "TESS CONTROL");
    // TES Shader
    tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tes, 1, &teShaderCode, NULL);
    glCompileShader(tes);
    checkCompileErrors(tes, "TESS EVALUATION");
    // GEO Shader
    geo = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geo, 1, &gShaderCode, NULL);
    glCompileShader(geo);
    checkCompileErrors(geo, "GEOMETRY");
    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    // Shader Program
    this->Program = glCreateProgram();
    glAttachShader(this->Program, vertex);
    glAttachShader(this->Program, tcs);
    glAttachShader(this->Program, tes);
    glAttachShader(this->Program, geo);
    glAttachShader(this->Program, fragment);
    glLinkProgram(this->Program);
    // Print linking errors if any
    checkCompileErrors(this->Program, "PROGRAM");
    // Delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertex);
    glDeleteShader(tcs);
    glDeleteShader(tes);
    glDeleteShader(geo);
    glDeleteShader(fragment);
}

void Shader::Use()
{
    glUseProgram(this->Program);
}

void Shader::checkCompileErrors(GLuint ID, std::string type)
{
    GLint success;
    GLchar infoLog[512];

    if (type != "PROGRAM")
    {
        glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(ID, 512, NULL, infoLog);
            std::cout << "shader compile error: " << infoLog << std::endl;
        }
    }
    else
    {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(ID, 512, NULL, infoLog);
            std::cout << "program linking error: " << infoLog << std::endl;
        }
    }
}