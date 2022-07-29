#include "pch.h"
#include "Model.h"

namespace
{
    std::tuple<std::vector<std::array<double, 3>>,
        std::vector<std::array<uint32_t, 3>>>
        ReadFromObjFile(std::filesystem::path file)
    {
        std::vector<std::array<double, 3>> vertices;
        std::vector<std::array<uint32_t, 3>> faces;
        std::ifstream in;
        // in.exceptions(std::ios::badbit | std::ios::failbit);
        in.open(file, std::ifstream::in);
        if (in.fail())
        {
            throw std::runtime_error(std::format(
                "Could not open model file '{}'",
                file.string()));
        }
        std::string line;
        while (!in.eof())
        {
            std::getline(in, line);
            std::istringstream iss(line);
            if (line.compare(0, 2, "v ") == 0)
            {
                iss.get();
                std::array<double, 3> vertex;
                for (size_t i = 0; i < 3; ++i)
                {
                    iss >> vertex[i];
                }
                vertices.push_back(std::move(vertex));
            }
            else if (line.compare(0, 2, "f ") == 0)
            {
                std::array<uint32_t, 3> face;
                // TODO: Don't really like this use of streams, replace later...
                char trash;
                int itrash, idx;
                iss >> trash;
                for (size_t i = 0; i < 3; ++i)
                {
                    iss >> idx >> trash >> itrash >> trash >> itrash;
                    face[i] = (idx - 1);
                }
                faces.push_back(std::move(face));
            }
        }
        return { std::move(vertices), std::move(faces) };
    }
}

namespace renderer
{
    Model::Model(
        std::vector<std::array<double, 3>>&& wfVertices,
        std::vector<std::array<uint32_t, 3>>&& wfFaces
    ) :
        m_wfVertices{ wfVertices },
        m_wfFaces{ wfFaces }
    { }

    Model::Model(std::filesystem::path objFile) : Model(ReadFromObjFile(objFile))
    { }

    const std::vector<std::array<double, 3>>& Model::Vertices() const
    {
        return m_wfVertices;
    }

    const std::vector<std::array<uint32_t, 3>>& Model::Faces() const
    {
        return m_wfFaces;
    }

    Model::Model(std::tuple<std::vector<std::array<double, 3>>,
        std::vector<std::array<uint32_t, 3>>>&& wfVerticesAndFaces) :
        Model(
            std::move(std::get<0>(wfVerticesAndFaces)),
            std::move(std::get<1>(wfVerticesAndFaces))
        )
    { }
}