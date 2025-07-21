#include "EditorResource.h"

#include <Mino/Loader/TextureLoader.h>
#include <Mino/Loader/ModelLoader.h>
#include <Mino/Loader/ShaderLoader.h>
#include <Mino/PathParser.h>

EditorResource::EditorResource()
{
	EModelParserFlags modelParserFlags = EModelParserFlags::NONE;
	modelParserFlags |= EModelParserFlags::TRIANGULATE;
	modelParserFlags |= EModelParserFlags::GEN_SMOOTH_NORMALS;
	modelParserFlags |= EModelParserFlags::OPTIMIZE_MESHES;
	modelParserFlags |= EModelParserFlags::FIND_INSTANCES;
	modelParserFlags |= EModelParserFlags::CALC_TANGENT_SPACE;
	modelParserFlags |= EModelParserFlags::JOIN_IDENTICAL_VERTICES;
	modelParserFlags |= EModelParserFlags::DEBONE;
	modelParserFlags |= EModelParserFlags::FIND_INVALID_DATA;
	modelParserFlags |= EModelParserFlags::IMPROVE_CACHE_LOCALITY;
	modelParserFlags |= EModelParserFlags::GEN_UV_COORDS;
	modelParserFlags |= EModelParserFlags::PRE_TRANSFORM_VERTICES;
	modelParserFlags |= EModelParserFlags::GLOBAL_SCALE;

	ETextureFilteringMode firstFilterEditor = ETextureFilteringMode::LINEAR;
	ETextureFilteringMode secondFilterEditor = ETextureFilteringMode::LINEAR;

	ETextureFilteringMode firstFilterBillboard = ETextureFilteringMode::NEAREST;
	ETextureFilteringMode secondFilterBillboard = ETextureFilteringMode::NEAREST;

	m_textures["Button_Play"] = TextureLoader::Create("Resource/Texture/button_play.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Pause"] = TextureLoader::Create("Resource/Texture/button_pause.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Stop"] = TextureLoader::Create("Resource/Texture/button_stop.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Next"] = TextureLoader::Create("Resource/Texture/button_next.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Refresh"] = TextureLoader::Create("Resource/Texture/button_refresh.png", firstFilterEditor, secondFilterEditor, false);

	m_textures["Icon_Folder"] = TextureLoader::Create("Resource/Texture/icon_folder.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Texture"] = TextureLoader::Create("Resource/Texture/icon_texture.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Model"] = TextureLoader::Create("Resource/Texture/icon_model.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Shader"] = TextureLoader::Create("Resource/Texture/icon_shader.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Material"] = TextureLoader::Create("Resource/Texture/icon_material.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Scene"] = TextureLoader::Create("Resource/Texture/icon_scene.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Script"] = TextureLoader::Create("Resource/Texture/icon_script.png", firstFilterEditor, secondFilterEditor, false);

	m_textures["Bill_Point_Light"] = TextureLoader::Create("Resource/Texture/icon_light_point.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Spot_Light"] = TextureLoader::Create("Resource/Texture/icon_light_spot.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Directional_Light"] = TextureLoader::Create("Resource/Texture/icon_light_directional.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Ambient_Box_Light"] = TextureLoader::Create("Resource/Texture/icon_light_ambient_box.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Ambient_Sphere_Light"] = TextureLoader::Create("Resource/Texture/icon_light_ambient_sphere.png", firstFilterBillboard, secondFilterBillboard, false);

	m_textures["Empty_Texture"] = TextureLoader::Create("Resource/Texture/empty_texture.png", firstFilterEditor, secondFilterEditor, false);

	m_models["Arrow_Translate"] = ModelLoader::Create("Resource/Model/Arrow_Translate.fbx", modelParserFlags);
	m_models["Arrow_Rotate"] = ModelLoader::Create("Resource/Model/Arrow_Rotate.fbx", modelParserFlags);
	m_models["Arrow_Scale"] = ModelLoader::Create("Resource/Model/Arrow_Scale.fbx", modelParserFlags);
	m_models["Arrow_Picking"] = ModelLoader::Create("Resource/Model/Arrow_Picking.fbx", modelParserFlags);
	m_models["Vertical_Plane"] = ModelLoader::Create("Resource/Model/Vertical_Plane.fbx", modelParserFlags);
	m_models["Camera"] = ModelLoader::Create("Resource/Model/Camera.fbx", modelParserFlags);
	m_models["Sphere"] = ModelLoader::Create("Resource/Model/Sphere.fbx", modelParserFlags);
	m_models["Plane"] = ModelLoader::Create("Resource/Model/Plane.fbx", modelParserFlags);

	m_shaders["Standard"] = ShaderLoader::Create("Resource/Shader/standard.shader");
	m_shaders["Gizmo"] = ShaderLoader::Create("Resource/Shader/gizmo.shader");
	m_shaders["Billboard"] = ShaderLoader::Create("Resource/Shader/billboard.shader");
	m_shaders["Grid"] = ShaderLoader::Create("Resource/Shader/grid.shader");
	m_shaders["Lambert"] = ShaderLoader::Create("Resource/Shader/lambert.shader");
	m_shaders["Unlit"] = ShaderLoader::Create("Resource/Shader/unlit.shader");
}

EditorResource::~EditorResource()
{
	for (auto[id, texture] : m_textures)
		TextureLoader::Destroy(texture);

	for (auto [id, mesh] : m_models)
		ModelLoader::Destroy(mesh);

	for (auto [id, shader] : m_shaders)
		ShaderLoader::Destroy(shader);
}

Texture* EditorResource::GetFileIcon(const std::string& p_filename)
{
	return GetTexture("Icon_" + PathParser::FileTypeToString(PathParser::GetFileType(p_filename)));
}

Texture* EditorResource::GetTexture(const std::string& p_id)
{
	if (m_textures.find(p_id) != m_textures.end())
		return m_textures.at(p_id);

	return nullptr;
}

Model* EditorResource::GetModel(const std::string& p_id)
{
	if (m_models.find(p_id) != m_models.end())
		return m_models.at(p_id);

	return nullptr;
}

Shader* EditorResource::GetShader(const std::string& p_id)
{
	if (m_shaders.find(p_id) != m_shaders.end())
		return m_shaders.at(p_id);

	return nullptr;
}
