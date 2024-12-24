// SPDX-License-Identifier: BSD-2-Clause
#pragma once
#include "mathlib/constexpr.hh"

constexpr static std::size_t CHUNK_SIZE = 16;
constexpr static std::size_t CHUNK_AREA = CHUNK_SIZE * CHUNK_SIZE;
constexpr static std::size_t CHUNK_VOLUME = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;
constexpr static std::size_t CHUNK_SIZE_LOG2 = cxpr::log2(CHUNK_SIZE);
