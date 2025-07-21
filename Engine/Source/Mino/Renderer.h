#pragma once

#include "Scene.h"
#include "Camera.h"
#include "Component/CCamera.h"
#include "Resource/Material.h"
#include "Math/FMatrix4.h"
#include "Resource/Texture.h"

#include "Setting/ERenderingCapability.h"
#include "Setting/EPixelDataFormat.h"
#include "Setting/EPixelDataType.h"
#include "Setting/EPrimitiveMode.h"
#include "Setting/EComparaisonAlgorithm.h"
#include "Setting/ERasterizationMode.h"
#include "Setting/ECullFace.h"
#include "Setting/EOperation.h"

class Renderer
{
public:
	using Drawable = std::tuple<FMatrix4, Mesh*, Material*, FMatrix4>;
	using Drawables = std::multimap<float, Drawable, std::less<float>>;

	Renderer();
	~Renderer();

	void Clear(bool colorBuffer = true, bool depthBuffer = true, bool p_stencilBuffer = true);
	void Clear(Camera& p_camera, bool p_colorBuffer = true, bool p_depthBuffer = true, bool p_stencilBuffer = true);

	void SetClearColor(float red, float green, float blue, float alpha = 1.0f);
	void SetViewport(uint32_t p_x, uint32_t p_y, uint32_t p_width, uint32_t p_height);
	void SetStencilAlgorithm(EComparaisonAlgorithm p_algorithm, int32_t p_reference, uint32_t p_mask);
	void SetDepthAlgorithm(EComparaisonAlgorithm p_algorithm);
	void SetRasterizationMode(ERasterizationMode p_rasterizationMode);
	void SetRasterizationLinesWidth(float p_width);
	void SetDepthWriting(bool p_enable);
	void SetColorWriting(bool p_enable);
	void SetColorWriting(bool p_enableRed, bool p_enableGreen, bool p_enableBlue, bool p_enableAlpha);
	void SetCapability(ERenderingCapability p_capability, bool p_value);
	void SetStencilOperations(EOperation p_stencilFail = EOperation::KEEP, EOperation p_depthFail = EOperation::KEEP, EOperation p_bothPass = EOperation::KEEP);
	void SetCullFace(ECullFace p_cullFace);
	void SetStencilMask(uint32_t p_mask);

	void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, EPixelDataFormat format, EPixelDataType type, void* data);

	bool GetCapability(ERenderingCapability p_capability) const;

	bool GetBool(GLenum p_parameter) const;
	bool GetBool(GLenum p_parameter, uint32_t p_index) const;

	int GetInt(GLenum p_parameter) const;
	int GetInt(GLenum p_parameter, uint32_t p_index) const;

	float GetFloat(GLenum p_parameter) const;
	float GetFloat(GLenum p_parameter, uint32_t p_index) const;

	double GetDouble(GLenum p_parameter) const;
	double GetDouble(GLenum p_parameter, uint32_t p_index) const;

	int64_t GetInt64(GLenum p_parameter) const;
	int64_t GetInt64(GLenum p_parameter, uint32_t p_index) const;

	std::string GetString(GLenum p_parameter) const;
	std::string GetString(GLenum p_parameter, uint32_t p_index) const;

	uint8_t FetchGLState() const;
	void ApplyStateMask(uint8_t p_mask);

	void RenderScene(Scene& p_scene, const Camera& p_camera, const FVector3& p_cameraPosition, Material* p_defaultMaterial = nullptr);

	Component::CCamera* FindMainCamera(const Scene& p_scene);
	Drawables FindAndSortDrawables(const Scene& p_scene, const FVector3& p_cameraPosition, Material* p_defaultMaterial);
	void DrawDrawable(const Drawable& p_toDraw);
	void DrawModelWithSingleMaterial(Model& p_model, Material& p_material, FMatrix4 const* p_modelMatrix, Material* p_defaultMaterial = nullptr);
	void DrawModelWithMaterials(Model& p_model, std::vector<Material*> p_materials, FMatrix4 const* p_modelMatrix, Material* p_defaultMaterial = nullptr);
	void DrawMesh(Mesh& p_mesh, Material& p_material, FMatrix4 const* p_modelMatrix);
	void Draw(IMesh& p_mesh, EPrimitiveMode p_primitiveMode, uint32_t p_instances = 1);

	void RegisterModelMatrixSender(std::function<void(FMatrix4)> p_modelMatrixSender);
	void RegisterUserMatrixSender(std::function<void(FMatrix4)> p_userMatrixSender);

	std::vector<FMatrix4> FindLightMatrices(const Scene& p_scene);

private:
	std::function<void(FMatrix4)> m_modelMatrixSender;
	std::function<void(FMatrix4)> m_userMatrixSender;
	Texture* m_emptyTexture = nullptr;
	uint8_t m_state;
};

