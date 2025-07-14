#include <glad/glad.h>

#include "Renderer.h"
#include "Component/CMaterialRenderer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::Clear(bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	glClear
	(
		(p_colorBuffer ? GL_COLOR_BUFFER_BIT : 0) |
		(p_depthBuffer ? GL_DEPTH_BUFFER_BIT : 0) |
		(p_stencilBuffer ? GL_STENCIL_BUFFER_BIT : 0)
	);
}

void Renderer::Clear(Camera& p_camera, bool p_colorBuffer, bool p_depthBuffer, bool p_stencilBuffer)
{
	GLfloat previousClearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE, previousClearColor);

	/* Clear the screen using the camera clear color */
	const FVector3& cameraClearColor = p_camera.GetClearColor();
	SetClearColor(cameraClearColor.x, cameraClearColor.y, cameraClearColor.z, 1.0f);
	Clear(p_colorBuffer, p_depthBuffer, p_stencilBuffer);

	/* Reset the OpenGL clear color to the previous clear color (Backuped one) */
	SetClearColor(previousClearColor[0], previousClearColor[1], previousClearColor[2], previousClearColor[3]);
}

void Renderer::SetClearColor(float red, float green, float blue, float alpha)
{
	glClearColor(red, green, blue, alpha);
}

void Renderer::SetViewport(uint32_t p_x, uint32_t p_y, uint32_t p_width, uint32_t p_height)
{
	glViewport(p_x, p_y, p_width, p_height);
}

void Renderer::SetStencilAlgorithm(EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask)
{
	glStencilFunc(static_cast<GLenum>(p_algorithm), p_reference, p_mask);
}

void Renderer::SetDepthAlgorithm(EComparaisonAlgorithm p_algorithm)
{
	glDepthFunc(static_cast<GLenum>(p_algorithm));
}

void Renderer::SetRasterizationMode(ERasterizationMode p_rasterizationMode)
{
	glPolygonMode(GL_FRONT_AND_BACK, static_cast<GLenum>(p_rasterizationMode));
}

void Renderer::SetRasterizationLinesWidth(float p_width)
{
	glLineWidth(p_width);
}

void Renderer::SetDepthWriting(bool p_enable)
{
	glDepthMask(p_enable);
}

void Renderer::SetColorWriting(bool p_enable)
{
	glColorMask(p_enable, p_enable, p_enable, p_enable);
}

void Renderer::SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha)
{
	glColorMask(p_enableRed, p_enableGreen, p_enableBlue, p_enableAlpha);
}

void Renderer::SetCapability(ERenderingCapability p_capability, bool p_value)
{
	(p_value ? glEnable : glDisable)(static_cast<GLenum>(p_capability));
}

void Renderer::SetCullFace(ECullFace p_cullFace)
{
	glCullFace(static_cast<GLenum>(p_cullFace));
}

void Renderer::SetStencilMask(uint32_t p_mask)
{
	glStencilMask(p_mask);
}

void Renderer::ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, EPixelDataFormat format, EPixelDataType type, void* data)
{
	glReadPixels(x, y, width, height, static_cast<GLenum>(format), static_cast<GLenum>(type), data);
}

void Renderer::RenderScene(Scene& p_scene, Camera& p_camera, const FVector3& p_cameraPosition)
{
	Drawables drawables = FindAndSortDrawables(p_scene, p_cameraPosition);
	for (const auto& [distance, drawable] : drawables)
		DrawDrawable(drawable);
}

void Renderer::ApplyStateMask(uint8_t p_mask)
{
	if (p_mask != m_state)
	{
		if ((p_mask & 0x01) != (m_state & 0x01))	SetDepthWriting(p_mask & 0x01);
		if ((p_mask & 0x02) != (m_state & 0x02))	SetColorWriting(p_mask & 0x02);
		if ((p_mask & 0x04) != (m_state & 0x04))	SetCapability(ERenderingCapability::BLEND, p_mask & 0x04);
		if ((p_mask & 0x08) != (m_state & 0x08))	SetCapability(ERenderingCapability::CULL_FACE, p_mask & 0x8);
		if ((p_mask & 0x10) != (m_state & 0x10))	SetCapability(ERenderingCapability::DEPTH_TEST, p_mask & 0x10);

		if ((p_mask & 0x08) && ((p_mask & 0x20) != (m_state & 0x20) || (p_mask & 0x40) != (m_state & 0x40)))
		{
			int backBit = p_mask & 0x20;
			int frontBit = p_mask & 0x40;
			SetCullFace(backBit && frontBit ? ECullFace::FRONT_AND_BACK : (backBit ? ECullFace::BACK : ECullFace::FRONT));
		}

		m_state = p_mask;
	}
}

Renderer::Drawables Renderer::FindAndSortDrawables(const Scene& p_scene, const FVector3& p_cameraPosition)
{
	Renderer::Drawables opaqueDrawables;

	for (Component::CModelRenderer* modelRenderer : p_scene.GetFastAccessComponents().modelRenderers)
	{
		if (auto model = modelRenderer->GetModel())
		{
			float distanceToActor = FVector3::Distance(modelRenderer->owner.transform.GetWorldPosition(), p_cameraPosition);
			if (auto materialRenderer = modelRenderer->owner.GetComponent<Component::CMaterialRenderer>())
			{
				const auto& transform = modelRenderer->owner.transform.GetFTransform();
				const Component::CMaterialRenderer::MaterialList& materials = materialRenderer->GetMaterials();

				for (auto mesh : model->GetMeshes())
				{
					Material* material = nullptr;

					if (mesh->GetMaterialIndex() < MAX_MATERIAL_COUNT)
					{
						material = materials.at(mesh->GetMaterialIndex());
					}

					if (material)
					{
						Renderer::Drawable element = { transform.GetWorldMatrix(), mesh, material };
						opaqueDrawables.emplace(distanceToActor, element);
					}
				}
			}
		}
	}

	return opaqueDrawables;
}

void Renderer::DrawDrawable(const Drawable& p_toDraw)
{
	DrawMesh(*std::get<1>(p_toDraw), *std::get<2>(p_toDraw), &std::get<0>(p_toDraw));
}

void Renderer::DrawModelWithSingleMaterial(Model& p_model, Material& p_material, FMatrix4 const* p_modelMatrix)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		Material* material = &p_material;

		if (material)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void Renderer::DrawModelWithMaterials(Model& p_model, std::vector<Material*> p_materials, FMatrix4 const* p_modelMatrix)
{
	if (p_modelMatrix)
		m_modelMatrixSender(*p_modelMatrix);

	for (auto mesh : p_model.GetMeshes())
	{
		Material* material = p_materials[mesh->GetMaterialIndex()];
		if (material)
			DrawMesh(*mesh, *material, nullptr);
	}
}

void Renderer::DrawMesh(Mesh& p_mesh, Material& p_material, FMatrix4 const* p_modelMatrix)
{
	if (p_material.HasShader() && p_material.GetGPUInstances() > 0)
	{
		if (p_modelMatrix)
			m_modelMatrixSender(*p_modelMatrix);
	
		uint8_t stateMask = p_material.GenerateStateMask();
		ApplyStateMask(stateMask);

		p_material.Bind();
		Draw(p_mesh, EPrimitiveMode::TRIANGLES, p_material.GetGPUInstances());
		p_material.Unbind();
	}
}

void Renderer::Draw(IMesh& p_mesh, EPrimitiveMode p_primitiveMode, uint32_t p_instances)
{
	if (p_instances > 0)
	{
		p_mesh.Bind();
		if (p_mesh.GetIndexCount() > 0)
		{
			if (p_instances == 1)
				glDrawElements(GL_TRIANGLES, p_mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
			else
				glDrawElementsInstanced(static_cast<GLenum>(p_primitiveMode), p_mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, p_instances);
		}
		else
		{
			if (p_instances == 1)
				glDrawArrays(static_cast<GLenum>(p_primitiveMode), 0, p_mesh.GetVertexCount());
			else
				glDrawArraysInstanced(static_cast<GLenum>(p_primitiveMode), 0, p_mesh.GetVertexCount(), p_instances);
		}
		p_mesh.Unbind();
	}
}

void Renderer::RegisterModelMatrixSender(std::function<void(FMatrix4)> p_modelMatrixSender)
{
	m_modelMatrixSender = p_modelMatrixSender;
}

std::vector<FMatrix4> Renderer::FindLightMatrices(const Scene& p_scene)
{
	std::vector<FMatrix4> result;

	const auto& facs = p_scene.GetFastAccessComponents();

	for (auto light : facs.lights)
	{
		result.push_back(light->GetData().GenerateMatrix());
	}

	return result;
}
