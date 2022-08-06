#include "pch.h"
#include "Simulation.h"

namespace
{
    constexpr double c_rotationVelocity{ 0.001 };
}

namespace Renderer
{
    Simulation::Simulation(std::shared_ptr<Renderer> renderer) :
        m_renderer{ renderer }
    {
        // Put a simple triangle in the world entities
        Model triangleModel{
            // Vertices
            {
                {  0.0,   0.5,  0.0 },
                { -0.5,  -0.5,  0.0 },
                {  0.5,  -0.5,  0.0 },
            },
            // Faces by index
            {
                { 0, 1, 2 }
            }
        };
        Entity testEntity{
            std::move(triangleModel),
            { 0.0, 0.0, 6.0 }, // Position
            { 0.0, 0.0, 0.0 }, // Rotation
            { 1.0, 1.0, 1.0 }, // Scale
        };
        m_worldEntities.push_back(std::move(testEntity));
    }

    void Simulation::Run()
    {
        std::chrono::high_resolution_clock::time_point lastUpdateTime{
            std::chrono::high_resolution_clock::now() };

        SDL_Event event{ 0 };
        do
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            auto updateDelta{ currentTime - lastUpdateTime };
            Update(updateDelta);
            lastUpdateTime = currentTime;

            if (m_renderer)
            {
                m_renderer->Draw(
                    currentTime,
                    m_camera,
                    m_worldEntities
                );
            }

            CheckSdlError(SDL_PollEvent(&event));
        }
        while (event.type != SDL_QUIT);
    }

    void Simulation::Update(std::chrono::high_resolution_clock::duration deltaTime)
    {
        // Rotate every object around the y (up) axis
        for (auto& entity : m_worldEntities)
        {
            const auto& entityRotation{ entity.Rotation() };
            auto newRotation{
                entityRotation +
                Eigen::Vector3d{
                    0.0,
                    c_rotationVelocity * std::chrono::duration<double, std::milli>(deltaTime).count(),
                    0.0,
                }
            };
            entity.Rotation(newRotation);
        }
    }
}