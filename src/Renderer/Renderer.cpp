#include "pch.h"
#include "Renderer.h"

namespace
{
    constexpr std::array<uint8_t, 4> c_clearColor{ 0, 0, 0, 0 };
}

namespace renderer
{
    Renderer::Renderer(
        uint32_t width,
        uint32_t height
    ) :
        m_width{ width },
        m_height{ height },
        m_sdlWindow{ nullptr },
        m_sdlRenderer{ nullptr }
    {
        CheckSdlError(SDL_Init(SDL_INIT_VIDEO));
        CheckSdlError(SDL_CreateWindowAndRenderer(
            m_width,
            m_height,
            0,
            &m_sdlWindow,
            &m_sdlRenderer
        ));
        CheckSdlError(SDL_SetRenderDrawColor(
            m_sdlRenderer,
            0,
            0,
            0,
            0
        ));
    }

    void Renderer::Run()
    {
        SDL_Event event{ 0 };
        do
        {
            CheckSdlError(SDL_SetRenderDrawColor(
                m_sdlRenderer,
                c_clearColor[0],
                c_clearColor[1],
                c_clearColor[2],
                c_clearColor[3]
            ));
            CheckSdlError(SDL_RenderClear(m_sdlRenderer));
            // Draw here
            SDL_RenderPresent(m_sdlRenderer);
            CheckSdlError(SDL_PollEvent(&event));
        }
        while (event.type != SDL_QUIT);
    }
}