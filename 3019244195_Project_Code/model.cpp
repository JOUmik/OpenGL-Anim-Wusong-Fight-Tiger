#include "model.h"
#include "stb_image.h"

// һЩ������ɫ
const glm::vec3 basic_colors[8] = {
	glm::vec3(1.0, 1.0, 1.0), // White
	glm::vec3(1.0, 1.0, 0.0), // Yellow
	glm::vec3(0.0, 1.0, 0.0), // Green
	glm::vec3(0.0, 1.0, 1.0), // Cyan
	glm::vec3(1.0, 0.0, 1.0), // Magenta
	glm::vec3(1.0, 0.0, 0.0), // Red
	glm::vec3(0.0, 0.0, 0.0), // Black
	glm::vec3(0.0, 0.0, 1.0)	 // Blue
};

// ������ĸ�����
const glm::vec3 cube_vertices[8] = {
	glm::vec3(-0.5, -0.5, -0.5),
	glm::vec3(0.5, -0.5, -0.5),
	glm::vec3(-0.5, 0.5, -0.5),
	glm::vec3(0.5, 0.5, -0.5),
	glm::vec3(-0.5, -0.5, 0.5),
	glm::vec3(0.5, -0.5, 0.5),
	glm::vec3(-0.5, 0.5, 0.5),
	glm::vec3(0.5, 0.5, 0.5)
};

// ������ƽ��
const vec3 square_vertices[4] = {
	vec3(-0.5, -0.5, 0.0),
	vec3(0.5, -0.5, 0.0),
	vec3(0.5, 0.5, 0.0),
	vec3(-0.5, 0.5, 0.0),
};

glm::vec3 Model::getScale()
{
	return scale;
}

glm::mat4 Model::getModelMatrix()
{
	if (this->symbol == 1)
	{
		glm::mat4 ZERO(
			vec4(0, 0, 0, 0),
			vec4(0, 0, 0, 0),
			vec4(0, 0, 0, 0),
			vec4(0, 0, 0, 0)
		);

		return ZERO;
	}
	glm::mat4 parentTransform(    // ��������
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);

	glm::mat4 unit(    // ��λ����
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);

	if (parent)
	{
		parentTransform = parent->getModelMatrix();
	}

	vec3 BC = vec3(-this->rotatePoint.x, -this->rotatePoint.y, -this->rotatePoint.z);

	/*
	glm::mat4 T_BC = glm::translate(unit, BC);

	glm::mat4 T_CB = glm::translate(unit, rotatePoint);

	glm::mat4 trans = glm::translate(unit, this->translation);

	glm::mat4 rotateZ = glm::rotate(unit, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));    //��Z����ת
	glm::mat4 rotateY = glm::rotate(unit, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));    //��Y����ת
	glm::mat4 rotateX = glm::rotate(unit, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));    //��X����ת
	*/

	glm::mat4 T_BC(
		vec4(1, 0, 0, BC.x),
		vec4(0, 1, 0, BC.y),
		vec4(0, 0, 1, BC.z),
		vec4(0, 0, 0, 1)
	);

	glm::mat4 T_CB(
		vec4(1, 0, 0, this->rotatePoint.x),
		vec4(0, 1, 0, this->rotatePoint.y),
		vec4(0, 0, 1, this->rotatePoint.z),
		vec4(0, 0, 0, 1)
	);

	glm::mat4 trans(
		vec4(1, 0, 0, this->translation.x),
		vec4(0, 1, 0, this->translation.y),
		vec4(0, 0, 1, this->translation.z),
		vec4(0, 0, 0, 1)
	);

	GLfloat angleX = M_PI / 180.0 * this->rotation.x;
	GLfloat angleY = M_PI / 180.0 * this->rotation.y;
	GLfloat angleZ = M_PI / 180.0 * this->rotation.z;

	mat4 rotateX(
		vec4(1, 0, 0, 0),
		vec4(0, cos(angleX), -sin(angleX), 0),
		vec4(0, sin(angleX), cos(angleX), 0),
		vec4(0, 0, 0, 1)
	);

	mat4 rotateY(
		vec4(cos(angleY), 0, sin(angleY), 0),
		vec4(0, 1, 0, 0),
		vec4(-sin(angleY), 0, cos(angleY), 0),
		vec4(0, 0, 0, 1)
	);

	mat4 rotateZ(
		vec4(cos(angleZ), -sin(angleZ), 0, 0),
		vec4(sin(angleZ), cos(angleZ), 0, 0),
		vec4(0, 0, 1, 0),
		vec4(0, 0, 0, 1)
	);

	return
		T_BC *  	// ��ת���ƶ���ԭ��
		rotateZ * rotateY * rotateX * //��ת
		T_CB * // �ƻ�
		trans *	// �Ƶ���ģ�ͽӺϵ�
		parentTransform;	// ��Ӧ��ģ�͵ı任
}

void Model::setTranslation(glm::vec3 translation)
{
	this->translation = translation;
}

void Model::setRotation(glm::vec3 rotation)
{
	this->rotation = rotation;
}

void Model::setScale(glm::vec3 scale)
{
	this->scale = scale;
}

void Model::setNormalize(bool do_norm)
{
	do_normalize_size = do_norm;
}

void Model::generateCube(glm::vec3 _color, glm::vec3 _scale)
{
	// ��������ǰҪ�Ȱ���Щvector���
	cleanData();

	for (int i = 0; i < 8; i++)
	{
		vertex_positions.push_back(cube_vertices[i] * _scale);
		if (_color[0] == -1) {
			vertex_colors.push_back(basic_colors[i]);
		}
		else {
			vertex_colors.push_back(_color);
		}
	}

	// ÿ��������Ƭ�Ķ����±�
	faces.push_back(glm::ivec3(0, 3, 1));
	faces.push_back(glm::ivec3(0, 2, 3));
	faces.push_back(glm::ivec3(1, 5, 4));
	faces.push_back(glm::ivec3(1, 4, 0));
	faces.push_back(glm::ivec3(4, 2, 0));
	faces.push_back(glm::ivec3(4, 6, 2));
	faces.push_back(glm::ivec3(5, 6, 4));
	faces.push_back(glm::ivec3(5, 7, 6));
	faces.push_back(glm::ivec3(2, 6, 7));
	faces.push_back(glm::ivec3(2, 7, 3));
	faces.push_back(glm::ivec3(1, 7, 5));
	faces.push_back(glm::ivec3(1, 3, 7));

	// ��ɫ�±꣬��һ�������ɫ��һ��
	for (int i = 0; i < 6; i++) {
		color_index.push_back(glm::ivec3(i, i, i));
		color_index.push_back(glm::ivec3(i, i, i));
	}

	texture_index = faces;
	normal_index = faces;
	storeFacesPoints();

	textures.clear();

	// ������������
	// 031
	textures.push_back(glm::vec2(0.25, 0.25));
	textures.push_back(glm::vec2(0.5, 0.5));
	textures.push_back(glm::vec2(0.5, 0.25));
	// 023
	textures.push_back(glm::vec2(0.25, 0.25));
	textures.push_back(glm::vec2(0.25, 0.5));
	textures.push_back(glm::vec2(0.5, 0.5));
	// 154
	textures.push_back(glm::vec2(0.5, 0.25));
	textures.push_back(glm::vec2(0.5, 0.0));
	textures.push_back(glm::vec2(0.25, 0.0));
	// 140
	textures.push_back(glm::vec2(0.5, 0.25));
	textures.push_back(glm::vec2(0.25, 0.0));
	textures.push_back(glm::vec2(0.25, 0.25));
	// 420
	textures.push_back(glm::vec2(0.0, 0.25));
	textures.push_back(glm::vec2(0.25, 0.5));
	textures.push_back(glm::vec2(0.25, 0.25));
	// 462
	textures.push_back(glm::vec2(0.0, 0.25));
	textures.push_back(glm::vec2(0.0, 0.5));
	textures.push_back(glm::vec2(0.25, 0.5));
	// 564
	textures.push_back(glm::vec2(0.75, 0.25));
	textures.push_back(glm::vec2(1.0, 0.5));
	textures.push_back(glm::vec2(1.0, 0.25));
	// 576
	textures.push_back(glm::vec2(0.75, 0.25));
	textures.push_back(glm::vec2(0.75, 0.5));
	textures.push_back(glm::vec2(1.0, 0.5));
	// 267
	textures.push_back(glm::vec2(0.25, 0.5));
	textures.push_back(glm::vec2(0.25, 0.75));
	textures.push_back(glm::vec2(0.5, 0.75));
	// 273
	textures.push_back(glm::vec2(0.25, 0.5));
	textures.push_back(glm::vec2(0.5, 0.75));
	textures.push_back(glm::vec2(0.5, 0.5));
	// 175
	textures.push_back(glm::vec2(0.5, 0.25));
	textures.push_back(glm::vec2(0.75, 0.5));
	textures.push_back(glm::vec2(0.75, 0.25));
	// 137
	textures.push_back(glm::vec2(0.5, 0.25));
	textures.push_back(glm::vec2(0.5, 0.5));
	textures.push_back(glm::vec2(0.75, 0.5));

	normals.clear();

	// �����εķ��������ܿ�֮ǰ���㷨�����ķ���ֱ�Ӽ��㣬��Ϊÿ���ı���ƽ���������ģ�������������
	for (int i = 0; i < faces.size(); i++)
	{
		normals.push_back(face_normals[i]);
		normals.push_back(face_normals[i]);
		normals.push_back(face_normals[i]);
	}
}

void Model::generateSquare(glm::vec3 color, glm::vec3 _scale, float texScale)
{
	// ��������ǰҪ�Ȱ���Щvector���
	cleanData();

	for (int i = 0; i < 4; i++)
	{
		vertex_positions.push_back(square_vertices[i] * _scale);
		vertex_colors.push_back(color);
		vertex_normals.push_back(vec3(0, 0, 1));
	}

	// ÿ��������Ƭ�Ķ����±�
	faces.push_back(ivec3(0, 1, 2));
	faces.push_back(ivec3(0, 2, 3));
	normal_index = faces;
	color_index = faces;
	storeFacesPoints();

	textures.clear();
	// ������������
	// 012
	textures.push_back(vec2(0, 0));
	textures.push_back(vec2(1 * texScale, 0));
	textures.push_back(vec2(1 * texScale, 1 * texScale));
	// 023
	textures.push_back(vec2(0, 0));
	textures.push_back(vec2(1 * texScale, 1 * texScale));
	textures.push_back(vec2(0, 1 * texScale));

	normals.clear();
	for (int i = 0; i < 6; i++)
	{
		normals.push_back(vec3(0, 0, 1));
	}
}

Model::Model()
{
	do_normalize_size = true;
	diagonal_length = 1.0;
	parent = NULL;
	connect_position = glm::vec3(0.0);
	scale = glm::vec3(1.0);
	rotation = glm::vec3(0.0);
	translation = glm::vec3(0.0);
}

Model::~Model()
{
}

void Model::setParent(Model* parent)
{
	this->parent = parent;
}

void Model::storeFacesPoints()
{
	// ����ȡ�Ķ������������Ƭ�ϵĶ����±��������
	// Ҫ���ݸ�GPU��points��������

	// ��¼�����Χ�д�С���������ڴ�С�Ĺ�һ��
	// �Ȼ�ð�Χ�еĶԽǶ���
	float max_x = -FLT_MAX;
	float max_y = -FLT_MAX;
	float max_z = -FLT_MAX;
	float min_x = FLT_MAX;
	float min_y = FLT_MAX;
	float min_z = FLT_MAX;
	for (int i = 0; i < vertex_positions.size(); i++) {
		auto& position = vertex_positions[i];
		if (position.x > max_x) max_x = position.x;
		if (position.y > max_y) max_y = position.y;
		if (position.z > max_z) max_z = position.z;
		if (position.x < min_x) min_x = position.x;
		if (position.y < min_y) min_y = position.y;
		if (position.z < min_z) min_z = position.z;
	}
	up_corner = vec3(max_x, max_y, max_z);
	down_corner = vec3(min_x, min_y, min_z);
	center = vec3((min_x + max_x) / 2.0, (min_y + max_y) / 2.0, (min_z + max_z) / 2.0);

	diagonal_length = length(up_corner - down_corner);

	// ���Ƿ��һ�������С���ǵĻ������ｫ���嶥�����ŵ��Խ��߳���Ϊ1�İ�Χ����
	if (do_normalize_size) {
		for (int i = 0; i < vertex_positions.size(); i++) {
			vertex_positions[i] = (vertex_positions[i] - center) / diagonal_length;
		}
	}


	// ���㷨����
	if (vertex_normals.size() == 0)
		computeVertexNormals();

	for (int i = 0; i < faces.size(); i++)
	{
		// ����
		points.push_back(vertex_positions[faces[i].x]);
		points.push_back(vertex_positions[faces[i].y]);
		points.push_back(vertex_positions[faces[i].z]);
		// ��ɫ
		//colors.push_back(vertex_colors[color_index[i].x]);
		//colors.push_back(vertex_colors[color_index[i].y]);
		//colors.push_back(vertex_colors[color_index[i].z]);
		// ������
		if (vertex_normals.size() != 0)
		{
			normals.push_back(vertex_normals[normal_index[i].x]);
			normals.push_back(vertex_normals[normal_index[i].y]);
			normals.push_back(vertex_normals[normal_index[i].z]);
		}
		// ����
		if (vertex_textures.size() != 0)
		{
			textures.push_back(vertex_textures[texture_index[i].x]);
			textures.push_back(vertex_textures[texture_index[i].y]);
			textures.push_back(vertex_textures[texture_index[i].z]);
		}
	}
}

void Model::cleanData()
{
	vertex_positions.clear();
	vertex_colors.clear();
	vertex_normals.clear();
	vertex_textures.clear();

	faces.clear();
	normal_index.clear();
	color_index.clear();
	texture_index.clear();

	face_normals.clear();

	points.clear();
	colors.clear();
	normals.clear();
	textures.clear();
}

void load_texture_STBImage(const std::string& file_name, GLuint& m_texName)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// �ָ���ʼ�����ʽ
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// �ͷ�ͼ���ڴ�
	stbi_image_free(pixels);
};

void Model::bindData()
{
	const std::vector<vec3>& points = this->getPoints();
	const std::vector<vec3>& normals = this->getNormals();
	const std::vector<vec3>& colors = this->getColors();
	const std::vector<vec2>& textures = this->getTextures();

	// ���������������
	glGenVertexArrays(1, &this->VAO);  	// ����1�������������
	glBindVertexArray(this->VAO);  	// �󶨶����������

	// ��������ʼ�����㻺�����
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

	// ��������������ɫ��
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)((points.size()) * sizeof(vec3)));

	// ����ɫ������ɫ��
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((points.size() + normals.size()) * sizeof(vec3)));

	// ���������괫����ɫ��
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)((points.size() + colors.size() + normals.size()) * sizeof(vec3)));

	// ��������Ļ������
	glGenTextures(1, &this->texture);
	// ����stb_image��������
	load_texture_STBImage(this->texture_path, this->texture);

	// Clean up
	glUseProgram(0);

	glBindVertexArray(0);
}

std::vector<ivec3> Model::getFaces()
{
	return faces;
}
std::vector<vec3> Model::getPoints()
{
	return points;
}
std::vector<vec3> Model::getColors()
{
	return colors;
}

std::vector<glm::vec3> Model::getNormals()
{
	return normals;;
}

std::vector<glm::vec2> Model::getTextures()
{
	return textures;
}

void Model::computeTriangleNormals()
{
	face_normals.resize(faces.size());
	for (size_t i = 0; i < faces.size(); i++)
	{
		auto& face = faces[i];
		vec3 v01 = vertex_positions[face.y] - vertex_positions[face.x];
		vec3 v02 = vertex_positions[face.z] - vertex_positions[face.x];
		face_normals[i] = normalize(cross(v01, v02));
	}
}

void Model::computeVertexNormals()
{
	// ������Ƭ�ķ�����
	if (face_normals.size() == 0 && faces.size() > 0)
	{
		computeTriangleNormals();
	}
	// ��ʼ��������Ϊ0
	vertex_normals.resize(vertex_positions.size(), vec3(0, 0, 0));
	for (size_t i = 0; i < faces.size(); i++)
	{
		auto& face = faces[i];
		vertex_normals[face.x] += face_normals[i];
		vertex_normals[face.y] += face_normals[i];
		vertex_normals[face.z] += face_normals[i];
	}
	for (size_t i = 0; i < vertex_normals.size(); i++)
	{
		vertex_normals[i] = normalize(vertex_normals[i]);
	}
}

glm::vec3 Model::getTranslation()
{
	return translation;
}

glm::vec3 Model::getRotation()
{
	return rotation;
}