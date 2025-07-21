#include "ShapeDrawer.h"
#include "Loader/ShaderLoader.h"

ShapeDrawer::ShapeDrawer(Renderer &p_renderer) : m_renderer(p_renderer)
{
    std::vector<Vertex> vertices;
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	});
	vertices.push_back
	({
		0, 0, 0,
		0, 0,
		0, 0, 0,
		0, 0, 0,
		0, 0, 0
	});

	m_lineMesh = new Mesh(vertices, { 0, 1 }, 0);

    std::string vertexShader = R"(
#version 430 core

uniform vec3 start;
uniform vec3 end;
uniform mat4 viewProjection;

void main()
{
	vec3 position = gl_VertexID == 0 ? start : end;
    gl_Position = viewProjection * vec4(position, 1.0);
}

)";

	std::string fragmentShader = R"(
#version 430 core

uniform vec3 color;

out vec4 FRAGMENT_COLOR;

void main()
{
	FRAGMENT_COLOR = vec4(color, 1.0);
}
)";

    m_lineShader = ShaderLoader::CreateFromSource(vertexShader, fragmentShader);
}

ShapeDrawer::~ShapeDrawer()
{
}

void ShapeDrawer::SetViewProjection(const FMatrix4 &p_viewProjection)
{
	m_lineShader->Bind();
	m_lineShader->SetUniformMat4("viewProjection", p_viewProjection);
	m_lineShader->Unbind();
}

void ShapeDrawer::DrawLine(const FVector3 &p_start, const FVector3 &p_end, const FVector3 &p_color, float p_lineWidth)
{
    m_lineShader->Bind();

	m_lineShader->SetUniformVec3("start", p_start);
	m_lineShader->SetUniformVec3("end", p_end);
	m_lineShader->SetUniformVec3("color", p_color);

	m_renderer.SetRasterizationMode(ERasterizationMode::LINE);
	m_renderer.SetRasterizationLinesWidth(p_lineWidth);
	m_renderer.Draw(*m_lineMesh, EPrimitiveMode::LINES);
	m_renderer.SetRasterizationLinesWidth(1.0f);
	m_renderer.SetRasterizationMode(ERasterizationMode::FILL);

	m_lineShader->Unbind();
}
