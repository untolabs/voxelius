// SPDX-License-Identifier: BSD-2-Clause
#pragma once

struct Resource {
    std::string name {};
};

namespace resource
{
template<typename T>
std::shared_ptr<const T> load(const std::string &name, unsigned int load_flags = 0U);
template<typename T>
void hard_cleanup(void);
template<typename T>
void soft_cleanup(void);
} // namespace resource
