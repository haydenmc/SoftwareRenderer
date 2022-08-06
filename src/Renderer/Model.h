#pragma once

namespace Renderer
{
    struct Model
    {
        Model(
            std::vector<Eigen::Vector3d>&& vertices,
            std::vector<Eigen::Vector3i>&& faces
        );
        const std::vector<Eigen::Vector3d>& Vertices() const;
        const std::vector<Eigen::Vector3i>& Faces() const;

    private:
        const std::vector<Eigen::Vector3d> m_vertices;
        const std::vector<Eigen::Vector3i> m_faces;
    };
}