#pragma once

#include "Model.h"

namespace Renderer
{
    struct Entity
    {
        Entity(
            const Eigen::Vector3d& position = { 0., 0., 0. },
            const Eigen::Vector3d& rotation = { 0., 0., 0. },
            const Eigen::Vector3d& scale = { 0., 0., 0. }
        );

        Entity(
            Model&& model,
            const Eigen::Vector3d& position = { 0., 0., 0. },
            const Eigen::Vector3d& rotation = { 0., 0., 0. },
            const Eigen::Vector3d& scale = { 0., 0., 0. }
        );

        const Eigen::Vector3d& Position() const;
        void Position(const Eigen::Vector3d& value);
        const Eigen::Vector3d& Rotation() const;
        void Rotation(const Eigen::Vector3d& value);
        const Eigen::Vector3d& Scale() const;
        void Scale(const Eigen::Vector3d& value);
        const std::optional<Model>& Model() const;

    private:
        Eigen::Vector3d m_position;
        Eigen::Vector3d m_rotation;
        Eigen::Vector3d m_scale;
        const std::optional<Renderer::Model> m_model;
    };
}