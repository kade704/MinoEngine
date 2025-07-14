#include "SceneLoader.h"
#include "../tinyxml2.h"
#include "../Logger.h"

Scene* SceneLoader::Create(const std::string& path)
{
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());
    if (doc.Error())
    {
        MINO_ERROR("File not found : " + path);
        return nullptr;
    }

    auto root = doc.FirstChild();
    auto scene = new Scene();
    scene->OnDeserialize(doc, root);

    return scene;
}
