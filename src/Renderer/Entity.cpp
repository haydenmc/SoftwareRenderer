#include "pch.h"
#include "Entity.h"

namespace Renderer
{
    Entity::Entity(
        const Eigen::Vector3d& position,
        const Eigen::Vector3d& rotation,
        const Eigen::Vector3d& scale
    ) :
        m_position{ position },
        m_rotation{ rotation },
        m_scale{ scale },
        m_model{ std::nullopt }
    {
        spdlog::info(
            "Entity without model created @ pos: ({},{},{}), rot: ({},{},{}).",
            m_position.x(), m_position.y(), m_position.z(),
            m_rotation.x(), m_rotation.y(), m_rotation.z());
    }

    Entity::Entity(
        Renderer::Model&& model,
        const Eigen::Vector3d& position,
        const Eigen::Vector3d& rotation,
        const Eigen::Vector3d& scale
    ) :
        m_position{ position },
        m_rotation{ rotation },
        m_scale{ scale },
        m_model{ std::move(model) }
    {
        spdlog::info(
            "Entity with model created @ pos: ({},{},{}), rot: ({},{},{}).",
            m_position.x(), m_position.y(), m_position.z(),
            m_rotation.x(), m_rotation.y(), m_rotation.z());
    }

    const Eigen::Vector3d& Entity::Position() const
    {
        return m_position;
    }

    void Entity::Position(const Eigen::Vector3d& value)
    {
        m_position = value;
    }

    const Eigen::Vector3d& Entity::Rotation() const
    {
        return m_rotation;
    }

    void Entity::Rotation(const Eigen::Vector3d& value)
    {
        m_rotation = value;
    }

    const Eigen::Vector3d& Entity::Scale() const
    {
        return m_scale;
    }

    void Entity::Scale(const Eigen::Vector3d& value)
    {
        m_scale = value;
    }

    const std::optional<Model>& Entity::Model() const
    {
        return m_model;
    }
}
