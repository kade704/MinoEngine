#include "CCamera.h"
#include "../Actor.h"
#include "../GUIDrawer.h"
#include "../Serializer.h"

namespace Component
{
    CCamera::CCamera(Actor& p_owner)
        : AComponent(p_owner)
    {
    }

    CCamera::~CCamera()
    {
    }

    void CCamera::SetFov(float p_value)
    {
        m_camera.SetFov(p_value);
    }

    void CCamera::SetNear(float p_value)
    {
        m_camera.SetNear(p_value);
    }

    void CCamera::SetFar(float p_value)
    {
        m_camera.SetFar(p_value);
    }

    void CCamera::SetClearColor(const FVector3& p_clearColor)
    {
        m_camera.SetClearColor(p_clearColor);
    }

    float CCamera::GetFov() const
    {
        return m_camera.GetFov();
    }

    float CCamera::GetNear() const
    {
        return m_camera.GetNear();
    }

    float CCamera::GetFar() const
    {
        return m_camera.GetFar();
    }

    Camera& CCamera::GetCamera()
    {
        return m_camera;
    }

    const FVector3& CCamera::GetClearColor() const
    {
        return m_camera.GetClearColor();
    }

    void CCamera::OnSerialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
    {
        Serializer::SerializeFloat(p_doc, p_node, "fov", m_camera.GetFov());
        Serializer::SerializeFloat(p_doc, p_node, "near", m_camera.GetNear());
        Serializer::SerializeFloat(p_doc, p_node, "far", m_camera.GetFar());
        Serializer::SerializeVec3(p_doc, p_node, "clear_color", m_camera.GetClearColor());
    }

    void CCamera::OnDeserialize(tinyxml2::XMLDocument& p_doc, tinyxml2::XMLNode* p_node)
    {
        m_camera.SetFov(Serializer::DeserializeFloat(p_doc, p_node, "fov"));
        m_camera.SetNear(Serializer::DeserializeFloat(p_doc, p_node, "near"));
        m_camera.SetFar(Serializer::DeserializeFloat(p_doc, p_node, "far"));
        m_camera.SetClearColor(Serializer::DeserializeVec3(p_doc, p_node, "clear_color"));
    }

    void CCamera::OnInspector(WidgetContainer& p_root)
    {
        GUIDrawer::DrawScalar<float>(p_root, "Field of view", std::bind(&CCamera::GetFov, this), std::bind(&CCamera::SetFov, this, std::placeholders::_1));
        GUIDrawer::DrawScalar<float>(p_root, "Near", std::bind(&CCamera::GetNear, this), std::bind(&CCamera::SetNear, this, std::placeholders::_1));
        GUIDrawer::DrawScalar<float>(p_root, "Far", std::bind(&CCamera::GetFar, this), std::bind(&CCamera::SetFar, this, std::placeholders::_1));
        GUIDrawer::DrawColor(p_root, "Clear color", [this]() {return reinterpret_cast<const Color&>(GetClearColor()); }, [this](Color p_color) { SetClearColor({ p_color.r, p_color.g, p_color.b }); }, false);
    }
}
