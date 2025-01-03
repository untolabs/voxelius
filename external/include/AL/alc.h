/*
 * alc.h - Context OpenAL functions
 * Copyright (C) 2021-2025, Kirill Dmtirievich
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
#ifndef AL_ALC_H
#define AL_ALC_H 1
#include <AL/salad.h>

#define ALC_VERSION_0_1 1

#define ALC_APIENTRY SALAD_ALX_APIENTRY

typedef salad_float32_type  ALCfloat;
typedef salad_float64_type  ALCdouble;
typedef salad_int16_type    ALCshort;
typedef salad_int32_type    ALCint;
typedef salad_int32_type    ALCsizei;
typedef salad_int8_type     ALCbyte;
typedef salad_uint16_type   ALCushort;
typedef salad_uint32_type   ALCenum;
typedef salad_uint32_type   ALCuint;
typedef salad_uint8_type    ALCboolean;
typedef salad_uint8_type    ALCchar;
typedef salad_uint8_type    ALCubyte;
typedef void                ALCvoid;

struct ALCdevice;
struct ALCcontext;

typedef struct ALCdevice ALCdevice;
typedef struct ALCcontext ALCcontext;

#define ALC_INVALID (0x0000)
#define ALC_FALSE   (0x0000)
#define ALC_TRUE    (0x0001)

#define ALC_ALL_ATTRIBUTES                      (0x1003)
#define ALC_ALL_DEVICES_SPECIFIER               (0x1013)
#define ALC_ATTRIBUTES_SIZE                     (0x1002)
#define ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER    (0x0311)
#define ALC_CAPTURE_DEVICE_SPECIFIER            (0x0310)
#define ALC_CAPTURE_SAMPLES                     (0x0312)
#define ALC_DEFAULT_ALL_DEVICES_SPECIFIER       (0x1012)
#define ALC_DEFAULT_DEVICE_SPECIFIER            (0x1004)
#define ALC_DEVICE_SPECIFIER                    (0x1005)
#define ALC_ENUMERATE_ALL_EXT                   (0x0001)
#define ALC_EXT_CAPTURE                         (0x0001)
#define ALC_EXTENSIONS                          (0x1006)
#define ALC_FREQUENCY                           (0x1007)
#define ALC_INVALID_CONTEXT                     (0xA002)
#define ALC_INVALID_DEVICE                      (0xA001)
#define ALC_INVALID_ENUM                        (0xA003)
#define ALC_INVALID_VALUE                       (0xA004)
#define ALC_MAJOR_VERSION                       (0x1000)
#define ALC_MINOR_VERSION                       (0x1001)
#define ALC_MONO_SOURCES                        (0x1010)
#define ALC_NO_ERROR                            (0x0000)
#define ALC_OUT_OF_MEMORY                       (0xA005)
#define ALC_REFRESH                             (0x1008)
#define ALC_STEREO_SOURCES                      (0x1011)
#define ALC_SYNC                                (0x1009)

typedef ALCboolean(ALC_APIENTRY *PFN_alcCaptureCloseDevice)(ALCdevice *device);
typedef ALCboolean(ALC_APIENTRY *PFN_alcCloseDevice)(ALCdevice *device);
typedef ALCboolean(ALC_APIENTRY *PFN_alcIsExtensionPresent)(ALCdevice *device, const ALCchar *extname);
typedef ALCboolean(ALC_APIENTRY *PFN_alcMakeContextCurrent)(ALCcontext *context);
typedef ALCcontext *(ALC_APIENTRY *PFN_alcCreateContext)(ALCdevice *device, const ALCint *attrlist);
typedef ALCcontext *(ALC_APIENTRY *PFN_alcGetCurrentContext)(void);
typedef ALCdevice *(ALC_APIENTRY *PFN_alcCaptureOpenDevice)(const ALCchar *devicename, ALCuint frequency, ALCenum format, ALCsizei buffersize);
typedef ALCdevice *(ALC_APIENTRY *PFN_alcGetContextsDevice)(ALCcontext *context);
typedef ALCdevice *(ALC_APIENTRY *PFN_alcOpenDevice)(const ALCchar *devicename);
typedef ALCenum(ALC_APIENTRY *PFN_alcGetEnumValue)(ALCdevice *device, const ALCchar *enumname);
typedef ALCenum(ALC_APIENTRY *PFN_alcGetError)(ALCdevice *device);
typedef ALCvoid *(ALC_APIENTRY *PFN_alcGetProcAddress)(ALCdevice *device, const ALCchar *funcname);
typedef const ALCchar *(ALC_APIENTRY *PFN_alcGetString)(ALCdevice *device, ALCenum param);
typedef void(ALC_APIENTRY *PFN_alcCaptureSamples)(ALCdevice *device, ALCvoid *buffer, ALCsizei samples);
typedef void(ALC_APIENTRY *PFN_alcCaptureStart)(ALCdevice *device);
typedef void(ALC_APIENTRY *PFN_alcCaptureStop)(ALCdevice *device);
typedef void(ALC_APIENTRY *PFN_alcDestroyContext)(ALCcontext *context);
typedef void(ALC_APIENTRY *PFN_alcGetIntegerv)(ALCdevice *device, ALCenum param, ALCsizei size, ALCint *values);
typedef void(ALC_APIENTRY *PFN_alcProcessContext)(ALCcontext *context);
typedef void(ALC_APIENTRY *PFN_alcSuspendContext)(ALCcontext *context);

SALAD_EXTERN PFN_alcCaptureCloseDevice  SALAD_alcCaptureCloseDevice;
SALAD_EXTERN PFN_alcCaptureOpenDevice   SALAD_alcCaptureOpenDevice;
SALAD_EXTERN PFN_alcCaptureSamples      SALAD_alcCaptureSamples;
SALAD_EXTERN PFN_alcCaptureStart        SALAD_alcCaptureStart;
SALAD_EXTERN PFN_alcCaptureStop         SALAD_alcCaptureStop;
SALAD_EXTERN PFN_alcCloseDevice         SALAD_alcCloseDevice;
SALAD_EXTERN PFN_alcCreateContext       SALAD_alcCreateContext;
SALAD_EXTERN PFN_alcDestroyContext      SALAD_alcDestroyContext;
SALAD_EXTERN PFN_alcGetContextsDevice   SALAD_alcGetContextsDevice;
SALAD_EXTERN PFN_alcGetCurrentContext   SALAD_alcGetCurrentContext;
SALAD_EXTERN PFN_alcGetEnumValue        SALAD_alcGetEnumValue;
SALAD_EXTERN PFN_alcGetError            SALAD_alcGetError;
SALAD_EXTERN PFN_alcGetIntegerv         SALAD_alcGetIntegerv;
SALAD_EXTERN PFN_alcGetProcAddress      SALAD_alcGetProcAddress;
SALAD_EXTERN PFN_alcGetString           SALAD_alcGetString;
SALAD_EXTERN PFN_alcIsExtensionPresent  SALAD_alcIsExtensionPresent;
SALAD_EXTERN PFN_alcMakeContextCurrent  SALAD_alcMakeContextCurrent;
SALAD_EXTERN PFN_alcOpenDevice          SALAD_alcOpenDevice;
SALAD_EXTERN PFN_alcProcessContext      SALAD_alcProcessContext;
SALAD_EXTERN PFN_alcSuspendContext      SALAD_alcSuspendContext;

#define alcCaptureCloseDevice   SALAD_alcCaptureCloseDevice
#define alcCaptureOpenDevice    SALAD_alcCaptureOpenDevice
#define alcCaptureSamples       SALAD_alcCaptureSamples
#define alcCaptureStart         SALAD_alcCaptureStart
#define alcCaptureStop          SALAD_alcCaptureStop
#define alcCloseDevice          SALAD_alcCloseDevice
#define alcCreateContext        SALAD_alcCreateContext
#define alcDestroyContext       SALAD_alcDestroyContext
#define alcGetContextsDevice    SALAD_alcGetContextsDevice
#define alcGetCurrentContext    SALAD_alcGetCurrentContext
#define alcGetEnumValue         SALAD_alcGetEnumValue
#define alcGetError             SALAD_alcGetError
#define alcGetIntegerv          SALAD_alcGetIntegerv
#define alcGetProcAddress       SALAD_alcGetProcAddress
#define alcGetString            SALAD_alcGetString
#define alcIsExtensionPresent   SALAD_alcIsExtensionPresent
#define alcMakeContextCurrent   SALAD_alcMakeContextCurrent
#define alcOpenDevice           SALAD_alcOpenDevice
#define alcProcessContext       SALAD_alcProcessContext
#define alcSuspendContext       SALAD_alcSuspendContext

#endif /* AL_ALC_H */
