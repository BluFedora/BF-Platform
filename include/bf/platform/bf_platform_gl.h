#ifndef BF_PLATFORM_GL_H
#define BF_PLATFORM_GL_H

#include "bf_platform.h"

#if BIFROST_PLATFORM_ANDROID
#include <GLES3/gl3.h>
typedef void* (* GLADloadproc)(const char *name);
#elif BIFROST_PLATFORM_IOS
#include <OpenGLES/ES3/gl.h>
typedef void* (* GLADloadproc)(const char *name);
#elif BIFROST_PLATFORM_MACOS
#include <OpenGL/gl3.h>
#include <glad/glad.h>
#elif BIFROST_PLATFORM_EMSCRIPTEN
#define GL_GLEXT_PROTOTYPES
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#elif BIFROST_PLATFORM_WINDOWS
#include <glad/glad.h>
#endif

#if __cplusplus
extern "C" {
#endif

BF_PLATFORM_API void         bfWindow_makeGLContextCurrent(bfWindow* self);
BF_PLATFORM_API GLADloadproc bfPlatformGetProcAddress(void);
BF_PLATFORM_API void         bfWindowGL_swapBuffers(bfWindow* self);

#if __cplusplus
}
#endif

#endif /* BF_PLATFORM_GL_H */

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
