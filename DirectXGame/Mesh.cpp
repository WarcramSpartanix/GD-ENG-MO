#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "GraphicsEngine.h"
#include "VertexMesh.h"

#include "Quaternion.h"


Mesh::Mesh(const wchar_t* fullPath) : Resource(fullPath)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	
	std::string warn;
	std::string err;

	std::wstring filename = (std::wstring) fullPath;
	std::string inputFile(filename.begin(), filename.end());
	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warn, &err, inputFile.c_str());

	if (!err.empty())
	{
		throw std::exception("Mesh not created successfully");
	}
	if (!res)
	{
		throw std::exception("Mesh not created successfully");
	}
	if (shapes.size() > 1)
	{
		throw std::exception("Mesh not created successfully");
	}

	std::vector<unsigned int> list_indices;
	tinyobj::real_t highestTx = 0;
	tinyobj::real_t highestTy = 0;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t index_offset = 0;
		list_vertices.reserve(shapes[s].mesh.indices.size());
		list_indices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			unsigned char num_face_verts = shapes[s].mesh.num_face_vertices[f];

			for (unsigned char v = 0; v < num_face_verts; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				if (!attribs.texcoords.empty())
				{
					tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
					tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];

					if (tx > highestTx)
						highestTx = tx;
					if (ty > highestTy)
						highestTy = ty;

					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(tx, ty));
					list_vertices.push_back(vertex);
				}
				else
				{
					VertexMesh vertex(Vector3D(vx, vy, vz), Vector2D(0, 0));
					list_vertices.push_back(vertex);
				}

				list_indices.push_back((unsigned int)index_offset + v);
			}

			index_offset += num_face_verts;
		}
	}

	if (highestTx >= highestTy)
	{
		for (int i = 0; i < list_vertices.size(); i++)
		{
			list_vertices[i].m_texcoord = list_vertices[i].m_texcoord * (1.0f / highestTx);
		}
	}
	else
	{
		for (int i = 0; i < list_vertices.size(); i++)
		{
			list_vertices[i].m_texcoord = list_vertices[i].m_texcoord * (1.0f / highestTy);
		}
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::getInstance()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::getInstance()->getRenderSystem()->createVertexBuffer(&list_vertices[0], sizeof(VertexMesh), 
		(UINT)list_vertices.size(), shader_byte_code, size_shader);
	m_index_buffer = GraphicsEngine::getInstance()->getRenderSystem()->createIndexBuffer(&list_indices[0], (UINT)list_indices.size());
}

Mesh::~Mesh()
{
	delete m_vertex_buffer;
	delete m_index_buffer;
}

VertexBuffer* Mesh::getVertexBuffer()
{
	return m_vertex_buffer;
}

IndexBuffer* Mesh::getIndexBuffer()
{
	return m_index_buffer;
}

void Mesh::scaleVertexLocations(Vector3D scale)
{
	this->scale = scale;
	
	updateVertexLocations();
}

void Mesh::rotateVertexLocations(Vector3D rot)
{
	rotation = rot;
	
	updateVertexLocations();
}

void Mesh::moveVertexLocations(Vector3D pos)
{
	position = pos;
	
	updateVertexLocations();
}

void Mesh::updateVertexLocations()
{
	std::vector<VertexMesh> list = list_vertices;

	for (int i = 0; i < list.size(); i++)
	{
		list[i].m_position = Vector3D(list[i].m_position.x * scale.x, list[i].m_position.y * scale.y, list[i].m_position.z * scale.z);
		list[i].m_position = Quaternion::rotatePointEuler(list[i].m_position, rotation);
		list[i].m_position = list[i].m_position + position;
	}

	void* shader_byte_code = nullptr;
	size_t size_shader = 0;
	GraphicsEngine::getInstance()->getVertexMeshLayoutShaderByteCodeAndSize(&shader_byte_code, &size_shader);
	m_vertex_buffer = GraphicsEngine::getInstance()->getRenderSystem()->createVertexBuffer(&list[0], sizeof(VertexMesh),
		(UINT)list.size(), shader_byte_code, size_shader);
}
