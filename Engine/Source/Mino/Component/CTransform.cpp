#include "CTransform.h"
#include "../Serializer.h"
#include "../GUIDrawer.h"
#include "../imgui/imgui.h"

Component::CTransform::CTransform(Actor& owner) :
	AComponent(owner)
{
    m_transform.GenerateMatrices(FVector3::Zero, FQuaternion::Identity, FVector3::One);
}

void Component::CTransform::SetParent(CTransform& parent)
{
    m_transform.SetParent(parent.GetFTransform());
}

bool Component::CTransform::HasParent() const
{
    return m_transform.HasParent();
}

void Component::CTransform::RemoveParent()
{
    m_transform.RemoveParent();
}

void Component::CTransform::SetLocalPosition(const FVector3& pos)
{
    m_transform.SetLocalPosition(pos);
}

void Component::CTransform::SetLocalRotation(const FQuaternion& rot)
{
    m_transform.SetLocalRotation(rot);
}

void Component::CTransform::SetLocalScale(const FVector3& scale)
{
    m_transform.SetLocalScale(scale);
}

const FVector3& Component::CTransform::GetLocalPosition() const
{
    return m_transform.GetLocalPosition();
}

const FQuaternion& Component::CTransform::GetLocalRotation() const
{
    return m_transform.GetLocalRotation();
}

const FVector3& Component::CTransform::GetLocalScale() const
{
    return m_transform.GetLocalScale();
}

const FVector3& Component::CTransform::GetWorldPosition() const
{
    return m_transform.GetWorldPosition();
}

const FQuaternion& Component::CTransform::GetWorldRotation() const
{
    return m_transform.GetWorldRotation();
}

const FVector3& Component::CTransform::GetWorldScale() const
{
    return m_transform.GetWorldScale();
}

const FMatrix4& Component::CTransform::GetWorldMatrix() const
{
    return m_transform.GetWorldMatrix();
}

FTransform& Component::CTransform::GetFTransform()
{
    return m_transform;
}

FVector3 Component::CTransform::GetWorldForward() const
{
    return m_transform.GetWorldForward();
}

FVector3 Component::CTransform::GetWorldUp() const
{
    return m_transform.GetWorldUp();
}

FVector3 Component::CTransform::GetWorldRight() const
{
    return m_transform.GetWorldRight();
}

FVector3 Component::CTransform::GetLocalForward() const
{
    return m_transform.GetLocalForward();
}

FVector3 Component::CTransform::GetLocalUp() const
{
    return m_transform.GetLocalUp();
}

FVector3 Component::CTransform::GetLocalRight() const
{
    return m_transform.GetLocalRight();
}

void Component::CTransform::OnSerialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
    Serializer::SerializeVec3(doc, node, "position", GetLocalPosition());
    Serializer::SerializeQuat(doc, node, "rotation", GetLocalRotation());
    Serializer::SerializeVec3(doc, node, "scale", GetLocalScale());
}

void Component::CTransform::OnDeserialize(tinyxml2::XMLDocument& doc, tinyxml2::XMLNode* node)
{
    m_transform.GenerateMatrices
    (
        Serializer::DeserializeVec3(doc, node, "position"),
        Serializer::DeserializeQuat(doc, node, "rotation"),
        Serializer::DeserializeVec3(doc, node, "scale")
    );
}

void Component::CTransform::OnInspector(WidgetContainer& p_root)
{
    auto getRotation = [this]
    {
        return FQuaternion::EulerAngles(GetLocalRotation());
    };

    auto setRotation = [this](FVector3 result)
    {
        SetLocalRotation(FQuaternion(result));
    };

    GUIDrawer::DrawVec3(p_root, "Position", std::bind(&CTransform::GetLocalPosition, this), std::bind(&CTransform::SetLocalPosition, this, std::placeholders::_1), 0.05f);
    GUIDrawer::DrawVec3(p_root, "Rotation", getRotation, setRotation, 0.05f);
    GUIDrawer::DrawVec3(p_root, "Scale", std::bind(&CTransform::GetLocalScale, this), std::bind(&CTransform::SetLocalScale, this, std::placeholders::_1), 0.05f, 0.0001f);
}