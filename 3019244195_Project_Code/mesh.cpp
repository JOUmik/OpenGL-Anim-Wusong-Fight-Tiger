#include "mesh.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mesh::Mesh() {};
Mesh::~Mesh() {};

void Mesh::load_texture_STBImage(const std::string& file_name, GLuint& m_texName)
{
	int width, height, channels = 0;
	unsigned char* pixels = NULL;
	stbi_set_flip_vertically_on_load(true);
	pixels = stbi_load(file_name.c_str(), &width, &height, &channels, 0);

	// 调整行对齐格式
	if (width * channels % 4 != 0) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLenum format = GL_RGB;
	// 设置通道格式
	switch (channels) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: format = GL_RGB; break;
	}

	// 绑定纹理对象
	glBindTexture(GL_TEXTURE_2D, m_texName);

	// 指定纹理的放大，缩小滤波，使用线性方式，即当图片放大的时候插值方式
	// 将图片的rgb数据上传给opengl
	glTexImage2D(
		GL_TEXTURE_2D,	// 指定目标纹理，这个值必须是GL_TEXTURE_2D
		0,				// 执行细节级别，0是最基本的图像级别，n表示第N级贴图细化级别
		format,			// 纹理数据的颜色格式(GPU显存)
		width,			// 宽度。早期的显卡不支持不规则的纹理，则宽度和高度必须是2^n
		height,			// 高度。早期的显卡不支持不规则的纹理，则宽度和高度必须是2^n
		0,				// 指定边框的宽度。必须为0
		format,			// 像素数据的颜色格式(CPU内存)
		GL_UNSIGNED_BYTE,	// 指定像素数据的数据类型
		pixels			// 指定内存中指向图像数据的指针
	);

	// 生成多级渐远纹理，多消耗1/3的显存，较小分辨率时获得更好的效果
	// glGenerateMipmap(GL_TEXTURE_2D);

	// 指定插值方法
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// 恢复初始对齐格式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// 释放图形内存
	stbi_image_free(pixels);
};

void Mesh::addMesh(Model* mesh)
{
	mesh->bindData();
	meshes.push_back(mesh);
}

void Mesh::bindObjectAndData(Model* mesh, GLuint program)
{
	const std::vector<vec3>& points = mesh->getPoints();
	const std::vector<vec3>& normals = mesh->getNormals();
	const std::vector<vec3>& colors = mesh->getColors();
	const std::vector<vec2>& textures = mesh->getTextures();

	// 创建顶点数组对象
	glGenVertexArrays(1, &mesh->VAO);  	// 分配1个顶点数组对象
	glBindVertexArray(mesh->VAO);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &mesh->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER,
		points.size() * sizeof(vec3) +
		normals.size() * sizeof(vec3) +
		colors.size() * sizeof(vec3) +
		textures.size() * sizeof(vec2),
		NULL, GL_STATIC_DRAW);

	// 绑定顶点数据
	glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(vec3), points.data());
	// 绑定法向量数据
	glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), normals.size() * sizeof(vec3), normals.data());
	// 绑定颜色数据
	glBufferSubData(GL_ARRAY_BUFFER, (points.size() + normals.size()) * sizeof(vec3), colors.size() * sizeof(vec3), colors.data());
	// 绑定纹理数据
	glBufferSubData(GL_ARRAY_BUFFER, (points.size() + normals.size() + colors.size()) * sizeof(vec3), textures.size() * sizeof(vec2), textures.data());

	// 将顶点传入着色器
	//GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

	// 将法向量传入着色器
	//GLuint nLocation = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3,
		GL_FLOAT, GL_FALSE, 0,
		(void*)((points.size()) * sizeof(vec3)));

	// 将颜色传入着色器
	//GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0,
		(void*)((points.size() + normals.size()) * sizeof(vec3)));

	// 将纹理坐标传入着色器
	//GLuint tLocation = glGetAttribLocation(program, "vTexture");
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2,
		GL_FLOAT, GL_FALSE, 0,
		(void*)((points.size() + colors.size() + normals.size()) * sizeof(vec3)));

	// 创建纹理的缓存对象
	glGenTextures(1, &mesh->texture);
	// 调用stb_image生成纹理
	load_texture_STBImage(mesh->texture_path, mesh->texture);
	// 传递纹理数据 将生成的纹理传给shader
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	// Clean up
	glUseProgram(0);

	glBindVertexArray(0);
}

void Mesh::drawMesh(Model* mesh, Shader* shader)
{
	// 传递 model 矩阵
	mat4 model = mesh->getModelMatrix();
	GLuint modelLocation = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, glm::value_ptr(model));

	// diffuess纹理
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(mesh->textureType, mesh->texture);
	glUniform1i(glGetUniformLocation(shader->Program, "diffuseTexture"), 0);

	glBindVertexArray(mesh->VAO);

	glDrawArrays(GL_TRIANGLES, 0, mesh->getPoints().size());

	glBindVertexArray(0);
}


void Mesh::drawMeshes(Shader* shader)
{
	for (int i = 0; i < meshes.size(); i++)
	{
		drawMesh(meshes[i], shader);
	}
}