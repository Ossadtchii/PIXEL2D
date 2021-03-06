#include "Transform.h"
#include "..//Physics/Collider.h"
#include <iostream>

namespace PIXL
{
	Transform::Transform() : m_needsUpdate(true)
	{
		
	}

	void Transform::Init()
	{
		m_rotation = 0;
		m_position.Set(0, 0);
		m_scale.Set(1.0f, 1.0f);
		m_localMatrix = math::Matrix4x4::Identity();
		m_collider = nullptr;
	}

	const math::Vector2f& Transform::GetPosition() const
	{
		return m_position;
	}

	const float Transform::GetRotation() const
	{
		return m_rotation;
	}

	const math::Vector2f& Transform::GetScale() const
	{
		return m_scale;
	}

	const math::Matrix4x4& Transform::GetLocalTransform() const
	{
		return m_localMatrix;
	}

	const math::Matrix4x4& Transform::GetWorldTransform() const
	{
		return m_worldMatrix;
	}

	void Transform::SetPosition(const math::Vector2f& newPosition)
	{
		m_position.Set(newPosition.x, newPosition.y);
		

		if (m_collider)
		{
			entity->GetComponent<physics::Collider>().SetPositionToSimUnits(math::Vector2f(newPosition.x, newPosition.y));
		}
			
		m_needsUpdate = true;
	}

	void Transform::AddForce(const math::Vector2f& newPosition)
	{

	}

	void Transform::SetRotation(float newRotation)
	{
		m_rotation = newRotation;
		m_needsUpdate = true;
	}

	void Transform::SetScale(const math::Vector2f& newScale)
	{
		m_scale = newScale;
		m_needsUpdate = true;
		
	}


	math::Vector2f Transform::GetUp()
	{
		return m_up;
	}

	math::Vector2f Transform::GetRight()
	{
		return m_right;
	}

	void Transform::Update(Float32 deltaTime)
	{
		if (entity->HasComponent<physics::Collider>() && m_collider == nullptr)
			m_collider = &entity->GetComponent<physics::Collider>();

		if (m_collider != nullptr)
		{
			math::Vector2f simPos = m_collider->GetPositionFromSimUnits();
			m_position.Set(simPos.x, simPos.y);
			m_needsUpdate = true;
		}

		if (m_needsUpdate)
		{
			m_localMatrix =
				math::Matrix4x4::Translation(m_position)
				* math::Matrix4x4::Rotation(m_rotation)
				* math::Matrix4x4::Scaled({ m_scale.x, m_scale.y});

			m_worldMatrix = m_localMatrix;

			if (entity->GetParent() != nullptr)
			{
				m_localMatrix = entity->GetParent()->GetTransform().GetLocalTransform() * m_localMatrix;
				m_worldMatrix = entity->GetParent()->GetTransform().GetWorldTransform() * m_worldMatrix;
			}
			
		//	m_right.Set(m_worldMatrix.elements[0], m_worldMatrix.elements[4], m_worldMatrix.elements[8]);
			m_right.Set(m_worldMatrix.elements[0], m_worldMatrix.elements[4]);
			//m_up.Set(m_worldMatrix.elements[1], m_worldMatrix.elements[5], m_worldMatrix.elements[9]);
			m_up.Set(m_worldMatrix.elements[1], m_worldMatrix.elements[5]);

			std::vector<Entity*>& childs = entity->childs();
			for (size_t i = 0; i < childs.size(); i++)
			{
				childs[i]->GetTransform().SetDirty();
			}

			m_needsUpdate = false;
		}
	}

	bool Transform::IsDirty()
	{
		return m_needsUpdate;
	}

	void Transform::SetDirty()
	{
		m_needsUpdate = true;
	}

}