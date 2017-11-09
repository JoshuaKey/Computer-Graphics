#include "stdafx.h"
#include "mesh.h"
#include <glm/glm.hpp> 

#define TINYOBJLOADER_IMPLEMENTATION // define this in only *one* .cc
#include "tiny_obj_loader.h"

Mesh::Mesh()
{
	//
}

Mesh::~Mesh()
{
	//
}

void Mesh::Use()
{
	assert(m_vao != 0);
	glBindVertexArray(m_vao);
}

void Mesh::BindVertexAttrib(GLuint attrib, eVertexType type)
{
	glEnableVertexAttribArray(attrib);
	glVertexAttribBinding(attrib, static_cast<int>(type));
}

void Mesh::Render()
{
	glBindVertexArray(m_vao);
	glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	glBindVertexArray(0);
}

bool Mesh::Load(const std::string& filename)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string returnMessage;
	bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &returnMessage, filename.c_str());

	if (!returnMessage.empty())
	{
		std::cerr << returnMessage << std::endl;
	}

	if (result == false)
	{
		return false;
	}

	std::cout << std::endl;
	std::cout << "model: " << filename << std::endl;
	std::cout << "# vertex: " << attrib.vertices.size() / 3 << std::endl;
	std::cout << "# normals: " << attrib.normals.size() / 3 << std::endl;
	std::cout << "# texCoords: " << attrib.texcoords.size() / 2 << std::endl;
	std::cout << "# materials: " << materials.size() << std::endl;
	std::cout << "# shapes: " << shapes.size() << std::endl << std::endl;
	
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;

	// load buffers
	for (const auto& shape : shapes)
	{
		size_t indexOffset = 0;
		for (const auto& numVerts : shape.mesh.num_face_vertices)
		{
			for (size_t v = 0; v < numVerts; v++)
			{
				tinyobj::index_t index = shape.mesh.indices[indexOffset + v];
				
				if (index.vertex_index != -1)
				{
					glm::vec3 position(0.0f);
					position.x = attrib.vertices[3 * index.vertex_index + 0];
					position.y = attrib.vertices[3 * index.vertex_index + 1];
					position.z = attrib.vertices[3 * index.vertex_index + 2];
					vertices.push_back(position);
				}

				if (index.normal_index != -1)
				{
					glm::vec3 normal(0.0f);
					normal.x = attrib.normals[3 * index.normal_index + 0];
					normal.y = attrib.normals[3 * index.normal_index + 1];
					normal.z = attrib.normals[3 * index.normal_index + 2];
					normals.push_back(normal);
				}

				if (index.texcoord_index != -1)
				{
					glm::vec2 texcoord(0.0f);
					texcoord.x = attrib.texcoords[2 * index.texcoord_index + 0];
					texcoord.y = 1.0f - attrib.texcoords[2 * index.texcoord_index + 1];
					texcoords.push_back(texcoord);
				}
			}
			indexOffset += numVerts;
			
			//shape.mesh.material_ids[numVerts];
		}
	}

	// compute normals if not provided
	if (normals.empty() && !vertices.empty())
	{
		for (size_t i = 0; i < vertices.size(); i+=3)
		{
			glm::vec3 normal;
			CalculateNormal(normal, vertices[i + 0], vertices[i + 1], vertices[i + 2]);
			normals.push_back(normal);
			normals.push_back(normal);
			normals.push_back(normal);
		}
	}

	// set
	if (!vertices.empty())
	{
		AddBuffer(eVertexType::POSITION, vertices.size(), sizeof(glm::vec3), (GLvoid*)vertices.data());
	}
	if (!normals.empty())
	{
		AddBuffer(eVertexType::NORMAL, normals.size(), sizeof(glm::vec3), (GLvoid*)normals.data());
	}
	if (!texcoords.empty())
	{
		AddBuffer(eVertexType::TEXCOORD, texcoords.size(), sizeof(glm::vec2), (GLvoid*)texcoords.data());
	}

	// create vertex array object
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	for (auto& buffer : m_buffers)
	{
		// create vertex buffer
		glGenBuffers(1, &buffer.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, buffer.vbo);
		glBufferData(GL_ARRAY_BUFFER, buffer.numElements * buffer.elementSizeInBytes, buffer.data, GL_STATIC_DRAW);

		// bind vertex buffer in vao
		glBindVertexBuffer(static_cast<int>(buffer.type), buffer.vbo, 0, buffer.elementSizeInBytes);
		int numValueSize = buffer.elementSizeInBytes / sizeof(float);
		glVertexAttribFormat(static_cast<int>(buffer.type), numValueSize, GL_FLOAT, GL_FALSE, 0);
	}
	
	m_numVertices = m_buffers[0].numElements;

	return true;
}

void Mesh::AddBuffer(eVertexType type, size_t numElements, size_t elementSize, void* data)
{
	// check if vertex buffer already exists
	bool exists = false;
	for (size_t i = 0; i < m_buffers.size(); ++i)
	{
		if (m_buffers[i].type == type)
		{
			exists = true;
			break;
		}
	}

	if (!exists)
	{
		m_buffers.push_back({ type, 0, numElements, elementSize, data });
	}
}

void Mesh::CalculateNormal(glm::vec3& normal, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
	glm::vec3 v1_0 = v1 - v0;
	glm::vec3 v2_0 = v2 - v0;

	normal = glm::cross(v1 - v0, v2 - v0);
	normal = glm::normalize(normal);
}