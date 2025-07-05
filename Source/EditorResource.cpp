#include "EditorResource.h"
#include "Loader/TextureLoader.h"
#include "Loader/ModelLoader.h"
#include "Loader/ShaderLoader.h"
#include "PathParser.h"

EditorResource::EditorResource()
{
	m_textures["Button_Play"] = TextureLoader::Create("Contents/editor/button_play.png");
	m_textures["Button_Pause"] = TextureLoader::Create("Contents/editor/button_pause.png");
	m_textures["Button_Stop"] = TextureLoader::Create("Contents/editor/button_stop.png");
	m_textures["Button_Next"] = TextureLoader::Create("Contents/editor/button_next.png");
	m_textures["Button_Refresh"] = TextureLoader::Create("Contents/editor/button_refresh.png");

	m_textures["Icon_Folder"] = TextureLoader::Create("Contents/editor/icon_folder.png");
	m_textures["Icon_Texture"] = TextureLoader::Create("Contents/editor/icon_texture.png");
	m_textures["Icon_Model"] = TextureLoader::Create("Contents/editor/icon_model.png");
	m_textures["Icon_Shader"] = TextureLoader::Create("Contents/editor/icon_shader.png");
	m_textures["Icon_Material"] = TextureLoader::Create("Contents/editor/icon_material.png");
	m_textures["Icon_Scene"] = TextureLoader::Create("Contents/editor/icon_scene.png");
	m_textures["Icon_Script"] = TextureLoader::Create("Contents/editor/button_play.png");

	m_textures["Bill_Point_Light"] = TextureLoader::Create("Contents/editor/icon_light_point.png");
	m_textures["Bill_Spot_Light"] = TextureLoader::Create("Contents/editor/icon_light_spot.png");
	m_textures["Bill_Directional_Light"] = TextureLoader::Create("Contents/editor/icon_light_directional.png");
	m_textures["Bill_Ambient_Box_Light"] = TextureLoader::Create("Contents/editor/icon_light_ambient_box.png");
	m_textures["Bill_Ambient_Sphere_Light"] = TextureLoader::Create("Contents/editor/icon_light_ambient_sphere.png");

	m_models["Arrow_Translate"] = ModelLoader::Create("Contents/editor/Arrow_Translate.fbx");
	m_models["Arrow_Rotate"] = ModelLoader::Create("Contents/editor/Arrow_Rotate.fbx");
	m_models["Arrow_Scale"] = ModelLoader::Create("Contents/editor/Arrow_Scale.fbx");
	m_models["Vertical_Plane"] = ModelLoader::Create("Contents/editor/Vertical_Plane.fbx");

	m_shaders["Gizmo"] = ShaderLoader::Create("Contents/editor/gizmo.shader");
	m_shaders["Billboard"] = ShaderLoader::Create("Contents/editor/billboard.shader");
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
