#include "pch.h"
#include "Renderer.h"
#include "Simulation.h"
#include <spdlog/sinks/msvc_sink.h>

namespace
{
    constexpr uint32_t c_windowWidth{ 800 };
    constexpr uint32_t c_windowHeight{ 600 };
    constexpr uint16_t c_framesPerSecondLimit{ 60 };

    void SetupLogging()
    {
#ifdef _DEBUG
        auto msvcSink{ std::make_shared<spdlog::sinks::msvc_sink_mt>() };
        auto logger{ std::make_shared<spdlog::logger>("debug", msvcSink) };
        spdlog::set_default_logger(logger);
#endif
    }
}

int wWinMain(
    HINSTANCE /*hInstance*/,
    HINSTANCE /*prevInstance*/,
    wchar_t* /*cmdLine*/,
    int /*nCmd*/
)
{
    SetupLogging();
    spdlog::info("WinMain");
    spdlog::info("Initializing renderer.");
    auto renderer{ std::make_shared<Renderer::Renderer>(
        c_windowWidth,
        c_windowHeight,
        c_framesPerSecondLimit
    ) };
    spdlog::info("Initializing simulation.");
    Renderer::Simulation sim{ renderer };
    spdlog::info("Starting simulation.");
    sim.Run();
    spdlog::info("Simulation stopped.");
    return 0;
}