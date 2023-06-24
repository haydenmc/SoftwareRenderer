#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// STL
#include <array>
#include <chrono>
#include <filesystem>
#include <format>
#include <fstream>
#include <numbers>
#include <optional>
#include <stdexcept>
#include <sstream>
#include <string>
#include <vector>

// SDL
#include <SDL.h>

// Eigen3
#pragma warning( suppress: 5054 4127 )
#include <Eigen/Dense>

// spdlog
#include <spdlog/spdlog.h>

// Utilities
#include "Utilities.h"