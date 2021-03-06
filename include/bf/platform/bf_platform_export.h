/******************************************************************************/
/*!
* @file   bf_platform_export.h
* @author Shareef Raheem (http://blufedora.github.io)
*
* @brief
*  Contains the macros for exporting symbols for the shared library.
*
* @version 0.0.1-beta
* @date    2020-06-21
*
* @copyright Copyright (c) 2019-2020 Shareef Abdoul-Raheem
*/
/******************************************************************************/
#ifndef BF_PLATFORM_EXPORT_H
#define BF_PLATFORM_EXPORT_H

#if __cplusplus
extern "C" {
#endif

#if defined _WIN32 || defined __CYGWIN__
#ifdef BIFROST_PLATFORM_EXPORT

#ifdef __GNUC__
#define BF_PLATFORM_API __attribute__((dllexport))
#else
#define BF_PLATFORM_API __declspec(dllexport)
#endif

#elif defined(BIFROST_PLATFORM_EXPORT_STATIC)

#define BF_PLATFORM_API

#else

#ifdef __GNUC__
#define BF_PLATFORM_API __attribute__((dllimport))
#else
#define BF_PLATFORM_API __declspec(dllimport)
#endif

#endif

#define BF_PLATFORM_NOAPI

#else
#if __GNUC__ >= 4
#define BF_PLATFORM_API   __attribute__((visibility("default")))
#define BF_PLATFORM_NOAPI __attribute__((visibility("hidden")))
#else
#define BF_PLATFORM_API
#define BF_PLATFORM_NOAPI
#endif
#endif

#if __cplusplus
}
#endif

#endif /* BF_PLATFORM_EXPORT_H */

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
