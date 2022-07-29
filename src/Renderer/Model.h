#pragma once

namespace renderer
{
    struct Model
    {
        Model(
            std::vector<std::array<double, 3>>&& wfVertices,
            std::vector<std::array<uint32_t, 3>>&& wfFaces
        );

        Model(std::filesystem::path objFile);

        const std::vector<std::array<double, 3>>& Vertices() const;
        const std::vector<std::array<uint32_t, 3>>& Faces() const;

    private:
        const std::vector<std::array<double, 3>> m_wfVertices;
        const std::vector<std::array<uint32_t, 3>> m_wfFaces;

        Model(std::tuple<std::vector<std::array<double, 3>>,
            std::vector<std::array<uint32_t, 3>>>&& wfVerticesAndFaces);
    };
}