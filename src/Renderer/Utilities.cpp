#include "pch.h"
#include "Utilities.h"

namespace renderer
{
    SdlException::SdlException(const char* message) : std::runtime_error(message)
    { }

    SdlException::SdlException(std::string message) : std::runtime_error(message)
    { }

    void CheckSdlError(int sdlReturnValue)
    {
        if (sdlReturnValue < 0)
        {
            throw SdlException{ SDL_GetError() };
        }
    }
}