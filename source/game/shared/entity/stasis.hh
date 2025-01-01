// SPDX-License-Identifier: BSD-2-Clause
#pragma once

class StasisComponent final {
public:
    // Attaches the component to any entity
    // which TransformComponent states the entity
    // is within a chunk that is not yet present
    static void fixed_update(void);
};
