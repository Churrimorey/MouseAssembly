#include "ImportedModel.h"
#include <gl/glew.h>
#include <gl/glut.h>
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include "vendor/glm/gtc/type_ptr.hpp"
#include <sstream>

static const int numVAOs = 1;
static const int numVBOs = 3;
GLuint renderingProgram = 0;
GLuint vao[numVAOs] = { 0 };
GLuint vbo[numVBOs] = { 0 };

void setupVertices(const char* filePath)
{
	ImportedModel myModel(filePath);
	vector<glm::vec3> vert = myModel.getVertices();
	vector<glm::vec2> text = myModel.getTextureCoords();
	vector<glm::vec3> norm = myModel.getNormals();

	vector<float> pValues;
	vector<float> tValues;
	vector<float> nValues;

	for (int i = 0; i < myModel.getNumVertices(); i++)
	{
		/*pValues.push_back(vert[i * 3 + 0].x);
		pValues.push_back(vert[i * 3 + 1].y);
		pValues.push_back(vert[i * 3 + 2].z);

		tValues.push_back(text[i * 2 + 0].s);
		tValues.push_back(text[i * 2 + 1].t);

		nValues.push_back(norm[i * 3 + 0].x);
		nValues.push_back(norm[i * 3 + 1].y);
		nValues.push_back(norm[i * 3 + 2].z);*/

		pValues.push_back(vert[i].x);
		pValues.push_back(vert[i].y);
		pValues.push_back(vert[i].z);

		tValues.push_back(text[i].s);
		tValues.push_back(text[i].t);

		nValues.push_back(norm[i].x);
		nValues.push_back(norm[i].y);
		nValues.push_back(norm[i].z);
	}

	glGenVertexArrays(numVAOs, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, pValues.size() * sizeof(float), &(pValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, myModel.getVertices().size() * sizeof(float), &(pVlaues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, tValues.size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, myModel.getTextureCoords().size() * sizeof(float), &(tValues[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, nValues.size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, myModel.getNormals().size() * sizeof(float), &(nValues[0]), GL_STATIC_DRAW);
}

void render(ImportedModel myModel, int textureID) {
	// 使用之前创建的着色器程序
	glUseProgram(renderingProgram);

	// 绑定 VAO
	glBindVertexArray(vao[0]);

	// 启用顶点属性数组
	glEnableVertexAttribArray(0); // 位置
	glEnableVertexAttribArray(1); // 纹理坐标
	glEnableVertexAttribArray(2); // 法向量

	// 绑定纹理（如果有）
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID); // 假设 textureID 是已加载的纹理 ID

	// 绘制模型
	glDrawArrays(GL_TRIANGLES, 0, myModel.getNumVertices());

	// 禁用顶点属性数组
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	// 解绑 VAO
	glBindVertexArray(0);
}
void ImportedModel::drawobj() {
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<glm::vec2> texCoords = getTextureCoords();
	std::vector<glm::vec3> normals = getNormals();

	glPushMatrix();
	glBegin(GL_TRIANGLES); // 假设模型是由三角形组成的
	for (int i = 0; i < getNumVertices(); ++i) {
		// 应用纹理坐标
		if (i < texCoords.size()) {
			glTexCoord2f(texCoords[i].x, texCoords[i].y);
		}

		// 应用法线
		if (i < normals.size()) {
			glNormal3f(normals[i].x, normals[i].y, normals[i].z);
		}

		// 绘制顶点
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
	}
	glEnd();
	glPopMatrix();
}
ImportedModel::ImportedModel()
{
}

ImportedModel::ImportedModel(const char* filePath)
{
	ModelImporter modelImporter = ModelImporter();
	modelImporter.parseOBJ(filePath);
	_numVertices = modelImporter.getNumVertices();
	vector<float> verts = modelImporter.getVertices();
	vector<float> tcs = modelImporter.getTextureCoordinates();
	vector<float> normals = modelImporter.getNormals();

	for (int i = 0; i < _numVertices; i++)
	{
		_vertices.push_back(glm::vec3(verts[i * 3 + 0], verts[i * 3 + 1], verts[i * 3 + 2]));
		_texCoords.push_back(glm::vec2(tcs[i * 2 + 0], tcs[i * 2 + 1]));
		_normalVecs.push_back(glm::vec3(normals[i * 3 + 0], normals[i * 3 + 1], normals[i * 3 + 2]));
	}
}

int ImportedModel::getNumVertices()
{
	return _numVertices;
}

std::vector<glm::vec3> ImportedModel::getVertices()
{
	return _vertices;
}

std::vector<glm::vec2> ImportedModel::getTextureCoords()
{
	return _texCoords;
}

std::vector<glm::vec3> ImportedModel::getNormals()
{
	return _normalVecs;
}

/// <summary>
/// ModelImporter implement
/// </summary>

ModelImporter::ModelImporter()
{

}

void ModelImporter::parseOBJ(const char* filePath)
{
	float x = 0.f, y = 0.f, z = 0.f;
	string content;
	ifstream fileStream(filePath, ios::in);
	string line = "";

	while (!fileStream.eof())
	{
		getline(fileStream, line);
		if (line.compare(0, 2, "v ") == 0)
		{
			std::stringstream ss(line.erase(0, 1));
			ss >> x >> y >> z;
			//ss >> x; ss >> y; ss >> z;
			_vertVals.push_back(x);
			_vertVals.push_back(y);
			_vertVals.push_back(z);
		}
		if (line.compare(0, 2, "vt") == 0)
		{
			std::stringstream ss(line.erase(0, 2));
			ss >> x >> y;
			_stVals.push_back(x);
			_stVals.push_back(y);
		}
		if (line.compare(0, 2, "vn") == 0)
		{
			std::stringstream ss(line.erase(0, 2));
			ss >> x >> y >> z;
			_normVals.push_back(x);
			_normVals.push_back(y);
			_normVals.push_back(z);
		}
		if (line.compare(0, 1, "f") == 0)
		{
			string oneCorner, v, t, n;
			std::stringstream ss(line.erase(0, 2));
			for (int i = 0; i < 3; i++)
			{
				getline(ss, oneCorner, ' ');
				//getline(ss, oneCorner, " ");
				stringstream oneCornerSS(oneCorner);
				getline(oneCornerSS, v, '/');
				getline(oneCornerSS, t, '/');
				getline(oneCornerSS, n, '/');

				// 处理顶点索引
				int vertRef = v.empty() ? -1 : (stoi(v) - 1) * 3;
				if (vertRef != -1) {
					_triangleVerts.push_back(_vertVals[vertRef]);
					_triangleVerts.push_back(_vertVals[vertRef + 1]);
					_triangleVerts.push_back(_vertVals[vertRef + 2]);
				}

				// 处理纹理坐标索引
				int tcRef = t.empty() ? -1 : (stoi(t) - 1) * 2;
				if (tcRef != -1) {
					_textureCoords.push_back(_stVals[tcRef]);
					_textureCoords.push_back(_stVals[tcRef + 1]);
				}

				// 处理法线索引
				int normRef = n.empty() ? -1 : (stoi(n) - 1) * 3;
				if (normRef != -1) {
					_normals.push_back(_normVals[normRef]);
					_normals.push_back(_normVals[normRef + 1]);
					_normals.push_back(_normVals[normRef + 2]);
				}
			}

		}
	}
}

int ModelImporter::getNumVertices()
{
	return (_triangleVerts.size() / 3);
}

std::vector<float> ModelImporter::getVertices()
{
	return _triangleVerts;
}

std::vector<float> ModelImporter::getTextureCoordinates()
{
	return _textureCoords;
}

std::vector<float> ModelImporter::getNormals()
{
	return _normals;
}

