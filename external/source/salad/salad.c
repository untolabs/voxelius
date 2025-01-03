/*
 * salad.c - OpenAL loader implementation
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
#include <AL/salad.h>
#include <stddef.h>

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/efx.h>

#if defined(__unix__) || (defined(__APPLE__) && defined(__MACH__))
#define SALAD_POSIX 1
#define SALAD_WIN32 0
#elif defined(_WIN32)
#define SALAD_POSIX 0
#define SALAD_WIN32 1
#endif

#if SALAD_POSIX
#include <dlfcn.h>
#define salad_load_library(library)     dlopen((library), RTLD_LAZY)
#define salad_get_proc(handle, proc)    dlsym((handle), (proc))
#elif SALAD_WIN32
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>
#define salad_load_library(library)     ((void *)LoadLibraryA((library)))
#define salad_get_proc(handle, proc)    ((void *)GetProcAddress((HMODULE)(handle), (proc)))
#endif

#if SALAD_POSIX && defined(__APPLE__)
static const char *openal_libnames[] = { "libopenal.dylib", NULL };
#elif SALAD_POSIX && defined(__linux__)
static const char *openal_libnames[] = { "libopenal.so.1", "libopenal.so", NULL };
#elif SALAD_WIN32
static const char *openal_libnames[] = { "openal32.dll", "soft_oal.dll", NULL };
#endif

#if SALAD_PARANOID
#define SALAD_RESOLVE(pointer, value) if(((pointer) = (value)) == NULL) return 0; else ((void)(0))
#else
#define SALAD_RESOLVE(pointer, value) if(((pointer) = (value)) == NULL) ((void)(0)); else((void)(0))
#endif

/**
 * The default function that uses platform-specific
 * library names and a platform-specific dynamic linking API
 * @param procname  Function name
 * @param arg       Optional implementation-used argument
 * @returns         A resolved function pointer on success, NULL on failure
 */
static void* default_loadfunc(const char *procname, void *arg)
{
    if(arg != NULL)
        return salad_get_proc(arg, procname);
    return NULL;
}

/* Core OpenAL functions <AL/al.h> */
PFN_alBuffer3f              SALAD_alBuffer3f                = NULL;
PFN_alBuffer3i              SALAD_alBuffer3i                = NULL;
PFN_alBufferData            SALAD_alBufferData              = NULL;
PFN_alBufferf               SALAD_alBufferf                 = NULL;
PFN_alBufferfv              SALAD_alBufferfv                = NULL;
PFN_alBufferi               SALAD_alBufferi                 = NULL;
PFN_alBufferiv              SALAD_alBufferiv                = NULL;
PFN_alDeleteBuffers         SALAD_alDeleteBuffers           = NULL;
PFN_alDeleteSources         SALAD_alDeleteSources           = NULL;
PFN_alDisable               SALAD_alDisable                 = NULL;
PFN_alEnable                SALAD_alEnable                  = NULL;
PFN_alGenBuffers            SALAD_alGenBuffers              = NULL;
PFN_alGenSources            SALAD_alGenSources              = NULL;
PFN_alGetBoolean            SALAD_alGetBoolean              = NULL;
PFN_alGetBooleanv           SALAD_alGetBooleanv             = NULL;
PFN_alGetBuffer3f           SALAD_alGetBuffer3f             = NULL;
PFN_alGetBuffer3i           SALAD_alGetBuffer3i             = NULL;
PFN_alGetBufferf            SALAD_alGetBufferf              = NULL;
PFN_alGetBufferfv           SALAD_alGetBufferfv             = NULL;
PFN_alGetBufferi            SALAD_alGetBufferi              = NULL;
PFN_alGetBufferiv           SALAD_alGetBufferiv             = NULL;
PFN_alGetDouble             SALAD_alGetDouble               = NULL;
PFN_alGetDoublev            SALAD_alGetDoublev              = NULL;
PFN_alGetEnumValue          SALAD_alGetEnumValue            = NULL;
PFN_alGetError              SALAD_alGetError                = NULL;
PFN_alGetFloat              SALAD_alGetFloat                = NULL;
PFN_alGetFloatv             SALAD_alGetFloatv               = NULL;
PFN_alGetInteger            SALAD_alGetInteger              = NULL;
PFN_alGetIntegerv           SALAD_alGetIntegerv             = NULL;
PFN_alGetListener3f         SALAD_alGetListener3f           = NULL;
PFN_alGetListener3i         SALAD_alGetListener3i           = NULL;
PFN_alGetListenerf          SALAD_alGetListenerf            = NULL;
PFN_alGetListenerfv         SALAD_alGetListenerfv           = NULL;
PFN_alGetListeneri          SALAD_alGetListeneri            = NULL;
PFN_alGetListeneriv         SALAD_alGetListeneriv           = NULL;
PFN_alGetProcAddress        SALAD_alGetProcAddress          = NULL;
PFN_alGetSource3f           SALAD_alGetSource3f             = NULL;
PFN_alGetSource3i           SALAD_alGetSource3i             = NULL;
PFN_alGetSourcef            SALAD_alGetSourcef              = NULL;
PFN_alGetSourcefv           SALAD_alGetSourcefv             = NULL;
PFN_alGetSourcei            SALAD_alGetSourcei              = NULL;
PFN_alGetSourceiv           SALAD_alGetSourceiv             = NULL;
PFN_alGetString             SALAD_alGetString               = NULL;
PFN_alIsBuffer              SALAD_alIsBuffer                = NULL;
PFN_alIsEnabled             SALAD_alIsEnabled               = NULL;
PFN_alIsExtensionPresent    SALAD_alIsExtensionPresent      = NULL;
PFN_alIsSource              SALAD_alIsSource                = NULL;
PFN_alListener3f            SALAD_alListener3f              = NULL;
PFN_alListener3i            SALAD_alListener3i              = NULL;
PFN_alListenerf             SALAD_alListenerf               = NULL;
PFN_alListenerfv            SALAD_alListenerfv              = NULL;
PFN_alListeneri             SALAD_alListeneri               = NULL;
PFN_alListeneriv            SALAD_alListeneriv              = NULL;
PFN_alSource3f              SALAD_alSource3f                = NULL;
PFN_alSource3i              SALAD_alSource3i                = NULL;
PFN_alSourcef               SALAD_alSourcef                 = NULL;
PFN_alSourcefv              SALAD_alSourcefv                = NULL;
PFN_alSourcei               SALAD_alSourcei                 = NULL;
PFN_alSourceiv              SALAD_alSourceiv                = NULL;
PFN_alSourcePause           SALAD_alSourcePause             = NULL;
PFN_alSourcePausev          SALAD_alSourcePausev            = NULL;
PFN_alSourcePlay            SALAD_alSourcePlay              = NULL;
PFN_alSourcePlayv           SALAD_alSourcePlayv             = NULL;
PFN_alSourceQueueBuffers    SALAD_alSourceQueueBuffers      = NULL;
PFN_alSourceRewind          SALAD_alSourceRewind            = NULL;
PFN_alSourceRewindv         SALAD_alSourceRewindv           = NULL;
PFN_alSourceStop            SALAD_alSourceStop              = NULL;
PFN_alSourceStopv           SALAD_alSourceStopv             = NULL;
PFN_alSourceUnqueueBuffers  SALAD_alSourceUnqueueBuffers    = NULL;

/* Context OpenAL functions <AL/alc.h> */
PFN_alcCaptureCloseDevice   SALAD_alcCaptureCloseDevice = NULL;
PFN_alcCaptureOpenDevice    SALAD_alcCaptureOpenDevice  = NULL;
PFN_alcCaptureSamples       SALAD_alcCaptureSamples     = NULL;
PFN_alcCaptureStart         SALAD_alcCaptureStart       = NULL;
PFN_alcCaptureStop          SALAD_alcCaptureStop        = NULL;
PFN_alcCloseDevice          SALAD_alcCloseDevice        = NULL;
PFN_alcCreateContext        SALAD_alcCreateContext      = NULL;
PFN_alcDestroyContext       SALAD_alcDestroyContext     = NULL;
PFN_alcGetContextsDevice    SALAD_alcGetContextsDevice  = NULL;
PFN_alcGetCurrentContext    SALAD_alcGetCurrentContext  = NULL;
PFN_alcGetEnumValue         SALAD_alcGetEnumValue       = NULL;
PFN_alcGetError             SALAD_alcGetError           = NULL;
PFN_alcGetIntegerv          SALAD_alcGetIntegerv        = NULL;
PFN_alcGetProcAddress       SALAD_alcGetProcAddress     = NULL;
PFN_alcGetString            SALAD_alcGetString          = NULL;
PFN_alcIsExtensionPresent   SALAD_alcIsExtensionPresent = NULL;
PFN_alcMakeContextCurrent   SALAD_alcMakeContextCurrent = NULL;
PFN_alcOpenDevice           SALAD_alcOpenDevice         = NULL;
PFN_alcProcessContext       SALAD_alcProcessContext     = NULL;
PFN_alcSuspendContext       SALAD_alcSuspendContext     = NULL;

/* Extension: ALC_EXT_EFX <AL/efx.h> */
PFN_alDeleteEffects SALAD_alDeleteEffects   = NULL;
PFN_alDeleteFilters SALAD_alDeleteFilters   = NULL;
PFN_alEffectf       SALAD_alEffectf         = NULL;
PFN_alEffectfv      SALAD_alEffectfv        = NULL;
PFN_alEffecti       SALAD_alEffecti         = NULL;
PFN_alEffectiv      SALAD_alEffectiv        = NULL;
PFN_alFilterf       SALAD_alFilterf         = NULL;
PFN_alFilterfv      SALAD_alFilterfv        = NULL;
PFN_alFilteri       SALAD_alFilteri         = NULL;
PFN_alFilteriv      SALAD_alFilteriv        = NULL;
PFN_alGenEffects    SALAD_alGenEffects      = NULL;
PFN_alGenFilters    SALAD_alGenFilters      = NULL;
PFN_alGetEffectf    SALAD_alGetEffectf      = NULL;
PFN_alGetEffectfv   SALAD_alGetEffectfv     = NULL;
PFN_alGetEffecti    SALAD_alGetEffecti      = NULL;
PFN_alGetEffectiv   SALAD_alGetEffectiv     = NULL;
PFN_alGetFilterf    SALAD_alGetFilterf      = NULL;
PFN_alGetFilterfv   SALAD_alGetFilterfv     = NULL;
PFN_alGetFilteri    SALAD_alGetFilteri      = NULL;
PFN_alGetFilteriv   SALAD_alGetFilteriv     = NULL;
PFN_alIsEffect      SALAD_alIsEffect        = NULL;
PFN_alIsFilter      SALAD_alIsFilter        = NULL;


int saladLoadALdefault(void)
{
    size_t i;
    void *module = NULL;

    for(i = 0; openal_libnames[i]; ++i) {
        if((module = salad_load_library(openal_libnames[i])))
            return saladLoadALfunc(&default_loadfunc, module);
        continue;
    }

    return 0;
}

int saladLoadALfunc(SALAD_loadfunc_type loadfunc, void *arg)
{
    if(loadfunc == NULL) {
        /* Don't bother continuing
         * to try and load function pointers
         * from literally thin air */
        return 0;
    }

    SALAD_RESOLVE(SALAD_alBuffer3f,             loadfunc("alBuffer3f",              arg));
    SALAD_RESOLVE(SALAD_alBuffer3i,             loadfunc("alBuffer3i",              arg));
    SALAD_RESOLVE(SALAD_alBufferData,           loadfunc("alBufferData",            arg));
    SALAD_RESOLVE(SALAD_alBufferf,              loadfunc("alBufferf",               arg));
    SALAD_RESOLVE(SALAD_alBufferfv,             loadfunc("alBufferfv",              arg));
    SALAD_RESOLVE(SALAD_alBufferi,              loadfunc("alBufferi",               arg));
    SALAD_RESOLVE(SALAD_alBufferiv,             loadfunc("alBufferiv",              arg));
    SALAD_RESOLVE(SALAD_alDeleteBuffers,        loadfunc("alDeleteBuffers",         arg));
    SALAD_RESOLVE(SALAD_alDeleteSources,        loadfunc("alDeleteSources",         arg));
    SALAD_RESOLVE(SALAD_alDisable,              loadfunc("alDisable",               arg));
    SALAD_RESOLVE(SALAD_alEnable,               loadfunc("alEnable",                arg));
    SALAD_RESOLVE(SALAD_alGenBuffers,           loadfunc("alGenBuffers",            arg));
    SALAD_RESOLVE(SALAD_alGenSources,           loadfunc("alGenSources",            arg));
    SALAD_RESOLVE(SALAD_alGetBoolean,           loadfunc("alGetBoolean",            arg));
    SALAD_RESOLVE(SALAD_alGetBooleanv,          loadfunc("alGetBooleanv",           arg));
    SALAD_RESOLVE(SALAD_alGetBuffer3f,          loadfunc("alGetBuffer3f",           arg));
    SALAD_RESOLVE(SALAD_alGetBuffer3i,          loadfunc("alGetBuffer3i",           arg));
    SALAD_RESOLVE(SALAD_alGetBufferf,           loadfunc("alGetBufferf",            arg));
    SALAD_RESOLVE(SALAD_alGetBufferfv,          loadfunc("alGetBufferfv",           arg));
    SALAD_RESOLVE(SALAD_alGetBufferi,           loadfunc("alGetBufferi",            arg));
    SALAD_RESOLVE(SALAD_alGetBufferiv,          loadfunc("alGetBufferiv",           arg));
    SALAD_RESOLVE(SALAD_alGetDouble,            loadfunc("alGetDouble",             arg));
    SALAD_RESOLVE(SALAD_alGetDoublev,           loadfunc("alGetDoublev",            arg));
    SALAD_RESOLVE(SALAD_alGetEnumValue,         loadfunc("alGetEnumValue",          arg));
    SALAD_RESOLVE(SALAD_alGetError,             loadfunc("alGetError",              arg));
    SALAD_RESOLVE(SALAD_alGetFloat,             loadfunc("alGetFloat",              arg));
    SALAD_RESOLVE(SALAD_alGetFloatv,            loadfunc("alGetFloatv",             arg));
    SALAD_RESOLVE(SALAD_alGetInteger,           loadfunc("alGetInteger",            arg));
    SALAD_RESOLVE(SALAD_alGetIntegerv,          loadfunc("alGetIntegerv",           arg));
    SALAD_RESOLVE(SALAD_alGetListener3f,        loadfunc("alGetListener3f",         arg));
    SALAD_RESOLVE(SALAD_alGetListener3i,        loadfunc("alGetListener3i",         arg));
    SALAD_RESOLVE(SALAD_alGetListenerf,         loadfunc("alGetListenerf",          arg));
    SALAD_RESOLVE(SALAD_alGetListenerfv,        loadfunc("alGetListenerfv",         arg));
    SALAD_RESOLVE(SALAD_alGetListeneri,         loadfunc("alGetListeneri",          arg));
    SALAD_RESOLVE(SALAD_alGetListeneriv,        loadfunc("alGetListeneriv",         arg));
    SALAD_RESOLVE(SALAD_alGetProcAddress,       loadfunc("alGetProcAddress",        arg));
    SALAD_RESOLVE(SALAD_alGetSource3f,          loadfunc("alGetSource3f",           arg));
    SALAD_RESOLVE(SALAD_alGetSource3i,          loadfunc("alGetSource3i",           arg));
    SALAD_RESOLVE(SALAD_alGetSourcef,           loadfunc("alGetSourcef",            arg));
    SALAD_RESOLVE(SALAD_alGetSourcefv,          loadfunc("alGetSourcefv",           arg));
    SALAD_RESOLVE(SALAD_alGetSourcei,           loadfunc("alGetSourcei",            arg));
    SALAD_RESOLVE(SALAD_alGetSourceiv,          loadfunc("alGetSourceiv",           arg));
    SALAD_RESOLVE(SALAD_alGetString,            loadfunc("alGetString",             arg));
    SALAD_RESOLVE(SALAD_alIsBuffer,             loadfunc("alIsBuffer",              arg));
    SALAD_RESOLVE(SALAD_alIsEnabled,            loadfunc("alIsEnabled",             arg));
    SALAD_RESOLVE(SALAD_alIsExtensionPresent,   loadfunc("alIsExtensionPresent",    arg));
    SALAD_RESOLVE(SALAD_alIsSource,             loadfunc("alIsSource",              arg));
    SALAD_RESOLVE(SALAD_alListener3f,           loadfunc("alListener3f",            arg));
    SALAD_RESOLVE(SALAD_alListener3i,           loadfunc("alListener3i",            arg));
    SALAD_RESOLVE(SALAD_alListenerf,            loadfunc("alListenerf",             arg));
    SALAD_RESOLVE(SALAD_alListenerfv,           loadfunc("alListenerfv",            arg));
    SALAD_RESOLVE(SALAD_alListeneri,            loadfunc("alListeneri",             arg));
    SALAD_RESOLVE(SALAD_alListeneriv,           loadfunc("alListeneriv",            arg));
    SALAD_RESOLVE(SALAD_alSource3f,             loadfunc("alSource3f",              arg));
    SALAD_RESOLVE(SALAD_alSource3i,             loadfunc("alSource3i",              arg));
    SALAD_RESOLVE(SALAD_alSourcef,              loadfunc("alSourcef",               arg));
    SALAD_RESOLVE(SALAD_alSourcefv,             loadfunc("alSourcefv",              arg));
    SALAD_RESOLVE(SALAD_alSourcei,              loadfunc("alSourcei",               arg));
    SALAD_RESOLVE(SALAD_alSourceiv,             loadfunc("alSourceiv",              arg));
    SALAD_RESOLVE(SALAD_alSourcePause,          loadfunc("alSourcePause",           arg));
    SALAD_RESOLVE(SALAD_alSourcePausev,         loadfunc("alSourcePausev",          arg));
    SALAD_RESOLVE(SALAD_alSourcePlay,           loadfunc("alSourcePlay",            arg));
    SALAD_RESOLVE(SALAD_alSourcePlayv,          loadfunc("alSourcePlayv",           arg));
    SALAD_RESOLVE(SALAD_alSourceQueueBuffers,   loadfunc("alSourceQueueBuffers",    arg));
    SALAD_RESOLVE(SALAD_alSourceRewind,         loadfunc("alSourceRewind",          arg));
    SALAD_RESOLVE(SALAD_alSourceRewindv,        loadfunc("alSourceRewindv",         arg));
    SALAD_RESOLVE(SALAD_alSourceStop,           loadfunc("alSourceStop",            arg));
    SALAD_RESOLVE(SALAD_alSourceStopv,          loadfunc("alSourceStopv",           arg));
    SALAD_RESOLVE(SALAD_alSourceUnqueueBuffers, loadfunc("alSourceUnqueueBuffers",  arg));

    /* Context OpenAL functions */
    SALAD_RESOLVE(SALAD_alcCaptureCloseDevice,  loadfunc("alcCaptureCloseDevice",   arg));
    SALAD_RESOLVE(SALAD_alcCaptureOpenDevice,   loadfunc("alcCaptureOpenDevice",    arg));
    SALAD_RESOLVE(SALAD_alcCaptureSamples,      loadfunc("alcCaptureSamples",       arg));
    SALAD_RESOLVE(SALAD_alcCaptureStart,        loadfunc("alcCaptureStart",         arg));
    SALAD_RESOLVE(SALAD_alcCaptureStop,         loadfunc("alcCaptureStop",          arg));
    SALAD_RESOLVE(SALAD_alcCloseDevice,         loadfunc("alcCloseDevice",          arg));
    SALAD_RESOLVE(SALAD_alcCreateContext,       loadfunc("alcCreateContext",        arg));
    SALAD_RESOLVE(SALAD_alcDestroyContext,      loadfunc("alcDestroyContext",       arg));
    SALAD_RESOLVE(SALAD_alcGetContextsDevice,   loadfunc("alcGetContextsDevice",    arg));
    SALAD_RESOLVE(SALAD_alcGetCurrentContext,   loadfunc("alcGetCurrentContext",    arg));
    SALAD_RESOLVE(SALAD_alcGetEnumValue,        loadfunc("alcGetEnumValue",         arg));
    SALAD_RESOLVE(SALAD_alcGetError,            loadfunc("alcGetError",             arg));
    SALAD_RESOLVE(SALAD_alcGetIntegerv,         loadfunc("alcGetIntegerv",          arg));
    SALAD_RESOLVE(SALAD_alcGetProcAddress,      loadfunc("alcGetProcAddress",       arg));
    SALAD_RESOLVE(SALAD_alcGetString,           loadfunc("alcGetString",            arg));
    SALAD_RESOLVE(SALAD_alcIsExtensionPresent,  loadfunc("alcIsExtensionPresent",   arg));
    SALAD_RESOLVE(SALAD_alcMakeContextCurrent,  loadfunc("alcMakeContextCurrent",   arg));
    SALAD_RESOLVE(SALAD_alcOpenDevice,          loadfunc("alcOpenDevice",           arg));
    SALAD_RESOLVE(SALAD_alcProcessContext,      loadfunc("alcProcessContext",       arg));
    SALAD_RESOLVE(SALAD_alcSuspendContext,      loadfunc("alcSuspendContext",       arg));

    /* Extension: ALC_EXT_EFX
     * NOTE: these are not checked if SALAD_PARANOID
     * is defined; checking if they're present is on user code */
    SALAD_alDeleteEffects   = loadfunc("alDeleteEffects",   arg);
    SALAD_alDeleteFilters   = loadfunc("alDeleteFilters",   arg);
    SALAD_alEffectf         = loadfunc("alEffectf",         arg);
    SALAD_alEffectfv        = loadfunc("alEffectfv",        arg);
    SALAD_alEffecti         = loadfunc("alEffecti",         arg);
    SALAD_alEffectiv        = loadfunc("alEffectiv",        arg);
    SALAD_alFilterf         = loadfunc("alFilterf",         arg);
    SALAD_alFilterfv        = loadfunc("alFilterfv",        arg);
    SALAD_alFilteri         = loadfunc("alFilteri",         arg);
    SALAD_alFilteriv        = loadfunc("alFilteriv",        arg);
    SALAD_alGenEffects      = loadfunc("alGenEffects",      arg);
    SALAD_alGenFilters      = loadfunc("alGenFilters",      arg);
    SALAD_alGetEffectf      = loadfunc("alGetEffectf",      arg);
    SALAD_alGetEffectfv     = loadfunc("alGetEffectfv",     arg);
    SALAD_alGetEffecti      = loadfunc("alGetEffecti",      arg);
    SALAD_alGetEffectiv     = loadfunc("alGetEffectiv",     arg);
    SALAD_alGetFilterf      = loadfunc("alGetFilterf",      arg);
    SALAD_alGetFilterfv     = loadfunc("alGetFilterfv",     arg);
    SALAD_alGetFilteri      = loadfunc("alGetFilteri",      arg);
    SALAD_alGetFilteriv     = loadfunc("alGetFilteriv",     arg);
    SALAD_alIsEffect        = loadfunc("alIsEffect",        arg);
    SALAD_alIsFilter        = loadfunc("alIsFilter",        arg);

    return 1;
}
