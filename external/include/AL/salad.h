/*
 * salad.h - OpenAL loader declarations
 * Copyright (C) 2021-2025, Kirill Dmitrievich
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef AL_SALAD_H
#define AL_SALAD_H 1
#include <float.h>

#define SALAD_VERSION_MAJOR 0
#define SALAD_VERSION_MINOR 0
#define SALAD_VERSION_PATCH 2

#if defined(_MSC_VER)
#define SALAD_APIENTRY      __cdecl
#define SALAD_ALX_APIENTRY  __cdecl
#else
#define SALAD_APIENTRY
#define SALAD_ALX_APIENTRY
#endif

#if defined(__cplusplus)
#define SALAD_EXTERN extern "C"
#else
#define SALAD_EXTERN extern
#endif

typedef float               salad_float32_type;
typedef double              salad_float64_type;

typedef signed char         salad_int8_type;
typedef signed short        salad_int16_type;

typedef unsigned char       salad_uint8_type;
typedef unsigned short      salad_uint16_type;

#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || defined(__GNUC__)
#include <stdint.h>
typedef int32_t             salad_int32_type;
typedef uint32_t            salad_uint32_type;
#elif defined(__cplusplus) && __cplusplus >= 201103L
#include <cstdint>
typedef std::int32_t        salad_int32_type;
typedef std::uint32_t       salad_uint32_type;
#elif defined(_WIN32)
typedef __int32             salad_int32_type;
typedef unsigned __int32    salad_uint32_type;
#else
typedef int                 salad_int32_type;
typedef unsigned int        salad_uint32_type;
#endif

/**
 * A pointer to a function responsible for resolving a function
 * with a specific name from the module loaded by SALAD or by the user
 * @param procname  Function name
 * @param arg       Optional implementation-used argument
 * @returns         A resolved function pointer on success, NULL on failure
 */
typedef void* (SALAD_APIENTRY *SALAD_loadfunc_type)(const char *procname, void *arg);

/**
 * Loads OpenAL and extensions using implementation-defined
 * dynamic library paths and using implementation-defined API
 * @returns         Zero on failure, non-zero on success
 */
SALAD_EXTERN int saladLoadALdefault(void);

/**
 * Loads OpenAL and extensions using a user-provided load
 * function; Initially this was used to mate Source SDK's dynamic
 * linking API and OpenAL but the entire project went nowhere
 * @param loadfunc  User-provided load function
 * @param arg       Optional argument for the function
 * @returns         Zero on failure, non-zero on success
 */
SALAD_EXTERN int saladLoadALfunc(SALAD_loadfunc_type loadfunc, void *arg);

#endif /* AL_SALAD_H */
