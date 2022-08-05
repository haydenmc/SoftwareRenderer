#pragma once

namespace Renderer
{
    struct Model
    {
        Model(std::vector<Eigen::Vector3d>&& vertices);
        const std::vector<Eigen::Vector3d>& Vertices() const;

    private:
        const std::vector<Eigen::Vector3d> m_vertices;
    };
}