#pragma once

namespace Renderer
{
    struct SdlException : public std::runtime_error
    {
        SdlException(const char* message);
        SdlException(std::string message);
    };

    void CheckSdlError(int sdlReturnValue);
}