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
        // Put a simple 1x1x1 cube in the world entities
        Model cubeModel{{
            {  0.5,  0.5,  0.5 },
            { -0.5,  0.5,  0.5 },
            { -0.5, -0.5,  0.5 },
            {  0.5, -0.5,  0.5 },
            {  0.5,  0.5, -0.5 },
            { -0.5,  0.5, -0.5 },
            { -0.5, -0.5, -0.5 },
            {  0.5, -0.5, -0.5 },
        }};
        Entity cubeEntity{
            std::move(cubeModel),
            { 3.0, 3.0, 2.0 }, // Position
            { 0.0, 0.0, 0.0 }, // Rotation
            { 1.0, 1.0, 1.0 }, // Scale
        };
        m_worldEntities.push_back(std::move(cubeEntity));
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
                    c_rotationVelocity * std::chrono::duration<double, std::milli>(deltaTime).count(),
                    c_rotationVelocity* std::chrono::duration<double, std::milli>(deltaTime).count(),
                    c_rotationVelocity* std::chrono::duration<double, std::milli>(deltaTime).count(),
                }
            };
            entity.Rotation(newRotation);
        }
    }
}