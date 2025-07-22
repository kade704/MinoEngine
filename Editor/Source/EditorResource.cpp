#include "EditorResource.h"

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

	m_textures["Button_Play"] = Texture::Create("Resource/Texture/button_play.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Pause"] = Texture::Create("Resource/Texture/button_pause.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Stop"] = Texture::Create("Resource/Texture/button_stop.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Next"] = Texture::Create("Resource/Texture/button_next.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Button_Refresh"] = Texture::Create("Resource/Texture/button_refresh.png", firstFilterEditor, secondFilterEditor, false);

	m_textures["Icon_Folder"] = Texture::Create("Resource/Texture/icon_folder.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Texture"] = Texture::Create("Resource/Texture/icon_texture.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Model"] = Texture::Create("Resource/Texture/icon_model.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Shader"] = Texture::Create("Resource/Texture/icon_shader.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Material"] = Texture::Create("Resource/Texture/icon_material.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Scene"] = Texture::Create("Resource/Texture/icon_scene.png", firstFilterEditor, secondFilterEditor, false);
	m_textures["Icon_Script"] = Texture::Create("Resource/Texture/icon_script.png", firstFilterEditor, secondFilterEditor, false);

	m_textures["Bill_Point_Light"] = Texture::Create("Resource/Texture/icon_light_point.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Spot_Light"] = Texture::Create("Resource/Texture/icon_light_spot.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Directional_Light"] = Texture::Create("Resource/Texture/icon_light_directional.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Ambient_Box_Light"] = Texture::Create("Resource/Texture/icon_light_ambient_box.png", firstFilterBillboard, secondFilterBillboard, false);
	m_textures["Bill_Ambient_Sphere_Light"] = Texture::Create("Resource/Texture/icon_light_ambient_sphere.png", firstFilterBillboard, secondFilterBillboard, false);

	m_textures["Empty_Texture"] = Texture::Create("Resource/Texture/empty_texture.png", firstFilterEditor, secondFilterEditor, false);

	m_models["Arrow_Translate"] = Model::Create("Resource/Model/Arrow_Translate.fbx", modelParserFlags);
	m_models["Arrow_Rotate"] = Model::Create("Resource/Model/Arrow_Rotate.fbx", modelParserFlags);
	m_models["Arrow_Scale"] = Model::Create("Resource/Model/Arrow_Scale.fbx", modelParserFlags);
	m_models["Arrow_Picking"] = Model::Create("Resource/Model/Arrow_Picking.fbx", modelParserFlags);
	m_models["Vertical_Plane"] = Model::Create("Resource/Model/Vertical_Plane.fbx", modelParserFlags);
	m_models["Camera"] = Model::Create("Resource/Model/Camera.fbx", modelParserFlags);
	m_models["Sphere"] = Model::Create("Resource/Model/Sphere.fbx", modelParserFlags);
	m_models["Plane"] = Model::Create("Resource/Model/Plane.fbx", modelParserFlags);

	m_shaders["Standard"] = Shader::Create("Resource/Shader/standard.shader");
	m_shaders["Gizmo"] = Shader::Create("Resource/Shader/gizmo.shader");
	m_shaders["Billboard"] = Shader::Create("Resource/Shader/billboard.shader");
	m_shaders["Grid"] = Shader::Create("Resource/Shader/grid.shader");
	m_shaders["Lambert"] = Shader::Create("Resource/Shader/lambert.shader");
	m_shaders["Unlit"] = Shader::Create("Resource/Shader/unlit.shader");
}

EditorResource::~EditorResource()
{
	for (auto[id, texture] : m_textures)
		Texture::Destroy(texture);

	for (auto [id, mesh] : m_models)
		Model::Destroy(mesh);

	for (auto [id, shader] : m_shaders)
		Shader::Destroy(shader);
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
