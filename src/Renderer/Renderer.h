#pragma once

namespace renderer
{
    struct Renderer
    {
        Renderer(
            uint32_t width,
            uint32_t height
        );

        void Run();

    private:
        const uint32_t m_width;
        const uint32_t m_height;
        SDL_Window* m_sdlWindow;
        SDL_Renderer* m_sdlRenderer;
    };
}