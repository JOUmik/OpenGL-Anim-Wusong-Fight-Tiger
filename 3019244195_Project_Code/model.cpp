#include "model.h"
#include "stb_image.h"

// 一些基础颜色
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

// 立方体的各个点
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

// 正方形平面
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
	glm::mat4 parentTransform(    // 父结点矩阵
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, 1, 0, 0),
		glm::vec4(0, 0, 1, 0),
		glm::vec4(0, 0, 0, 1)
	);

	glm::mat4 unit(    // 单位矩阵
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

	glm::mat4 rotateZ = glm::rotate(unit, glm::radians(this->rotation.z), glm::vec3(0, 0, 1));    //绕Z轴旋转
	glm::mat4 rotateY = glm::rotate(unit, glm::radians(this->rotation.y), glm::vec3(0, 1, 0));    //绕Y轴旋转
	glm::mat4 rotateX = glm::rotate(unit, glm::radians(this->rotation.x), glm::vec3(1, 0, 0));    //绕X轴旋转
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
		T_BC *  	// 旋转点移动到原点
		rotateZ * rotateY * rotateX * //旋转
		T_CB * // 移回
		trans *	// 移到父模型接合点
		parentTransform;	// 对应父模型的变换
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
	// 创建顶点前要先把那些vector清空
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

	// 每个三角面片的顶点下标
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

	// 颜色下标，让一个面的颜色都一样
	for (int i = 0; i < 6; i++) {
		color_index.push_back(glm::ivec3(i, i, i));
		color_index.push_back(glm::ivec3(i, i, i));
	}

	texture_index = faces;
	normal_index = faces;
	storeFacesPoints();

	textures.clear();

	// 顶点纹理坐标
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

	// 正方形的法向量不能靠之前顶点法向量的方法直接计算，因为每个四边形平面是正交的，不是连续曲面
	for (int i = 0; i < faces.size(); i++)
	{
		normals.push_back(face_normals[i]);
		normals.push_back(face_normals[i]);
		normals.push_back(face_normals[i]);
	}
}

void Model::generateSquare(glm::vec3 color, glm::vec3 _scale, float texScale)
{
	// 创建顶点前要先把那些vector清空
	cleanData();

	for (int i = 0; i < 4; i++)
	{
		vertex_positions.push_back(square_vertices[i] * _scale);
		vertex_colors.push_back(color);
		vertex_normals.push_back(vec3(0, 0, 1));
	}

	// 每个三角面片的顶点下标
	faces.push_back(ivec3(0, 1, 2));
	faces.push_back(ivec3(0, 2, 3));
	normal_index = faces;
	color_index = faces;
	storeFacesPoints();

	textures.clear();
	// 顶点纹理坐标
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
	// 将读取的顶点根据三角面片上的顶点下标逐个加入
	// 要传递给GPU的points等容器内

	// 记录物体包围盒大小，可以用于大小的归一化
	// 先获得包围盒的对角顶点
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

	// 看是否归一化物体大小，是的话，这里将物体顶点缩放到对角线长度为1的包围盒内
	if (do_normalize_size) {
		for (int i = 0; i < vertex_positions.size(); i++) {
			vertex_positions[i] = (vertex_positions[i] - center) / diagonal_length;
		}
	}


	// 计算法向量
	if (vertex_normals.size() == 0)
		computeVertexNormals();

	for (int i = 0; i < faces.size(); i++)
	{
		// 坐标
		points.push_back(vertex_positions[faces[i].x]);
		points.push_back(vertex_positions[faces[i].y]);
		points.push_back(vertex_positions[faces[i].z]);
		// 颜色
		//colors.push_back(vertex_colors[color_index[i].x]);
		//colors.push_back(vertex_colors[color_index[i].y]);
		//colors.push_back(vertex_colors[color_index[i].z]);
		// 法向量
		if (vertex_normals.size() != 0)
		{
			normals.push_back(vertex_normals[normal_index[i].x]);
			normals.push_back(vertex_normals[normal_index[i].y]);
			normals.push_back(vertex_normals[normal_index[i].z]);
		}
		// 纹理
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// 恢复初始对齐格式
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	// 释放图形内存
	stbi_image_free(pixels);
};

void Model::bindData()
{
	const std::vector<vec3>& points = this->getPoints();
	const std::vector<vec3>& normals = this->getNormals();
	const std::vector<vec3>& colors = this->getColors();
	const std::vector<vec2>& textures = this->getTextures();

	// 创建顶点数组对象
	glGenVertexArrays(1, &this->VAO);  	// 分配1个顶点数组对象
	glBindVertexArray(this->VAO);  	// 绑定顶点数组对象

	// 创建并初始化顶点缓存对象
	glGenBuffers(1, &this->VBO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)(0));

	// 将法向量传入着色器
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)((points.size()) * sizeof(vec3)));

	// 将颜色传入着色器
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)((points.size() + normals.size()) * sizeof(vec3)));

	// 将纹理坐标传入着色器
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)((points.size() + colors.size() + normals.size()) * sizeof(vec3)));

	// 创建纹理的缓存对象
	glGenTextures(1, &this->texture);
	// 调用stb_image生成纹理
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
	// 计算面片的法向量
	if (face_normals.size() == 0 && faces.size() > 0)
	{
		computeTriangleNormals();
	}
	// 初始化法向量为0
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