#include "objModel.h"
#include "stb_image.h"

ObjModel::ObjModel(std::string const path)
{
    loadModel(path);
}

ObjModel::~ObjModel()
{
}

void ObjModel::Draw(Shader* shader)
{

    // ��ģ�;���
    glm::mat4 model(    // ��λ����
        glm::vec4(1, 0, 0, 0),
        glm::vec4(0, 1, 0, 0),
        glm::vec4(0, 0, 1, 0),
        glm::vec4(0, 0, 0, 1)
    );

    model = glm::translate(model, this->translate);                       //λ��
    model = glm::rotate(model, glm::radians(this->obj_rotateZ), glm::vec3(0, 0, 1));    //��Z����ת
    model = glm::rotate(model, glm::radians(this->obj_rotateY), glm::vec3(0, 1, 0));    //��Y����ת
    model = glm::rotate(model, glm::radians(this->obj_rotateX), glm::vec3(1, 0, 0));    //��X����ת
    model = glm::scale(model, this->scale);                                             //����

    // ģ�ͱ任����
    GLuint mlocation = glGetUniformLocation(shader->Program, "model");    // ��Ϊmodel��uniform������λ������
    glUniformMatrix4fv(mlocation, 1, GL_FALSE, glm::value_ptr(model));   // �����Ⱦ���


    for (unsigned int i = 0; i < meshes.size(); i++)
    {
        meshes[i].Draw(shader);
    }
}

void ObjModel::loadModel(std::string const path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    // �쳣����
    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "��ȡģ�ͳ��ִ���(Assimp): " << importer.GetErrorString() << std::endl;
        exit(-1);
    }
    // ģ���ļ����·��
    directory = path.substr(0, path.find_last_of('\\'));
    //std::cout << directory << std::endl;

    processNode(scene->mRootNode, scene);
}

//��Obj�ļ��е�mess�����ȡ��������
void ObjModel::processNode(aiNode* node, const aiScene* scene)
{
    //std::cout << node->mName.data << std::endl;

    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* imgMesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(imgMesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

ObjMesh ObjModel::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> tempVertices;
    std::vector<GLuint> tempIndices;
    std::vector<Texture> tempTextures;

    //��Vertexs
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex tempVertex;

        //�涥��
        tempVertex.Position.x = mesh->mVertices[i].x;
        tempVertex.Position.y = mesh->mVertices[i].y;
        tempVertex.Position.z = mesh->mVertices[i].z;

        //�淨��
        tempVertex.Normal.x = mesh->mNormals[i].x;
        tempVertex.Normal.y = mesh->mNormals[i].y;
        tempVertex.Normal.z = mesh->mNormals[i].z;

        //����������
        if (mesh->mTextureCoords[0])
        {
            tempVertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
            tempVertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            tempVertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        tempVertices.push_back(tempVertex);
    }

    //��Indices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; j++)
        {
            tempIndices.push_back(mesh->mFaces[i].mIndices[j]);
        }
    }

    //��Textures
    // ����в��ʣ���ô���ݲ���
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        tempTextures.insert(tempTextures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //std::cout << "diffuse_finish" << std::endl;
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        tempTextures.insert(tempTextures.end(), specularMaps.begin(), specularMaps.end());
        //std::cout << "specular_finish" << std::endl;
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        tempTextures.insert(tempTextures.end(), normalMaps.begin(), normalMaps.end());
        //std::cout << "normal_finish" << std::endl;
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        tempTextures.insert(tempTextures.end(), heightMaps.begin(), heightMaps.end());
        //std::cout << "height_finish" << std::endl;
    }

    return ObjMesh(tempVertices, tempIndices, tempTextures);
}

std::vector<Texture> ObjModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        //std::cout << str.C_Str() << std::endl;

        //��������Ƿ���ع���������ع�ֱ���ã����ٶ�ȡ����
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true; //ͬ���������Ѿ����ع��ˣ���������һ������
                break;
            }
        }

        //�������û�м��ع��������
        if (!skip)
        {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  //�ŵ�textures_loaded��
        }
    }
    return textures;
}

GLuint ObjModel::TextureFromFile(const char* path, const std::string& directory)
{
    std::string filename = std::string(path);
    filename = directory + '\\' + filename;
    //std::cout << filename << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format = GL_RGB;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

