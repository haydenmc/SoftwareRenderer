#pragma once
#include "Renderer.h"

namespace Renderer
{
    struct Simulation
    {
        Simulation(std::shared_ptr<Renderer> renderer = nullptr);
        void Run();

    private:
        const std::shared_ptr<Renderer> m_renderer;
        Entity m_camera;
        std::vector<Entity> m_worldEntities;

        void Update(std::chrono::high_resolution_clock::duration deltaTime);
    };
}