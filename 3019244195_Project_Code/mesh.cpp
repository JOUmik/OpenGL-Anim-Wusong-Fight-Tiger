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

	// �����ж����ʽ
	if (width * channels % 4 != 0) glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	GLenum format = GL_RGB;
	// ����ͨ����ʽ
	switch (channels) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	default: format = GL_RGB; break;
	}

	// ���������
	glBindTexture(GL_TEXTURE_2D, m_texName);

	// ָ������ķŴ���С�˲���ʹ�����Է�ʽ������ͼƬ�Ŵ��ʱ���ֵ��ʽ
	// ��ͼƬ��rgb�����ϴ���opengl
	glTexImage2D(
		GL_TEXTURE_2D,	// ָ��Ŀ���������ֵ������GL_TEXTURE_2D
		0,				// ִ��ϸ�ڼ���0���������ͼ�񼶱�n��ʾ��N����ͼϸ������
		format,			// �������ݵ���ɫ��ʽ(GPU�Դ�)
		width,			// ��ȡ����ڵ��Կ���֧�ֲ�������������Ⱥ͸߶ȱ�����2^n
		height,			// �߶ȡ����ڵ��Կ���֧�ֲ�������������Ⱥ͸߶ȱ�����2^n
		0,				// ָ���߿�Ŀ�ȡ�����Ϊ0
		format,			// �������ݵ���ɫ��ʽ(CPU�ڴ�)
		GL_UNSIGNED_BYTE,	// ָ���������ݵ���������
		pixels			// ָ���ڴ���ָ��ͼ�����ݵ�ָ��
	);

	// ���ɶ༶��Զ����������1/3���Դ棬��С�ֱ���ʱ��ø��õ�Ч��
	// glGenerateMipmap(GL_TEXTURE_2D);

	// ָ����ֵ����
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// �ָ���ʼ�����ʽ
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// �ͷ�ͼ���ڴ�
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

	// ���������������
	glGenVertexArrays(1, &mesh->VAO);  	// ����1�������������
	glBindVertexArray(mesh->VAO);  	// �󶨶����������

	// ��������ʼ�����㻺�����
	glGenBuffers(1, &mesh->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
	glBufferData(GL_ARRAY_BUFFER,
		points.size() * sizeof(vec3) +
		normals.size() * sizeof(vec3) +
		colors.size() * sizeof(vec3) +
		textures.size() * sizeof(vec2),
		NULL, GL_STATIC_DRAW);

	// �󶨶�������
	glBufferSubData(GL_ARRAY_BUFFER, 0, points.size() * sizeof(vec3), points.data());
	// �󶨷���������
	glBufferSubData(GL_ARRAY_BUFFER, points.size() * sizeof(vec3), normals.size() * sizeof(vec3), normals.data());
	// ����ɫ����
	glBufferSubData(GL_ARRAY_BUFFER, (points.size() + normals.size()) * sizeof(vec3), colors.size() * sizeof(vec3), colors.data());
	// ����������
	glBufferSubData(GL_ARRAY_BUFFER, (points.size() + normals.size() + colors.size()) * sizeof(vec3), textures.size() * sizeof(vec2), textures.data());

	// �����㴫����ɫ��
	//GLuint pLocation = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

	// ��������������ɫ��
	//GLuint nLocation = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3,
		GL_FLOAT, GL_FALSE, 0,
		(void*)((points.size()) * sizeof(vec3)));

	// ����ɫ������ɫ��
	//GLuint cLocation = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0,
		(void*)((points.size() + normals.size()) * sizeof(vec3)));

	// ���������괫����ɫ��
	//GLuint tLocation = glGetAttribLocation(program, "vTexture");
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2,
		GL_FLOAT, GL_FALSE, 0,
		(void*)((points.size() + colors.size() + normals.size()) * sizeof(vec3)));

	// ��������Ļ������
	glGenTextures(1, &mesh->texture);
	// ����stb_image��������
	load_texture_STBImage(mesh->texture_path, mesh->texture);
	// ������������ �����ɵ�������shader
	glUniform1i(glGetUniformLocation(program, "texture"), 0);

	// Clean up
	glUseProgram(0);

	glBindVertexArray(0);
}

void Mesh::drawMesh(Model* mesh, Shader* shader)
{
	// ���� model ����
	mat4 model = mesh->getModelMatrix();
	GLuint modelLocation = glGetUniformLocation(shader->Program, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_TRUE, glm::value_ptr(model));

	// diffuess����
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