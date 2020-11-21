#include "bf/platform/bf_platform.h"

#include "bf/platform/bf_platform_event.h"

#if BIFROST_PLATFORM_EMSCRIPTEN
#include <emscripten.h>
#endif

#include <stdlib.h> /* realloc */
#include <string.h> /* memcpy  */

bfPlatformInitParams g_BifrostPlatform;

bfPlatformGfxAPI bfPlatformGetGfxAPI(void)
{
#if defined(BF_PLATFORM_USE_VULKAN)
  return BIFROST_PLATFORM_GFX_VUlKAN;
#elif defined(BF_PLATFORM_USE_OPENGL)
  return BIFROST_PLATFORM_GFX_OPENGL;
#else
#error "One of these should be defined"
#endif
}

void* bfPlatformDefaultAllocator(void* ptr, size_t old_size, size_t new_size, void* user_data)
{
  (void)user_data;
  (void)old_size;

  /*
    NOTE(Shareef):
      "if new_size is zero, the behavior is implementation defined
      (null pointer may be returned in which case the old memory block may or may not be freed),
      or some non-null pointer may be returned that may
      not be used to access storage."
  */
  if (new_size == 0u)
  {
    free(ptr);
    ptr = NULL;
  }
  else
  {
    void* const new_ptr = realloc(ptr, new_size);

    if (!new_ptr)
    {
      /*
        NOTE(Shareef):
          As to not leak memory, realloc says:
            "If there is not enough memory,
            the old memory block is not freed and null pointer is returned."
      */
      free(ptr);
    }

    ptr = new_ptr;
  }

  return ptr;
}

void* bfPlatformAlloc(size_t size)
{
  return g_BifrostPlatform.allocator(NULL, 0u, size, g_BifrostPlatform.user_data);
}

void* bfPlatformRealloc(void* ptr, size_t old_size, size_t new_size)
{
  return g_BifrostPlatform.allocator(ptr, old_size, new_size, g_BifrostPlatform.user_data);
}

void bfPlatformFree(void* ptr, size_t old_size)
{
  (void)g_BifrostPlatform.allocator(ptr, old_size, 0u, g_BifrostPlatform.user_data);
}


static void bfPlatformDoMainLoopImpl(void* arg)
{
  bfWindow* main_window = (bfWindow*)arg;

  bfPlatformPumpEvents();

  if (main_window->frame_fn)
  {
    main_window->frame_fn(main_window);
  }

#if BIFROST_PLATFORM_EMSCRIPTEN
  // Needed only if " ctx_attribs.explicitSwapControl" is true.
  // emscripten_webgl_commit_frame();
  // emscripten_cancel_main_loop();
#endif
}

void bfPlatformDoMainLoop(bfWindow* main_window)
{
#if BIFROST_PLATFORM_EMSCRIPTEN
  emscripten_set_main_loop_arg(&bfPlatformDoMainLoopImpl, main_window, 0 /* could also be 60fps */, 1);
#else
  while (!bfWindow_wantsToClose(main_window))
  {
    bfPlatformDoMainLoopImpl(main_window);
  }
#endif
}

/* Events */

bfKeyboardEvent bfKeyboardEvent_makeKeyMod(int key, uint8_t modifiers)
{
  bfKeyboardEvent self;
  self.key       = key;
  self.modifiers = modifiers;

  return self;
}

bfKeyboardEvent bfKeyboardEvent_makeCodepoint(unsigned codepoint)
{
  bfKeyboardEvent self;
  self.codepoint = codepoint;
  self.modifiers = 0x0;

  return self;
}

bfMouseEvent bfMouseEvent_make(int x, int y, uint8_t target_button, bfButtonFlags button_state)
{
  bfMouseEvent self;
  self.x             = x;
  self.y             = y;
  self.target_button = target_button;
  self.button_state  = button_state;

  return self;
}

bfScrollWheelEvent bfScrollWheelEvent_make(double x, double y)
{
  bfScrollWheelEvent self;
  self.x = x;
  self.y = y;

  return self;
}

bfWindowEvent bfWindowEvent_make(int width, int height, bfWindowFlags state)
{
  bfWindowEvent self;
  self.width  = width;
  self.height = height;
  self.state  = state;

  return self;
}

struct bfEvent bfEvent_makeImpl(bfEventType type, uint8_t flags, const void* data, size_t data_size)
{
#if __cplusplus
  bfEvent self{type, flags};
#else
  bfEvent self;
  self.type  = type;
  self.flags = flags;
#endif

  memcpy(&self.keyboard, data, data_size);

  return self;
}

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
