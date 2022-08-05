#include "pch.h"
#include "Model.h"

namespace Renderer
{
    Model::Model(std::vector<Eigen::Vector3d>&& vertices) :
        m_vertices{ vertices }
    { }

    const std::vector<Eigen::Vector3d>& Model::Vertices() const
    {
        return m_vertices;
    }
}