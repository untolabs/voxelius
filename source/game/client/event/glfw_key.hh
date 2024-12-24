// SPDX-License-Identifier: BSD-2-Clause
#pragma once

struct GlfwKeyEvent final {
    int key {GLFW_KEY_UNKNOWN};
    int scancode {};
    int action {};
    int mods {};
};
