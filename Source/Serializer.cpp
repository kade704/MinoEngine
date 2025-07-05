#include "Serializer.h"
#include "Loader/ShaderLoader.h"
#include "Manager/TextureManager.h"
#include "Manager/ModelManager.h"
#include "Manager/ShaderManager.h"
#include "ServiceLocator.h"

void Serializer::SerializeString(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const std::string& p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    element->SetText(p_value.c_str());
    p_node->InsertEndChild(element);
}

void Serializer::SerializeInt64(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, int64_t p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    element->SetText(p_value);
    p_node->InsertEndChild(element);
}

void Serializer::SerializeFloat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, float p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    element->SetText(p_value);
    p_node->InsertEndChild(element);
}

void Serializer::SerializeVec2(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const FVector2& p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    p_node->InsertEndChild(element);

    auto x = p_doc.NewElement("x");
    x->SetText(p_value.x);
    element->InsertEndChild(x);

    auto y = p_doc.NewElement("y");
    y->SetText(p_value.y);
    element->InsertEndChild(y);
}

void Serializer::SerializeVec3(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const FVector3& p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    p_node->InsertEndChild(element);

    auto x = p_doc.NewElement("x");
    x->SetText(p_value.x);
    element->InsertEndChild(x);

    auto y = p_doc.NewElement("y");
    y->SetText(p_value.y);
    element->InsertEndChild(y);

    auto z = p_doc.NewElement("z");
    z->SetText(p_value.z);
    element->InsertEndChild(z);
}

void Serializer::SerializeVec4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const FVector4& p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    p_node->InsertEndChild(element);

    auto x = p_doc.NewElement("x");
    x->SetText(p_value.x);
    element->InsertEndChild(x);

    auto y = p_doc.NewElement("y");
    y->SetText(p_value.y);
    element->InsertEndChild(y);

    auto z = p_doc.NewElement("z");
    z->SetText(p_value.z);
    element->InsertEndChild(z);

    auto w = p_doc.NewElement("w");
    w->SetText(p_value.w);
    element->InsertEndChild(w);
}

void Serializer::SerializeMat4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const FMatrix4& p_value)
{
}

void Serializer::SerializeQuat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const FQuaternion& p_value)
{
    auto element = p_doc.NewElement(p_name.c_str());
    p_node->InsertEndChild(element);

    auto x = p_doc.NewElement("x");
    x->SetText(p_value.x);
    element->InsertEndChild(x);

    auto y = p_doc.NewElement("y");
    y->SetText(p_value.y);
    element->InsertEndChild(y);

    auto z = p_doc.NewElement("z");
    z->SetText(p_value.z);
    element->InsertEndChild(z);

    auto w = p_doc.NewElement("w");
    w->SetText(p_value.w);
    element->InsertEndChild(w);
}

void Serializer::SerializeModel(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const Model* p_value)
{
    SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void Serializer::SerializeMaterial(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, const Material* p_value)
{
    SerializeString(p_doc, p_node, p_name.c_str(), p_value ? p_value->path : "?");
}

void Serializer::DeserializeString(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, std::string& p_out)
{
    if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
    {
        const char* result = element->GetText();
        p_out = result ? result : "";
    }
}

void Serializer::DeserializeInt64(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, int64_t& p_out)
{
    if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
        element->QueryInt64Text(&p_out);
}

void Serializer::DeserializeFloat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, float& p_out)
{
    if (auto element = p_node->FirstChildElement(p_name.c_str()); element)
        element->QueryFloatText(&p_out);
}

void Serializer::DeserializeVec2(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, FVector2& p_out)
{
    if (auto child = p_node->FirstChildElement(p_name.c_str()); p_node)
    {
        if (auto element = child->FirstChildElement("x"); element)
            element->QueryFloatText(&p_out.x);

        if (auto element = child->FirstChildElement("y"); element)
            element->QueryFloatText(&p_out.y);
    }
}

void Serializer::DeserializeVec3(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, FVector3& p_out)
{
    if (auto child = p_node->FirstChildElement(p_name.c_str()); p_node)
    {
        if (auto element = child->FirstChildElement("x"); element)
            element->QueryFloatText(&p_out.x);

        if (auto element = child->FirstChildElement("y"); element)
            element->QueryFloatText(&p_out.y);

        if (auto element = child->FirstChildElement("z"); element)
            element->QueryFloatText(&p_out.z);
    }
}

void Serializer::DeserializeVec4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, FVector4& p_out)
{
    if (auto child = p_node->FirstChildElement(p_name.c_str()); p_node)
    {
        if (auto element = child->FirstChildElement("x"); element)
            element->QueryFloatText(&p_out.x);

        if (auto element = child->FirstChildElement("y"); element)
            element->QueryFloatText(&p_out.y);

        if (auto element = child->FirstChildElement("z"); element)
            element->QueryFloatText(&p_out.z);

        if (auto element = child->FirstChildElement("w"); element)
            element->QueryFloatText(&p_out.z);
    }
}

void Serializer::DeserializeMat4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, FMatrix4& p_out)
{
    
}

void Serializer::DeserializeQuat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, FQuaternion& p_out)
{
    if (auto child = p_node->FirstChildElement(p_name.c_str()); p_node)
    {
        if (auto element = child->FirstChildElement("x"); element)
            element->QueryFloatText(&p_out.x);

        if (auto element = child->FirstChildElement("y"); element)
            element->QueryFloatText(&p_out.y);

        if (auto element = child->FirstChildElement("z"); element)
            element->QueryFloatText(&p_out.z);

        if (auto element = child->FirstChildElement("w"); element)
            element->QueryFloatText(&p_out.w);
    }
}

void Serializer::DeserializeTexture(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, Texture*& p_out)
{
    if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
        p_out = ServiceLocator::Get<TextureManager>().GetResource(path);
    else
        p_out = nullptr;
}

void Serializer::DeserializeShader(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, Shader*& p_out)
{
    if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
        p_out = ServiceLocator::Get<ShaderManager>().GetResource(path);
    else
        p_out = nullptr;
}

void Serializer::DeserializeModel(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name, Model*& p_out)
{
    if (std::string path = DeserializeString(p_doc, p_node, p_name.c_str()); path != "?" && path != "")
        p_out = ServiceLocator::Get<ModelManager>().GetResource(path);
    else
        p_out = nullptr;
}

std::string Serializer::DeserializeString(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    std::string result;
    DeserializeString(p_doc, p_node, p_name, result);
    return result;
}

int64_t Serializer::DeserializeInt64(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    int64_t result;
    DeserializeInt64(p_doc, p_node, p_name, result);
    return result;
}

float Serializer::DeserializeFloat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    float result;
    DeserializeFloat(p_doc, p_node, p_name, result);
    return result;
}

FVector2 Serializer::DeserializeVec2(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    FVector2 result;
    DeserializeVec2(p_doc, p_node, p_name, result);
    return result;
}

FVector3 Serializer::DeserializeVec3(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    FVector3 result;
    DeserializeVec3(p_doc, p_node, p_name, result);
    return result;
}

FVector4 Serializer::DeserializeVec4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    FVector4 result;
    DeserializeVec4(p_doc, p_node, p_name, result);
    return result;
}

FMatrix4 Serializer::DeserializeMat4(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    FMatrix4 result;
    DeserializeMat4(p_doc, p_node, p_name, result);
    return result;
}

FQuaternion Serializer::DeserializeQuat(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    FQuaternion result;
    DeserializeQuat(p_doc, p_node, p_name, result);
    return result;
}

Texture* Serializer::DeserializeTexture(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    Texture* result = nullptr;
    DeserializeTexture(p_doc, p_node, p_name, result);
    return result;
}

Shader* Serializer::DeserializeShader(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    Shader* result = nullptr;
    DeserializeShader(p_doc, p_node, p_name, result);
    return result;
}

Model* Serializer::DeserializeModel(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node, const std::string& p_name)
{
    Model* result;
    DeserializeModel(p_doc, p_node, p_name, result);
    return result;
}
