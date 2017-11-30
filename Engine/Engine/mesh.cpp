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
	LARGE_INTEGER beginHigh, frequencyHigh, endHigh;
	QueryPerformanceCounter(&beginHigh);
	QueryPerformanceFrequency(&frequencyHigh);

	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string returnMessage;
	bool result = tinyobj::LoadObj(&attrib, &shapes, &materials, &returnMessage, filename.c_str());

	QueryPerformanceCounter(&endHigh);

	std::cout << "Time Elapsed: " << endHigh.QuadPart - beginHigh.QuadPart << " Or... " <<
		(endHigh.QuadPart - beginHigh.QuadPart) / (frequencyHigh.QuadPart * 1.0) <<
		" Seconds\n" << std::endl;

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
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec3> bitTangents;

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
		glm::vec3* data = (glm::vec3*)(GLvoid*)vertices.data();
		if(data != nullptr) { data = nullptr; }
	}
	if (!normals.empty())
	{
		AddBuffer(eVertexType::NORMAL, normals.size(), sizeof(glm::vec3), (GLvoid*)normals.data());
	}
	if (!texcoords.empty())
	{
		AddBuffer(eVertexType::TEXCOORD, texcoords.size(), sizeof(glm::vec2), (GLvoid*)texcoords.data());
	}
	if (!vertices.empty() && !texcoords.empty() && !normals.empty())
	{

		for (int i = 0; i < vertices.size(); i += 3)
		{
			glm::vec3 bitTangent;
			glm::vec3 tangent;

			glm::vec3 pos1 = vertices[i];
			glm::vec3 pos2 = vertices[i+1];
			glm::vec3 pos3 = vertices[i+2];
			glm::vec2 uv1 = texcoords[i];
			glm::vec2 uv2 = texcoords[i+1];
			glm::vec2 uv3 = texcoords[i+2];

			CalculateTangent(&tangent, &bitTangent, pos1, pos2, pos3, uv1, uv2, uv3);
			glm::vec3 normal = normals[i];
			glm::vec3 crossT = glm::cross(normal, tangent); // This
			glm::vec3 Tcross = glm::cross(tangent, normal);
			glm::vec3 crossB = glm::cross(normal, bitTangent);
			glm::vec3 Bcross = glm::cross(bitTangent, normal); // This
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			bitTangents.push_back(bitTangent);
			bitTangents.push_back(bitTangent);
			bitTangents.push_back(bitTangent);
		}
		AddBuffer(eVertexType::TANGENT, tangents.size(), sizeof(glm::vec3), (GLvoid*)tangents.data());
		AddBuffer(eVertexType::BITTANGENT, bitTangents.size(), sizeof(glm::vec3), (GLvoid*)bitTangents.data());
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

//void Mesh::AddTangentBuffer()
//{
//	if (CheckBufferExistence(eVertexType::TANGENT)) { return; } // Already exists
//
//	//BufferInfo tanBuffer = {eVertexType::TANGENT, 0, tangents.size(), sizeof(glm::vec3), (GLvoid*)tangents.data()};
//	//BufferInfo bitTanBuffer = {eVertexType::BITTANGENT, 0, bitTangents.size(), sizeof(glm::vec3),
//	//	(GLvoid*)bitTangents.data()};
//	//m_buffers.push_back(tanBuffer);
//	//m_buffers.push_back(bitTanBuffer);
//
//	//// TANGENT
//	//// create vertex buffer
//	//glGenBuffers(1, &tanBuffer.vbo);
//	//glBindBuffer(GL_ARRAY_BUFFER, tanBuffer.vbo);
//	//glBufferData(GL_ARRAY_BUFFER, tanBuffer.numElements * tanBuffer.elementSizeInBytes, tanBuffer.data, GL_STATIC_DRAW);
//
//	//// bind vertex buffer in vao
//	//glBindVertexBuffer(static_cast<int>(tanBuffer.type), tanBuffer.vbo, 0, tanBuffer.elementSizeInBytes);
//	//int numValueSize = tanBuffer.elementSizeInBytes / sizeof(float);
//	//glVertexAttribFormat(static_cast<int>(tanBuffer.type), numValueSize, GL_FLOAT, GL_FALSE, 0);
//
//	//// BIT TANGENT
//	//// create vertex buffer
//	//glGenBuffers(1, &bitTanBuffer.vbo);
//	//glBindBuffer(GL_ARRAY_BUFFER, bitTanBuffer.vbo);
//	//glBufferData(GL_ARRAY_BUFFER, bitTanBuffer.numElements * bitTanBuffer.elementSizeInBytes, bitTanBuffer.data, 
//	//	GL_STATIC_DRAW);
//
//	//// bind vertex buffer in vao
//	//glBindVertexBuffer(static_cast<int>(bitTanBuffer.type), bitTanBuffer.vbo, 0, bitTanBuffer.elementSizeInBytes);
//	//numValueSize = bitTanBuffer.elementSizeInBytes / sizeof(float);
//	//glVertexAttribFormat(static_cast<int>(bitTanBuffer.type), numValueSize, GL_FLOAT, GL_FALSE, 0);
//}

bool Mesh::CheckBufferExistence(eVertexType type)
{
	for (size_t i = 0; i < m_buffers.size(); ++i)
	{
		if (m_buffers[i].type == type)
		{
			return true;
		}
	}
	return false;
}

void Mesh::AddBuffer(eVertexType type, size_t numElements, size_t elementSize, void* data)
{
	// check if vertex buffer already exists

	if (!CheckBufferExistence(type))
	{
		m_buffers.push_back({ type, 0, numElements, elementSize, data });
	}
	glm::vec3* data2 = (glm::vec3*)(GLvoid*)data;
	if(data != nullptr) { data = nullptr; }
}

void Mesh::CalculateNormal(glm::vec3& normal, const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2)
{
	glm::vec3 v1_0 = v1 - v0;
	glm::vec3 v2_0 = v2 - v0;

	normal = glm::cross(v1 - v0, v2 - v0);
	normal = glm::normalize(normal);
}

void Mesh::CalculateTangent(glm::vec3* tangent, glm::vec3* bitTangent, 
	const glm::vec3& pos1, const glm::vec3& pos2, const glm::vec3& pos3,
	const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3)
{
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;  

	float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	 if (tangent != nullptr)
	 {
		 tangent->x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		 tangent->y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		 tangent->z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
		 *tangent = glm::normalize(*tangent);
	 }

	if (bitTangent != nullptr)
	{
		bitTangent->x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitTangent->y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitTangent->z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
		*bitTangent = glm::normalize(*bitTangent); 
	}
	
}