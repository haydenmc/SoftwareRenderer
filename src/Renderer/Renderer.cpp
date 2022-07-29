#include "pch.h"
#include "Renderer.h"

namespace
{
    constexpr std::array<uint8_t, 4> c_clearColor{ 0, 0, 0, 0 };
    constexpr std::array<uint8_t, 4> c_wireFrameColor{ 255, 255, 255, 255 };
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
        m_sdlRenderer{ nullptr },
        m_headModel{ std::filesystem::path{ "Assets/head.obj" } }
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
            Draw();
            SDL_RenderPresent(m_sdlRenderer);
            CheckSdlError(SDL_PollEvent(&event));
        }
        while (event.type != SDL_QUIT);
    }

    void Renderer::Draw()
    {
        DrawWireframe(m_headModel);
    }

    void Renderer::DrawWireframe(const Model& model)
    {
        CheckSdlError(SDL_SetRenderDrawColor(
            m_sdlRenderer,
            c_wireFrameColor[0],
            c_wireFrameColor[1],
            c_wireFrameColor[2],
            c_wireFrameColor[3]
        ));
        
        for (const auto& face : model.Faces())
        {
            for (int j = 0; j < 3; j++)
            {
                const auto& v0{ model.Vertices().at(face[j]) };
                const auto& v1{ model.Vertices().at(face[(j + 1) % 3]) };
                auto x0{ static_cast<int>((v0[0] + 1.) * m_width / 2.) };
                auto y0{ static_cast<int>((-v0[1] + 1.) * m_height / 2.) };
                auto x1{ static_cast<int>((v1[0] + 1.) * m_width / 2.) };
                auto y1{ static_cast<int>((-v1[1] + 1.) * m_height / 2.) };
                SDL_RenderDrawLine(m_sdlRenderer, x0, y0, x1, y1);
            }
        }
    }
}