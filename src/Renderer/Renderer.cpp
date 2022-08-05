#include "pch.h"
#include "Renderer.h"

namespace
{
    constexpr double c_zNear{ 0.1 };
    constexpr double c_zFar{ 1000.0 };
    constexpr std::array<uint8_t, 4> c_clearColor{ 0, 0, 0, 0 };
    constexpr std::array<uint8_t, 4> c_wireFrameColor{ 255, 255, 255, 255 };

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
            { (aspectRatio * (1 / std::tan(fieldOfView / 2))), 0, 0, 0 },
            { 0, (1 / std::tan(fieldOfView / 2)), 0, 0},
            { 0, 0, (zFar / (zFar - zNear)), ((-zFar * zNear) / (zFar - zNear)) },
            { 0, 0, 1, 0 },
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
        m_sdlRenderer{ nullptr }
    {
        spdlog::info(
            "Creating render window ({}x{}) @ {}fps...",
            m_width,
            m_height,
            m_framesPerSecondLimit
        );
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
        const Entity& /*camera*/,
        const std::vector<Entity>& worldEntities
    )
    {
        CheckSdlError(SDL_SetRenderDrawColor(
            m_sdlRenderer,
            c_clearColor[0],
            c_clearColor[1],
            c_clearColor[2],
            c_clearColor[3]
        ));
        CheckSdlError(SDL_RenderClear(m_sdlRenderer));
        CheckSdlError(SDL_SetRenderDrawColor(
            m_sdlRenderer,
            c_wireFrameColor[0],
            c_wireFrameColor[1],
            c_wireFrameColor[2],
            c_wireFrameColor[3]
        ));

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
                        worldTransformationMatrix *
                        Eigen::Vector4d{ vertex.x(), vertex.y(), vertex.z(), 1 }
                    };
                    SDL_RenderDrawPoint(m_sdlRenderer, transformationResult.x() * 100, transformationResult.y() * 100);
                }
            }
        }

        SDL_RenderPresent(m_sdlRenderer);
    }
}