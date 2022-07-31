#pragma once

#include "Model.h"

namespace renderer
{
    struct Entity
    {
        Entity(
            const Eigen::Vector3d& position = { 0., 0., 0. },
            const Eigen::Vector3d& rotation = { 0., 0., 0. }
        );

        Entity(
            Model&& model,
            const Eigen::Vector3d& position = { 0., 0., 0. },
            const Eigen::Vector3d& rotation = { 0., 0., 0. }
        );

    private:
        Eigen::Vector3d m_position;
        Eigen::Vector3d m_rotation;
        const std::optional<Model> m_model;
    };
}