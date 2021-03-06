#include "bf/platform/bf_platform.h"

#include "bf/platform/bf_platform_event.h"
#include "bf/platform/bf_platform_gl.h"
#include "bf/platform/bf_platform_vulkan.h"

#include <glfw/glfw3.h>

#if BIFROST_PLATFORM_EMSCRIPTEN
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>  // EmscriptenWebGLContextAttributes etc
#endif

#include <assert.h>

extern bfPlatformInitParams g_BifrostPlatform;

static bfWindow* s_MainWindow = NULL;

int bfPlatformInit(bfPlatformInitParams params)
{
  const int was_success = glfwInit() == GLFW_TRUE;

  if (was_success)
  {
    g_BifrostPlatform = params;

    if (!g_BifrostPlatform.allocator)
    {
      g_BifrostPlatform.allocator = &bfPlatformDefaultAllocator;
    }
  }

  return was_success;
}

void bfPlatformPumpEvents(void)
{
  glfwPollEvents();
}

#if 0

  bool startupGLFW(glfw::ControllerEventCallback* onControllerEvent, glfw::ErrorCallback* onGLFWError)
  {
    s_onControllerEvent = onControllerEvent;
    s_onGLFWError       = onGLFWError;

    glfwSetJoystickCallback(GLFW_onJoystickStateChanged);
    glfwSetErrorCallback(GLFW_errorCallback);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

    return false;
  }

  void GLFW_onJoystickStateChanged(int joy, int event)
  {
    if (s_onControllerEvent)
    {
      const char* name = glfwGetJoystickName(joy);

      ControllerEvent::Type evt_type = ControllerEvent::ON_CONTROLLER_CONNECTED;

      if (event == GLFW_CONNECTED)
      {
        evt_type = ControllerEvent::ON_CONTROLLER_CONNECTED;
      }
      else if (event == GLFW_DISCONNECTED)
      {
        evt_type = ControllerEvent::ON_CONTROLLER_DISCONNECTED;
      }

      s_onControllerEvent->safeCall(ControllerEvent(evt_type, name, joy));
    }
  }

static void GLFW_errorCallback(int errorCode, const char* message)
{
  if (s_onGLFWError)
  {
    s_onGLFWError->safeCall(errorCode, message);
  }
}
#endif

static bfWindow* getWindow(GLFWwindow* window)
{
  return (bfWindow*)glfwGetWindowUserPointer(window);
}

static void dispatchEvent(bfWindow* window, bfEvent event)
{
  if (window->event_fn)
  {
    event.receiver = window;
    window->event_fn(window, &event);
  }
}

static bfKeyModifiers convertKeyModifiers(int mods)
{
  bfKeyModifiers result = 0x0;

  if (mods & GLFW_MOD_SHIFT)
  {
    result |= BIFROST_KEY_FLAG_SHIFT;
  }

  if (mods & GLFW_MOD_CONTROL)
  {
    result |= BIFROST_KEY_FLAG_CONTROL;
  }

  if (mods & GLFW_MOD_ALT)
  {
    result |= BIFROST_KEY_FLAG_ALT;
  }

  if (mods & GLFW_MOD_SUPER)
  {
    result |= BIFROST_KEY_FLAG_SUPER;
  }

  if (mods & GLFW_MOD_CAPS_LOCK)
  {
    result |= BIFROST_KEY_FLAG_IS_CAPS_LOCKED;
  }

  if (mods & GLFW_MOD_NUM_LOCK)
  {
    result |= BIFROST_KEY_FLAG_IS_NUM_LOCKED;
  }

  return result;
}

static bfButtonFlags convertButtonState(GLFWwindow* window)
{
  bfButtonFlags result = 0x0;

  for (int i = GLFW_MOUSE_BUTTON_1; i <= GLFW_MOUSE_BUTTON_LAST; ++i)
  {
    if (glfwGetMouseButton(window, i) == GLFW_PRESS)
    {
      result |= (1 << i);
    }
  }

  return result;
}

static int convertKey(int key)
{
  switch (key)
  {
    case GLFW_KEY_ESCAPE: return BIFROST_KEY_ESCAPE;
    case GLFW_KEY_ENTER: return BIFROST_KEY_ENTER;
    case GLFW_KEY_TAB: return BIFROST_KEY_TAB;
    case GLFW_KEY_LEFT: return BIFROST_KEY_LEFT;
    case GLFW_KEY_RIGHT: return BIFROST_KEY_RIGHT;
    case GLFW_KEY_UP: return BIFROST_KEY_UP;
    case GLFW_KEY_DOWN: return BIFROST_KEY_DOWN;
    case GLFW_KEY_PAGE_UP: return BIFROST_KEY_PAGE_UP;
    case GLFW_KEY_PAGE_DOWN: return BIFROST_KEY_PAGE_DOWN;
    case GLFW_KEY_HOME: return BIFROST_KEY_HOME;
    case GLFW_KEY_END: return BIFROST_KEY_END;
    case GLFW_KEY_INSERT: return BIFROST_KEY_INSERT;
    case GLFW_KEY_DELETE: return BIFROST_KEY_DELETE;
    case GLFW_KEY_BACKSPACE: return BIFROST_KEY_BACKSPACE;
    case GLFW_KEY_KP_ENTER: return BIFROST_KEY_PAD_ENTER;

    case GLFW_KEY_SPACE: return BIFROST_KEY_SPACE;

    case GLFW_KEY_A: return BIFROST_KEY_A;
    case GLFW_KEY_B: return BIFROST_KEY_B;
    case GLFW_KEY_C: return BIFROST_KEY_C;
    case GLFW_KEY_D: return BIFROST_KEY_D;
    case GLFW_KEY_E: return BIFROST_KEY_E;
    case GLFW_KEY_F: return BIFROST_KEY_F;
    case GLFW_KEY_G: return BIFROST_KEY_G;
    case GLFW_KEY_H: return BIFROST_KEY_H;
    case GLFW_KEY_I: return BIFROST_KEY_I;
    case GLFW_KEY_J: return BIFROST_KEY_J;
    case GLFW_KEY_K: return BIFROST_KEY_K;
    case GLFW_KEY_L: return BIFROST_KEY_L;
    case GLFW_KEY_M: return BIFROST_KEY_M;
    case GLFW_KEY_N: return BIFROST_KEY_N;
    case GLFW_KEY_O: return BIFROST_KEY_O;
    case GLFW_KEY_P: return BIFROST_KEY_P;
    case GLFW_KEY_Q: return BIFROST_KEY_Q;
    case GLFW_KEY_R: return BIFROST_KEY_R;
    case GLFW_KEY_S: return BIFROST_KEY_S;
    case GLFW_KEY_T: return BIFROST_KEY_T;
    case GLFW_KEY_U: return BIFROST_KEY_U;
    case GLFW_KEY_V: return BIFROST_KEY_V;
    case GLFW_KEY_W: return BIFROST_KEY_W;
    case GLFW_KEY_X: return BIFROST_KEY_X;
    case GLFW_KEY_Y: return BIFROST_KEY_Y;
    case GLFW_KEY_Z: return BIFROST_KEY_Z;

    default: return key;
  }
}

static void GLFW_onKeyChanged(GLFWwindow* window, int key, int scan_code, int action, int mods)
{
  const int converted_key = convertKey(key);

  if (converted_key != -1)
  {
    bfEventType evt_type;

    switch (action)
    {
      case GLFW_PRESS:
      {
        evt_type = BIFROST_EVT_ON_KEY_DOWN;
        break;
      }
      case GLFW_REPEAT:
      {
        evt_type = BIFROST_EVT_ON_KEY_HELD;
        break;
      }
      case GLFW_RELEASE:
      {
        evt_type = BIFROST_EVT_ON_KEY_UP;
        break;
      }
      default:
        assert(!"bfInvalidDefaultCase()");
        return;
        // bfInvalidDefaultCase();
    }

    bfKeyboardEvent evt_data = bfKeyboardEvent_makeKeyMod(converted_key, convertKeyModifiers(mods));

    dispatchEvent(getWindow(window), bfEvent_make(evt_type, 0x0, evt_data));
  }
}

static void GLFW_onMousePosChanged(GLFWwindow* window, double x_pos, double y_pos)
{
  bfWindow* const w        = getWindow(window);
  bfMouseEvent    evt_data = bfMouseEvent_make((int)(x_pos), (int)(y_pos), BIFROST_BUTTON_NONE, convertButtonState(window));

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_MOUSE_MOVE, 0x0, evt_data));
}

static void GLFW_onMouseButtonChanged(GLFWwindow* window, int button, int action, int mods)
{
  bfWindow* const w = getWindow(window);

  double x_pos, y_pos;
  glfwGetCursorPos(window, &x_pos, &y_pos);

  bfEventType evt_type;

  switch (action)
  {
    case GLFW_PRESS:
    {
      evt_type = BIFROST_EVT_ON_MOUSE_DOWN;
      break;
    }
    case GLFW_RELEASE:
    {
      evt_type = BIFROST_EVT_ON_MOUSE_UP;
      break;
    }
    default:
      assert(!"bfInvalidDefaultCase()");
      return;
      // bfInvalidDefaultCase();
  }

  uint8_t target_button;

  switch (button)
  {
    case GLFW_MOUSE_BUTTON_LEFT:
    {
      target_button = BIFROST_BUTTON_LEFT;
      break;
    }
    case GLFW_MOUSE_BUTTON_MIDDLE:
    {
      target_button = BIFROST_BUTTON_MIDDLE;
      break;
    }
    case GLFW_MOUSE_BUTTON_RIGHT:
    {
      target_button = BIFROST_BUTTON_RIGHT;
      break;
    }
    case GLFW_MOUSE_BUTTON_4:
    {
      target_button = BIFROST_BUTTON_EXTRA0;
      break;
    }
    case GLFW_MOUSE_BUTTON_5:
    {
      target_button = BIFROST_BUTTON_EXTRA1;
      break;
    }
    case GLFW_MOUSE_BUTTON_6:
    {
      target_button = BIFROST_BUTTON_EXTRA2;
      break;
    }
    case GLFW_MOUSE_BUTTON_7:
    {
      target_button = BIFROST_BUTTON_EXTRA3;
      break;
    }
    case GLFW_MOUSE_BUTTON_8:
    {
      target_button = BIFROST_BUTTON_EXTRA4;
      break;
    }
    default:
    {
      target_button = BIFROST_BUTTON_NONE;
      break;
    }
  }

  bfMouseEvent evt_data = bfMouseEvent_make((int)(x_pos), (int)(y_pos), target_button, convertButtonState(window));

  dispatchEvent(w, bfEvent_make(evt_type, 0x0, evt_data));
}

void GLFW_onWindowFileDropped(GLFWwindow* window, int count, const char** paths)
{
  // auto& w = getWindow(window);

  // w.onFileDrop().safeCall(FileEvent(w, paths, count));
}

static void GLFW_onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
  bfWindow* const w = getWindow(window);

  bfWindowEvent evt_data = bfWindowEvent_make(width, height, BIFROST_WINDOW_IS_NONE);

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_WINDOW_RESIZE, 0x0, evt_data));
}

static void GLFW_onWindowRefresh(GLFWwindow* window)
{
  bfWindow* const w = getWindow(window);

  if (w->frame_fn)
  {
    w->frame_fn(w);
  }
}

static void GLFW_onWindowCharacterInput(GLFWwindow* window, unsigned int codepoint)
{
  bfWindow* const w = getWindow(window);

  bfKeyboardEvent evt_data = bfKeyboardEvent_makeCodepoint(codepoint);

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_KEY_INPUT, 0x0, evt_data));
}

static void GLFW_onScrollWheel(GLFWwindow* window, double x_offset, double y_offset)
{
  bfWindow* const w = getWindow(window);

  bfScrollWheelEvent evt_data = bfScrollWheelEvent_make(x_offset, y_offset);

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_SCROLL_WHEEL, 0x0, evt_data));
}

static void GLFW_onWindowIconify(GLFWwindow* window, int iconified)
{
  bfWindow* const w = getWindow(window);

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  bfWindowEvent evt_data = bfWindowEvent_make(width, height, iconified == GLFW_TRUE ? BIFROST_WINDOW_IS_MINIMIZED : 0x0);

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_WINDOW_MINIMIZE, 0x0, evt_data));
}

void GLFW_onWindowFocusChanged(GLFWwindow* window, int focused)
{
  bfWindow* const w = getWindow(window);

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  bfWindowEvent evt_data = bfWindowEvent_make(width, height, focused == GLFW_TRUE ? BIFROST_WINDOW_IS_FOCUSED : 0x0);

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_WINDOW_FOCUS_CHANGED, 0x0, evt_data));
}

static void GLFW_onWindowClose(GLFWwindow* window)
{
  bfWindow* const w = getWindow(window);

  int width, height;
  glfwGetWindowSize(window, &width, &height);

  bfWindowEvent evt_data = bfWindowEvent_make(width, height, BIFROST_WINDOW_IS_NONE);

  dispatchEvent(w, bfEvent_make(BIFROST_EVT_ON_WINDOW_CLOSE, 0x0, evt_data));

  // glfwSetWindowShouldClose(window, GLFW_FALSE);
}

bfWindow* bfPlatformCreateWindow(const char* title, int width, int height, uint32_t flags)
{
  bfWindow* const window = bfPlatformAlloc(sizeof(bfWindow));

  if (window)
  {
    const int is_opengl = bfPlatformGetGfxAPI() == BIFROST_PLATFORM_GFX_OPENGL;

    glfwWindowHint(GLFW_CLIENT_API, is_opengl ? GLFW_OPENGL_API : GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, flags & k_bfWindowFlagIsResizable ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_VISIBLE, flags & k_bfWindowFlagIsVisible ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, flags & k_bfWindowFlagIsDecorated ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_MAXIMIZED, flags & k_bfWindowFlagIsMaximizedOnShow ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_FLOATING, flags & k_bfWindowFlagIsFloating ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUSED, flags & k_bfWindowFlagIsFocused ? GLFW_TRUE : GLFW_FALSE);
    glfwWindowHint(GLFW_FOCUS_ON_SHOW, flags & k_bfWindowFlagIsFocusedOnShow ? GLFW_TRUE : GLFW_FALSE);

#if _WIN32
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
#elif __APPLE__
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_FALSE);
#endif

    GLFWwindow* const glfw_handle = glfwCreateWindow(width, height, title, NULL, is_opengl && s_MainWindow ? s_MainWindow->handle : NULL);

    window->handle        = glfw_handle;
    window->event_fn      = NULL;
    window->frame_fn      = NULL;
    window->user_data     = NULL;
    window->renderer_data = NULL;

    glfwSetWindowUserPointer(glfw_handle, window);
    glfwSetKeyCallback(glfw_handle, GLFW_onKeyChanged);
    glfwSetCursorPosCallback(glfw_handle, GLFW_onMousePosChanged);
    glfwSetMouseButtonCallback(glfw_handle, GLFW_onMouseButtonChanged);
    glfwSetDropCallback(glfw_handle, GLFW_onWindowFileDropped);
    glfwSetWindowSizeCallback(glfw_handle, GLFW_onWindowSizeChanged);
    glfwSetCharCallback(glfw_handle, GLFW_onWindowCharacterInput);
    glfwSetScrollCallback(glfw_handle, GLFW_onScrollWheel);
    glfwSetWindowIconifyCallback(glfw_handle, GLFW_onWindowIconify);
    glfwSetWindowFocusCallback(glfw_handle, GLFW_onWindowFocusChanged);
    glfwSetWindowCloseCallback(glfw_handle, GLFW_onWindowClose);
    glfwSetWindowRefreshCallback(glfw_handle, GLFW_onWindowRefresh);

    // TODO(SR): This should be user configurable.
    glfwSetWindowSizeLimits(glfw_handle, 300, 70, GLFW_DONT_CARE, GLFW_DONT_CARE);

    if (!s_MainWindow)
    {
      s_MainWindow = window;
    }
  }

  return window;
}

int bfWindow_wantsToClose(bfWindow* self)
{
  return glfwWindowShouldClose(self->handle);
}

void bfWindow_show(bfWindow* self)
{
  glfwShowWindow(self->handle);
}

void bfWindow_getPos(bfWindow* self, int* x, int* y)
{
  glfwGetWindowPos(self->handle, x, y);
}

void bfWindow_setPos(bfWindow* self, int x, int y)
{
  glfwSetWindowPos(self->handle, x, y);
}

void bfWindow_getSize(bfWindow* self, int* x, int* y)
{
#if BIFROST_PLATFORM_EMSCRIPTEN
  double w, h;
  emscripten_get_element_css_size("#canvas", &w, &h);

  *x = (int)w;
  *y = (int)h;
#else
  glfwGetWindowSize(self->handle, x, y);
#endif
}

void bfWindow_setSize(bfWindow* self, int x, int y)
{
  glfwSetWindowSize(self->handle, x, y);
}

void bfWindow_focus(bfWindow* self)
{
  glfwFocusWindow(self->handle);
}

int bfWindow_isFocused(bfWindow* self)
{
  return glfwGetWindowAttrib(self->handle, GLFW_FOCUSED) != 0;
}

int bfWindow_isMinimized(bfWindow* self)
{
  return glfwGetWindowAttrib(self->handle, GLFW_ICONIFIED) != 0;
}

int bfWindow_isHovered(bfWindow* self)
{
  return glfwGetWindowAttrib(self->handle, GLFW_HOVERED) != 0;
}

void bfWindow_setTitle(bfWindow* self, const char* title)
{
  glfwSetWindowTitle(self->handle, title);
}

void bfWindow_setAlpha(bfWindow* self, float value)
{
#ifndef __EMSCRIPTEN__  // (GLFW_VERSION_MAJOR * 1000 + GLFW_VERSION_MINOR * 100 >= 3300)
  // glfwSetWindowOpacity(self->handle, value);
#endif
}

void bfPlatformDestroyWindow(bfWindow* window)
{
  glfwDestroyWindow(window->handle);
  bfPlatformFree(window, sizeof(bfWindow));
}

void bfPlatformQuit(void)
{
  glfwTerminate();
}

float bfPlatformGetDPIScale(void)
{
  float        result  = 1.0f;
  GLFWmonitor* monitor = glfwGetPrimaryMonitor();
  float        xscale, yscale;

  glfwGetMonitorContentScale(monitor, &xscale, &yscale);

  if (xscale > 1.0f || yscale > 1.0f)
  {
    result = xscale;
    // glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
  }

  return result;
}

const char* bfPlatformGetClipboard(bfClipbardDataType type)
{
  assert(type == BF_CLIPBOARD_UTF8_TEXT && "Currently only supported data type.");
  return glfwGetClipboardString(NULL);
}

Boolean bfPlatformSetClipboard(bfClipbardDataType type, const char* data, size_t data_length)
{
  assert(type == BF_CLIPBOARD_UTF8_TEXT && "Currently only supported data type.");

  char* const data_nul_terminated = bfPlatformAlloc(data_length + 1);

  memcpy(data_nul_terminated, data, data_length);
  data_nul_terminated[data_length] = '\0';

  glfwSetClipboardString(NULL, data_nul_terminated);

  bfPlatformFree(data_nul_terminated, data_length + 1);

  return 1;
}

#if GLFW_INCLUDE_VULKAN
int bfWindow_createVulkanSurface(bfWindow* self, VkInstance instance, VkSurfaceKHR* out)
{
  return glfwCreateWindowSurface(instance, self->handle, NULL, out) == VK_SUCCESS;
}
#endif

void bfWindow_makeGLContextCurrent(bfWindow* self)
{
  glfwMakeContextCurrent(self->handle);
}

GLADloadproc bfPlatformGetProcAddress(void)
{
  return (GLADloadproc)glfwGetProcAddress;
}

void bfWindowGL_swapBuffers(bfWindow* self)
{
  glfwSwapBuffers(self->handle);
}

#if 0
#if BIFROST_PLATFORM_WINDOWS
#define GLFW_EXPOSE_NATIVE_WIN32
#elif BIFROST_PLATFORM_MACOS
// #define GLFW_EXPOSE_NATIVE_COCOA
#endif

#include <glfw/glfw3native.h>

#undef GLFW_EXPOSE_NATIVE_WIN32
#undef GLFW_EXPOSE_NATIVE_COCOA
#endif

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