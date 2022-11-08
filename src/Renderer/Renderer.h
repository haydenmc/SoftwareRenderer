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
        SDL_Surface* m_sdlWindowSurface;
        std::chrono::high_resolution_clock::time_point m_lastFrameTime;

        void DrawInternal(
            const std::chrono::high_resolution_clock::time_point& timePoint,
            const Entity& camera,
            const std::vector<Entity>& worldEntities
        );

        void DrawEntities(
            const Entity& camera,
            const std::vector<Entity>& worldEntities
        );

        void DrawModel(
            const Eigen::Matrix4d& transformMatrix,
            const Model& model);

        void RasterizeTriangle(const std::array<Eigen::Vector2i, 3>& screenSpaceVertices);

        void SetAllPixels(
            uint8_t r,
            uint8_t g,
            uint8_t b
        );

        void SetPixel(
            uint32_t x,
            uint32_t y,
            uint8_t r,
            uint8_t g,
            uint8_t b
        );
    };
}