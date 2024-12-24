// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "cmake/config.hh"

// Experiments subsystem contains early and
// temporary features that don't have a
// good place to be permanently implemented yet

#if ENABLE_EXPERIMENTS

namespace experiments
{
void init(void);
void init_late(void);
void deinit(void);
void update(void);
void update_late(void);
} // namespace experiments

#endif /* ENABLE_EXPERIMENTS */
