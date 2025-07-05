#include "MaterialLoader.h"
#include "../tinyxml2.h"

Material* MaterialLoader::Create(const std::string& p_path)
{
    tinyxml2::XMLDocument doc;
	doc.LoadFile(p_path.c_str());
	if (!doc.Error())
	{
		tinyxml2::XMLNode* root = doc.FirstChild();
		Material* material = new Material();
		material->OnDeserialize(doc, root);

		return material;
	}
	else
	{
		return nullptr;
	}
}

bool MaterialLoader::Destroy(Material*& p_material)
{
    if (p_material)
    {
        delete p_material;
        p_material = nullptr;

        return true;
    }

    return false;
}
