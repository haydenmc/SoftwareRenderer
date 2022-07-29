#include "pch.h"
#include "Renderer.h"

namespace
{
    constexpr uint32_t c_windowWidth{ 800 };
    constexpr uint32_t c_windowHeight{ 600 };
}

int wWinMain(
    HINSTANCE /*hInstance*/,
    HINSTANCE /*prevInstance*/,
    wchar_t* /*cmdLine*/,
    int /*nCmd*/
)
{
    renderer::Renderer renderer{ c_windowWidth, c_windowHeight };
    renderer.Run();
    return 0;
}