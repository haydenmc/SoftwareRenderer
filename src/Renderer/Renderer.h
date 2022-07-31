#pragma once
#include "Entity.h"

namespace renderer
{
    struct Renderer
    {
        Renderer(
            uint32_t width,
            uint32_t height,
            uint16_t framesPerSecondLimit = 0
        );

        void Run();

    private:
        const uint32_t m_width;
        const uint32_t m_height;
        const uint16_t m_framesPerSecondLimit;
        const std::chrono::high_resolution_clock::duration m_timePerFrame;
        SDL_Window* m_sdlWindow;
        SDL_Renderer* m_sdlRenderer;
        Entity m_camera;
        std::vector<Entity> m_worldEntities;

        void Update(std::chrono::high_resolution_clock::duration deltaTime);
        void Draw();
        void DrawWireframe(const Model& model);
    };
}