#pragma once

#include <vector>
#include <memory>

#include "IMesh.h"
#include "../Vertex.h"
#include "../Buffer/VertexArray.h"
#include "../Buffer/VertexBuffer.h"
#include "../Buffer/IndexBuffer.h"

class Mesh : public IMesh
{
public:
	Mesh(const std::vector<Vertex>& p_vertices, const std::vector<uint32_t>& p_indices, uint32_t p_materialIndex);
	
	virtual void Bind() override;
	virtual void Unbind() override;
	virtual uint32_t GetVertexCount() override;
	virtual uint32_t GetIndexCount() override;

	uint32_t GetMaterialIndex() const;

private:
	void CreateBuffers(const std::vector<Vertex>& p_vertices, const std::vector<uint32_t>& p_indices);

private:
	const uint32_t m_vertexCount;
	const uint32_t m_indicesCount;
	const uint32_t m_materialIndex;

	Buffer::VertexArray m_vertexArray;
	std::unique_ptr<Buffer::VertexBuffer<float>> m_vertexBuffer;
	std::unique_ptr<Buffer::IndexBuffer> m_indexBuffer;
};

