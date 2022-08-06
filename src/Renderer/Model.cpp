#include "pch.h"
#include "Model.h"

namespace Renderer
{
    Model::Model(
        std::vector<Eigen::Vector3d>&& vertices,
        std::vector<Eigen::Vector3i>&& faces
    ) :
        m_vertices{ vertices },
        m_faces{ faces }
    { }

    const std::vector<Eigen::Vector3d>& Model::Vertices() const
    {
        return m_vertices;
    }

    const std::vector<Eigen::Vector3i>& Model::Faces() const
    {
        return m_faces;
    }
}