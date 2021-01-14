/******************************************************************************/
/*!
 * @file   bf_platform.h
 * @author Shareef Abdoul-Raheem (http://blufedora.github.io/)
 * @brief
 *   Handles the windowing and event layer for the engine.
 * 
 *   References:
 *     [https://blog.kowalczyk.info/article/j/guide-to-predefined-macros-in-c-compilers-gcc-clang-msvc-etc..html]
 *
 * @version 0.0.1
 * @date    2020-07-05
 *
 * @copyright Copyright (c) 2020 Shareef Abdoul-Raheem
 */
/******************************************************************************/
#ifndef BF_PLATFORM_H
#define BF_PLATFORM_H

#include "bf_platform_export.h"

#include <stddef.h> /* size_t   */
#include <stdint.h> /* uint32_t */

/* clang-format off */
#ifndef BIFROST_PLATFORM_WIN32
#define BIFROST_PLATFORM_WIN32         0
#define BIFROST_PLATFORM_WIN64         0
#define BIFROST_PLATFORM_WINDOWS       0
#define BIFROST_PLATFORM_ANDROID       0
#define BIFROST_PLATFORM_MACOS         0
#define BIFROST_PLATFORM_IOS           0
#define BIFROST_PLATFORM_EMSCRIPTEN    0
#define BIFROST_PLATFORM_LINUX         0
#endif

#ifdef _WIN32

  #undef BIFROST_PLATFORM_WINDOWS
  #define BIFROST_PLATFORM_WINDOWS 1

  #undef BIFROST_PLATFORM_WIN32
  #define BIFROST_PLATFORM_WIN32 1

  #ifdef _WIN64
    #undef BIFROST_PLATFORM_WIN64
    #define BIFROST_PLATFORM_WIN64 1
  #endif

#elif __APPLE__
  #include "TargetConditionals.h"
  #if TARGET_IPHONE_SIMULATOR
    #undef BIFROST_PLATFORM_IOS
    #define BIFROST_PLATFORM_IOS 1
  #elif TARGET_OS_IPHONE
    #undef BIFROST_PLATFORM_IOS
    #define BIFROST_PLATFORM_IOS 1
  #elif TARGET_OS_MAC
    #undef BIFROST_PLATFORM_MACOS
    #define BIFROST_PLATFORM_MACOS 1
	#define BIFROST_PLATFORM_MACOS 1
  #else
    #   error "Unknown Apple platform"
  #endif
#elif __ANDROID__
  #undef BIFROST_PLATFORM_ANDROID
  #define BIFROST_PLATFORM_ANDROID 1
#elif __linux

  #undef BIFROST_PLATFORM_LINUX
  #define BIFROST_PLATFORM_LINUX 1
  // linux
#elif __unix // all unices not caught above
             // Unix
#elif __posix
// POSIX
#endif

#if __EMSCRIPTEN__
  #undef BIFROST_PLATFORM_EMSCRIPTEN
  #define BIFROST_PLATFORM_EMSCRIPTEN 1
#endif

#if (BIFROST_PLATFORM_IOS || BIFROST_PLATFORM_ANDROID)
#define BIFROST_OPENGL_ES 1
#define BIFROST_OPENGL    0
#else
#define BIFROST_OPENGL_ES 0
#define BIFROST_OPENGL    1
#endif
/* clang-format on */

#if __cplusplus
extern "C" {
#endif
struct bfEvent;
typedef struct bfEvent bfEvent;

typedef enum
{
  BIFROST_PLATFORM_GFX_VUlKAN,
  BIFROST_PLATFORM_GFX_OPENGL,

} bfPlatformGfxAPI;

typedef void* (*bfPlatformAllocator)(void* ptr, size_t old_size, size_t new_size, void* user_data);

typedef struct
{
  int                 argc;      /*!< Argc from the main, could be 0.                                                                                             */
  char**              argv;      /*!< Argv from the main, allowed to be NULL                                                                                      */
  bfPlatformAllocator allocator; /*!< Custom allocator if you wanted to control where the platform gets it's memory from, if NULL will use the default allocator. */
  void*               user_data; /*!< User data for keeping tack some some global state, could be NULL.                                                           */

} bfPlatformInitParams;

struct bfWindow;
typedef void (*bfWindowEventFn)(struct bfWindow* window, bfEvent* event);
typedef void (*bfWindowFrameFn)(struct bfWindow* window);

#define k_bfPlatformForceEnumSize (0x7FFFFFFF)

enum
{
  /* Window Creation Flags */
  k_bfWindowFlagIsResizable       = (1 << 0),
  k_bfWindowFlagIsVisible         = (1 << 1),
  k_bfWindowFlagIsDecorated       = (1 << 2),
  k_bfWindowFlagIsMaximizedOnShow = (1 << 3),
  k_bfWindowFlagIsFloating        = (1 << 4),
  k_bfWindowFlagIsFocused         = (1 << 5),
  k_bfWindowFlagIsFocusedOnShow   = (1 << 6),

  /* Meta Flags */
  k_bfWindowFlagsDefault = k_bfWindowFlagIsVisible |
                           k_bfWindowFlagIsResizable |
                           k_bfWindowFlagIsMaximizedOnShow |
                           k_bfWindowFlagIsFocused |
                           k_bfWindowFlagIsDecorated,
  k_bfWindowFlagsMax = k_bfPlatformForceEnumSize
};

typedef struct bfWindow
{
  void*           handle;
  void*           user_data;
  void*           renderer_data;
  bfWindowEventFn event_fn;
  bfWindowFrameFn frame_fn;

} bfWindow; /*!< Base class for the window, each backend can extend it in various ways. */

typedef enum
{
  BF_CLIPBOARD_UTF8_TEXT,

} bfClipbardDataType;

typedef int Boolean;

/*!
 * @brief
 *   Initializes the underlying platform abstraction layer.
 *   Should be called before any other subsystem.
 *
 * @param params
 *   Some configuration parameters for startup.
 *
 * @return
 *   0 (false) - If there were was an error initializing.
 *   1 (true)  - Successfully initialized.
 */
BF_PLATFORM_API int  bfPlatformInit(bfPlatformInitParams params);
BF_PLATFORM_API void bfPlatformPumpEvents(void);
BF_PLATFORM_API bfWindow*        bfPlatformCreateWindow(const char* title, int width, int height, uint32_t flags);
BF_PLATFORM_API Boolean          bfWindow_wantsToClose(bfWindow* self);
BF_PLATFORM_API void             bfWindow_show(bfWindow* self);
BF_PLATFORM_API void             bfWindow_getPos(bfWindow* self, int* x, int* y);
BF_PLATFORM_API void             bfWindow_setPos(bfWindow* self, int x, int y);
BF_PLATFORM_API void             bfWindow_getSize(bfWindow* self, int* x, int* y);
BF_PLATFORM_API void             bfWindow_setSize(bfWindow* self, int x, int y);
BF_PLATFORM_API void             bfWindow_focus(bfWindow* self);
BF_PLATFORM_API int              bfWindow_isFocused(bfWindow* self);
BF_PLATFORM_API int              bfWindow_isMinimized(bfWindow* self);
BF_PLATFORM_API int              bfWindow_isHovered(bfWindow* self);
BF_PLATFORM_API void             bfWindow_setTitle(bfWindow* self, const char* title);
BF_PLATFORM_API void             bfWindow_setAlpha(bfWindow* self, float value);
BF_PLATFORM_API void             bfPlatformDestroyWindow(bfWindow* window);
BF_PLATFORM_API void             bfPlatformQuit(void);
BF_PLATFORM_API float            bfPlatformGetDPIScale(void);  // TODO(SR): Bad API cuz it assumes one monitor.
BF_PLATFORM_API const char*      bfPlatformGetClipboard(bfClipbardDataType type);
BF_PLATFORM_API Boolean          bfPlatformSetClipboard(bfClipbardDataType type, const char* data, size_t data_length);
BF_PLATFORM_API bfPlatformGfxAPI bfPlatformGetGfxAPI(void);
BF_PLATFORM_API void*            bfPlatformDefaultAllocator(void* ptr, size_t old_size, size_t new_size, void* user_data);
BF_PLATFORM_API void*            bfPlatformAlloc(size_t size);
BF_PLATFORM_API void*            bfPlatformRealloc(void* ptr, size_t old_size, size_t new_size);
BF_PLATFORM_API void             bfPlatformFree(void* ptr, size_t old_size);
BF_PLATFORM_API void             bfPlatformDoMainLoop(bfWindow* main_window);

#if __cplusplus
}
#endif

#endif /* BF_PLATFORM_H */

/******************************************************************************/
/*
  MIT License

  Copyright (c) 2020 Shareef Abdoul-Raheem

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
*/
/******************************************************************************/
