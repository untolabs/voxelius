// SPDX-License-Identifier: BSD-2-Clause
#pragma once

#include <cctype>
#include <cmath>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <cstdio> // std::sprintf-likes only. if you use anything else, I will murder you

#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <functional>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include <BS_thread_pool.hpp>

// FIXME: including hash_set8.hpp is fucked up whenever
// hash_table8.hpp is included. It doesn't even compile
// possibly due some function re-definitions. Too bad!
#include <emhash/hash_table8.hpp>

#include <enet/enet.h>

#include <entt/entity/registry.hpp>
#include <entt/signal/dispatcher.hpp>

#include <fastnoiselite.h>

#include <GLFW/glfw3.h>

#include <glad/gl.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>

#include <miniz.h>

#include <parson.h>

#include <physfs.h>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <stb_image.h>
#include <stb_image_write.h>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#if defined(__unix__)
#include <dlfcn.h>
#endif
