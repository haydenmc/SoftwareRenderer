#include "pch.h"
#include "Renderer.h"

namespace
{
    constexpr double c_zNear{ 0.1 };
    constexpr double c_zFar{ 100.0 };
    constexpr std::array<uint8_t, 3> c_clearColor{ 0, 0, 0 };

    Eigen::Matrix4d GetEntityTRSMatrix(const Renderer::Entity& entity)
    {
        // Translation
        const auto& entityPosition{ entity.Position() };
        Eigen::Matrix4d entityWorldTranslate{
            { 1., 0., 0., entityPosition.x() },
            { 0., 1., 0., entityPosition.y() },
            { 0., 0., 1., entityPosition.z() },
            { 0., 0., 0., 1. },
        };

        // Rotation
        const auto& entityRotation{ entity.Rotation() };
        Eigen::Matrix4d entityWorldRotateX{
            { 1., 0., 0., 0. },
            { 0., std::cos(entityRotation.x()), -std::sin(entityRotation.x()), 0. },
            { 0., std::sin(entityRotation.x()), std::cos(entityRotation.x()), 0. },
            { 0., 0., 0., 1. },
        };
        Eigen::Matrix4d entityWorldRotateY{
            { std::cos(entityRotation.y()), 0., std::sin(entityRotation.y()), 0. },
            { 0., 1., 0., 0. },
            { -std::sin(entityRotation.y()), 0., std::cos(entityRotation.y()), 0. },
            { 0., 0., 0., 1. },
        };
        Eigen::Matrix4d entityWorldRotateZ{
            { std::cos(entityRotation.z()), -std::sin(entityRotation.z()), 0., 0. },
            { std::sin(entityRotation.z()), std::cos(entityRotation.z()), 0., 0. },
            { 0., 0., 1., 0. },
            { 0., 0., 0., 1. },
        };
        Eigen::Matrix4d entityWorldRotate{
            entityWorldRotateZ *
            entityWorldRotateY *
            entityWorldRotateX
        };

        // TODO: Scale

        return entityWorldTranslate * entityWorldRotate;
    }

    Eigen::Matrix4d GetPerspectiveTransformMatrix(
        double fieldOfView,
        double aspectRatio,
        double zNear,
        double zFar
    )
    {
        return Eigen::Matrix4d{
            { (aspectRatio * (1. / std::tan(fieldOfView / 2.))), 0, 0, 0 },
            { 0, (1. / std::tan(fieldOfView / 2.)), 0, 0},
            { 0, 0, (zFar / (zFar - zNear)), (-(zFar / (zFar - zNear)) * zNear) },
            { 0, 0, -1, 0 },
        };
    }
}

namespace Renderer
{
    Renderer::Renderer(
        uint32_t width,
        uint32_t height,
        uint16_t framesPerSecondLimit
    ) :
        m_width{ width },
        m_height{ height },
        m_framesPerSecondLimit{ framesPerSecondLimit },
        m_timePerFrame{ m_framesPerSecondLimit == 0 ? std::chrono::seconds{ 0 } :
            (std::chrono::nanoseconds{ std::chrono::seconds{ 1 } } / m_framesPerSecondLimit) },
        m_sdlWindow{ nullptr },
        m_sdlWindowSurface{ nullptr }
    {
        spdlog::info(
            "Creating render window ({}x{}) @ {}fps...",
            m_width,
            m_height,
            m_framesPerSecondLimit
        );
        CheckSdlError(SDL_Init(SDL_INIT_VIDEO));
        m_sdlWindow = ThrowIfSdlNull(SDL_CreateWindow(
            "Simulation",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            m_width,
            m_height,
            0
        ));
        m_sdlWindowSurface = ThrowIfSdlNull(SDL_GetWindowSurface(m_sdlWindow));
        if (m_sdlWindowSurface->format->format != SDL_PIXELFORMAT_XRGB8888)
        {
            throw SdlException("Window surface has unexpected pixel format");
        }
    }

    void Renderer::Draw(
        const std::chrono::high_resolution_clock::time_point& timePoint,
        const Entity& camera,
        const std::vector<Entity>& worldEntities
    )
    {
        auto currentTime = std::chrono::high_resolution_clock::now();

        if (m_framesPerSecondLimit == 0)
        {
            DrawInternal(
                timePoint,
                camera,
                worldEntities
            );
            m_lastFrameTime = currentTime;
        }
        else
        {
            if ((currentTime - m_lastFrameTime) >= m_timePerFrame)
            {
                DrawInternal(
                    timePoint,
                    camera,
                    worldEntities
                );
                m_lastFrameTime = currentTime;
            }
        }
    }

    void Renderer::DrawInternal(
        const std::chrono::high_resolution_clock::time_point& timePoint,
        const Entity& camera,
        const std::vector<Entity>& worldEntities
    )
    {
        CheckSdlError(SDL_LockSurface(m_sdlWindowSurface));

        SetAllPixels(
            c_clearColor.at(0),
            c_clearColor.at(1),
            c_clearColor.at(2)
        );

        DrawEntities(
            camera,
            worldEntities
        );

        SDL_UnlockSurface(m_sdlWindowSurface);
        SDL_UpdateWindowSurface(m_sdlWindow);
    }

    void Renderer::DrawEntities(
        const Entity& camera,
        const std::vector<Entity>& worldEntities
    )
    {
        for (const auto& entity : worldEntities)
        {
            // Get new vertex positions
            if (entity.Model())
            {
                auto worldTransformationMatrix{ GetEntityTRSMatrix(entity) };
                const auto& entityModel{ entity.Model().value() };
                for (const auto& vertex : entityModel.Vertices())
                {
                    auto transformationResult{
                        GetPerspectiveTransformMatrix(
                            ((1 / 3.) * std::numbers::pi),
                            (m_height / static_cast<double>(m_width)),
                            c_zNear,
                            c_zFar
                        ) *
                        worldTransformationMatrix *
                        Eigen::Vector4d{ vertex.x(), vertex.y(), vertex.z(), 1 }
                    };

                    double ndcX{ transformationResult.x() };
                    double ndcY{ transformationResult.y() };
                    double ndcZ{ transformationResult.z() };
                    if (transformationResult.w() != 0)
                    {
                        ndcX /= transformationResult.w();
                        ndcY /= transformationResult.w();
                        ndcZ /= transformationResult.w();
                    }

                    auto screenX{ static_cast<uint32_t>((ndcX * m_width) + (m_width / 2.)) };
                    auto screenY{ static_cast<uint32_t>((ndcY * m_height) + (m_height / 2.)) };

                    if ((screenX <= m_width) && (screenY <= m_height))
                    {
                        SetPixel(screenX, screenY, 0xFF, 0xFF, 0xFF);
                    }
                }
            }
        }
    }

    void Renderer::SetAllPixels(uint8_t r, uint8_t g, uint8_t b)
    {
        for (uint32_t i = 0; i < (m_sdlWindowSurface->w * m_sdlWindowSurface->h); ++i)
        {
            uint32_t y{ i / m_sdlWindowSurface->w };
            uint32_t x{ i % m_sdlWindowSurface->w };
            auto const targetPixel{ reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(m_sdlWindowSurface->pixels) +
                y * m_sdlWindowSurface->pitch +
                x * m_sdlWindowSurface->format->BytesPerPixel
            ) };
            *targetPixel = (
                0xFF000000 | // alpha
                (static_cast<uint32_t>(r) << 16) | // r
                (static_cast<uint32_t>(g) << 8) | // g
                static_cast<uint32_t>(b) // b
            );
        }
    }

    void Renderer::SetPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b)
    {
        if (x > m_sdlWindowSurface->w || y > m_sdlWindowSurface->h)
        {
            throw std::invalid_argument("Attempt to set pixel outside of window boundary");
        }

        auto const targetPixel{ reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint8_t*>(m_sdlWindowSurface->pixels) +
            y * m_sdlWindowSurface->pitch +
            x * m_sdlWindowSurface->format->BytesPerPixel
        ) };
        *targetPixel = (
            0xFF000000 | // alpha
            (static_cast<uint32_t>(r) << 16) | // r
            (static_cast<uint32_t>(g) << 8) | // g
            static_cast<uint32_t>(b) // b
        );
    }
}