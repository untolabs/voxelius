// SPDX-License-Identifier: BSD-2-Clause
#pragma once

#include <cctype>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include <algorithm>
#include <array>
#include <chrono>
#include <memory>
#include <string>
#include <sstream>
#include <typeindex>
#include <typeinfo>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

// FIXME: including hash_set8.hpp is fucked up whenever
// hash_table8.hpp is included. It doesn't even compile
// possibly due some function re-definitions. Too bad!
#include <emhash/hash_table8.hpp>

#include <physfs.h>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>

#include <stb_image.h>
#include <stb_image_write.h>
