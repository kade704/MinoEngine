#pragma once

#include "Scene.h"
#include "Camera.h"
#include "CameraController.h"
#include "Resource/Material.h"
#include "Math/FMatrix4.h"

enum class ERasterizationMode
{
	POINT = 0x1B00,
	LINE = 0x1B01, 
	FILL = 0x1B02
};

enum class EComparaisonAlgorithm
{
	NEVER = 0x0200,
	LESS = 0x0201,
	EQUAL = 0x0202,
	LESS_EQUAL = 0x0203,
	GREATER = 0x0204,
	NOTEQUAL = 0x0205,
	GREATER_EQUAL = 0x0206,
	ALWAYS = 0x0207
};

enum class EPrimitiveMode
{
	POINTS = 0x0000,
	LINES = 0x0001,
	LINE_LOOP = 0x0002,
	LINE_STRIP = 0x0003,
	TRIANGLES = 0x0004,
	TRIANGLE_STRIP = 0x0005,
	TRIANGLE_FAN = 0x0006,
	LINES_ADJACENCY = 0x000A,
	LINE_STRIP_ADJACENCY = 0x000B,
	TRIANGLES_ADJACENCY = 0x000C,
	TRIANGLE_STRIP_ADJACENCY = 0x000D,
	PATCHES = 0xE,
};

enum class EPixelDataFormat
{
	COLOR_INDEX = 0x1900,
	STENCIL_INDEX = 0x1901,
	DEPTH_COMPONENT = 0x1902,
	RED = 0x1903,
	GREEN = 0x1904,
	BLUE = 0x1905,
	ALPHA = 0x1906,
	RGB = 0x1907,
	BGR = 0x80E0,
	RGBA = 0x1908,
	BGRA = 0x80E1,
	LUMINANCE = 0x1909,
	LUMINANCE_ALPHA = 0x190A,
};

enum class EPixelDataType
{
	BYTE = 0x1400,
	UNSIGNED_BYTE = 0x1401,
	BITMAP = 0x1A00,
	SHORT = 0x1402,
	UNSIGNED_SHORT = 0x1403,
	INT = 0x1404,
	UNSIGNED_INT = 0x1405,
	FLOAT = 0x1406,
	UNSIGNED_BYTE_3_3_2 = 0x8032,
	UNSIGNED_BYTE_2_3_3_REV = 0x8362,
	UNSIGNED_SHORT_5_6_5 = 0x8363,
	UNSIGNED_SHORT_5_6_5_REV = 0x8364,
	UNSIGNED_SHORT_4_4_4_4 = 0x8033,
	UNSIGNED_SHORT_4_4_4_4_REV = 0x8365,
	UNSIGNED_SHORT_5_5_5_1 = 0x8034,
	UNSIGNED_SHORT_1_5_5_5_REV = 0x8366,
	UNSIGNED_INT_8_8_8_8 = 0x8035,
	UNSIGNED_INT_8_8_8_8_REV = 0x8367,
	UNSIGNED_INT_10_10_10_2 = 0x8036,
	UNSIGNED_INT_2_10_10_10_REV = 0x8368
};

enum class ERenderingCapability
{
	BLEND = 0x0BE2, // If enabled, blend the computed fragment color values with the values in the color buffers. See glBlendFunc.
	CULL_FACE = 0x0B44, // If enabled, cull polygons based on their winding in window coordinates.
	DEPTH_TEST = 0x0B71, // If enabled, do depth comparisons and update the depth buffer. Note that even if the depth buffer exists and the depth mask is non-zero, the depth buffer is not updated if the depth test is disabled.
	DITHER = 0x0BD0, // If enabled, dither color components or indices before they are written to the color buffer.
	POLYGON_OFFSET_FILL = 0x8037, // If enabled, an offset is added to depth values of a polygon's fragments produced by rasterization.
	SAMPLE_ALPHA_TO_COVERAGE = 0x809E, // If enabled, compute a temporary coverage value where each bit is determined by the alpha value at the corresponding sample location. The temporary coverage value is then ANDed with the fragment coverage value.
	SAMPLE_COVERAGE = 0x80A0, // If enabled, the fragment's coverage is ANDed with the temporary coverage value. If GL_SAMPLE_COVERAGE_INVERT is set to GL_TRUE, invert the coverage value.
	SCISSOR_TEST = 0x0C11, // If enabled, discard fragments that are outside the scissor rectangle.
	STENCIL_TEST = 0x0B90, // If enabled, do stencil testing and update the stencil buffer.
	MULTISAMPLE = 0x809D  // If enabled, use multiple fragment samples in computing the final color of a pixel.	
};

enum class ECullFace
{
	FRONT = 0x0404,
	BACK = 0x0405,
	FRONT_AND_BACK = 0x0408
};

class Renderer
{
public:
	using Drawable = std::tuple<FMatrix4, Mesh*, Material*>;
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
	void SetCullFace(ECullFace p_cullFace);
	void SetStencilMask(uint32_t p_mask);
	void ReadPixels(uint32_t x, uint32_t y, uint32_t width, uint32_t height, EPixelDataFormat format, EPixelDataType type, void* data);
	void RenderScene(Scene& p_scene, Camera& p_camera, const FVector3& p_cameraPosition);
	void ApplyStateMask(uint8_t p_mask);

	Drawables FindAndSortDrawables(const Scene& p_scene, const FVector3& p_cameraPosition);
	void DrawDrawable(const Drawable& p_toDraw);
	void DrawModelWithSingleMaterial(Model& p_model, Material& p_material, FMatrix4 const* p_modelMatrix);
	void DrawModelWithMaterials(Model& p_model, std::vector<Material*> p_materials, FMatrix4 const* p_modelMatrix);
	void DrawMesh(Mesh& p_mesh, Material& p_material, FMatrix4 const* p_modelMatrix);
	void Draw(IMesh& p_mesh, EPrimitiveMode p_primitiveMode, uint32_t p_instances);

	void RegisterModelMatrixSender(std::function<void(FMatrix4)> p_modelMatrixSender);

	std::vector<FMatrix4> FindLightMatrices(const Scene& p_scene);

private:
	std::function<void(FMatrix4)> m_modelMatrixSender;
	uint8_t m_state;
};

