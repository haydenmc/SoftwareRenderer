#pragma once
#include "Entity.h"

namespace Renderer
{
    struct Renderer
    {
        Renderer(
            uint32_t width,
            uint32_t height,
            uint16_t framesPerSecondLimit = 0
        );

        void Draw(
            const std::chrono::high_resolution_clock::time_point& timePoint,
            const Entity& camera,
            const std::vector<Entity>& worldEntities
        );

    private:
        const uint32_t m_width;
        const uint32_t m_height;
        const uint16_t m_framesPerSecondLimit;
        const std::chrono::high_resolution_clock::duration m_timePerFrame;
        SDL_Window* m_sdlWindow;
        SDL_Renderer* m_sdlRenderer;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;

        void DrawInternal(
            const std::chrono::high_resolution_clock::time_point& timePoint,
            const Entity& camera,
            const std::vector<Entity>& worldEntities
        );
    };
}