// SPDX-License-Identifier: BSD-2-Clause
#pragma once

struct GlfwMouseButtonEvent final {
    int button {GLFW_KEY_UNKNOWN};
    int action {};
    int mods {};
};
