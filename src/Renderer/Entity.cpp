#include "pch.h"
#include "Entity.h"

namespace renderer
{
    Entity::Entity(
        const Eigen::Vector3d& position,
        const Eigen::Vector3d& rotation
    ) :
        m_position{ position },
        m_rotation{ rotation },
        m_model{ std::nullopt }
    {
        spdlog::info(
            "Entity without model created @ pos: ({},{},{}), rot: ({},{},{}).",
            m_position.x(), m_position.y(), m_position.z(),
            m_rotation.x(), m_rotation.y(), m_rotation.z());
    }

    Entity::Entity(
        Model&& model,
        const Eigen::Vector3d& position,
        const Eigen::Vector3d& rotation
    ) :
        m_position{ position },
        m_rotation{ rotation },
        m_model{ std::move(model) }
    {
        spdlog::info(
            "Entity with model created @ pos: ({},{},{}), rot: ({},{},{}).",
            m_position.x(), m_position.y(), m_position.z(),
            m_rotation.x(), m_rotation.y(), m_rotation.z());
    }
}
