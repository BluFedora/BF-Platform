﻿################################################################################
#                                                                              #
#                             BF PLATFORM PROJECT                              #
#                                                                              #
################################################################################
###          CMakeList.txt : Platform Sub Project of the Engine              ###
################################################################################

cmake_minimum_required(VERSION 3.8)

project(BF_Platform VERSION 0.0.2 DESCRIPTION "The platform abstraction layer sub project.")

option(BF_OPT_PLATFORM_GLFW   "This will use GLFW for windowing and system functions." ON)
option(BF_OPT_PLATFORM_SDL    "This will use SDL for windowing and system functions."  OFF)
option(BF_OPT_PLATFORM_QT     "This will use QT for windowing and system functions."   OFF)
option(BF_OPT_GRAPHICS_VULKAN "Vulkan will be used as the graphics Backend"            ON)
option(BF_OPT_GRAPHICS_OPENGL "OpenGL will be used as the graphics Backend"            OFF)

if (WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
     set(BF_PLATFORM_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/win64)
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
     set(BF_PLATFORM_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/win32)
  endif()
elseif(APPLE)
  set(BF_PLATFORM_LIB_DIR ${PROJECT_SOURCE_DIR}/lib/macOS)
endif()

if (WIN32)
  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
     set(BF_PLATFORM_LIB ${PROJECT_SOURCE_DIR}/lib/win64)
  elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
     set(BF_PLATFORM_LIB ${PROJECT_SOURCE_DIR}/lib/win32)
  endif()
elseif(APPLE)
  set(BF_PLATFORM_LIB ${PROJECT_SOURCE_DIR}/lib/macOS)
endif()

set(BF_PLATFORM_SOURCE_FILES   "${PROJECT_SOURCE_DIR}/src/bf_platform.c")
set(BF_PLATFORM_LIB_FILES "")

if(BF_OPT_PLATFORM_GLFW)
  set(BF_PLATFORM_SOURCE_FILES
    ${BF_PLATFORM_SOURCE_FILES}
    "${PROJECT_SOURCE_DIR}/src/bf_platform_glfw.c"
  )

  if(EMSCRIPTEN)
    # TODO(SR): Add Compiler flags here.
  elseif (WIN32)
    set(BF_PLATFORM_LIB_FILES
      ${BF_PLATFORM_LIB_FILES}
      "${BF_PLATFORM_LIB_DIR}/glfw3dll.lib"
    )
  elseif(APPLE)
    set(BF_PLATFORM_LIB_FILES
      ${BF_PLATFORM_LIB_FILES}
      "${BF_PLATFORM_LIB_DIR}/libglfw.3.dylib"
    )
  endif()

elseif(BF_OPT_PLATFORM_SDL)
  set(BF_PLATFORM_SOURCE_FILES
    ${BF_PLATFORM_SOURCE_FILES}
    "src/bf_platform_sdl.c"
  )

  if(EMSCRIPTEN)
  elseif (WIN32)
    set(BF_PLATFORM_LIB_FILES
      ${BF_PLATFORM_LIB_FILES}
      "${BF_PLATFORM_LIB}/SDL2.lib"
    )
  elseif(APPLE)
    set(BF_PLATFORM_LIB_FILES
      ${BF_PLATFORM_LIB_FILES}
      "${BF_PLATFORM_LIB_DIR}/libglfw.3.dylib"
    )
  endif()

elseif(BF_OPT_PLATFORM_QT)
  set(BF_PLATFORM_SOURCE_FILES
    ${BF_PLATFORM_SOURCE_FILES}
    "src/bf_platform_qt.c"
  )
else()
endif()

add_library(
  "${PROJECT_NAME}_static" 
  STATIC
    ${BF_PLATFORM_SOURCE_FILES}
)

target_include_directories(
  "${PROJECT_NAME}_static"
  PUBLIC 
    "${PROJECT_SOURCE_DIR}/include"
    "${PROJECT_SOURCE_DIR}/lib/include"
)

if(BF_OPT_GRAPHICS_VULKAN)
  find_package(Vulkan)

  if(${Vulkan_FOUND})
    target_include_directories(
      "${PROJECT_NAME}_static" 
      PRIVATE 
        "${Vulkan_INCLUDE_DIR}"
     )
  endif()

  target_compile_definitions(
    "${PROJECT_NAME}_static"
    PRIVATE
      GLFW_INCLUDE_VULKAN
      BF_PLATFORM_USE_VULKAN=1
  )
else()
  target_compile_definitions(
    "${PROJECT_NAME}_static"
    PRIVATE
      GLFW_INCLUDE_VULKAN
      BF_PLATFORM_USE_OPENGL=1
  )
endif()

target_link_libraries(
  "${PROJECT_NAME}_static"
  PUBLIC 
    "${BF_PLATFORM_LIB_FILES}"
)

target_compile_definitions(
  "${PROJECT_NAME}_static"
  PUBLIC 
    BIFROST_PLATFORM_EXPORT_STATIC
)

set_target_properties(
  "${PROJECT_NAME}_static" 
  PROPERTIES 
    VERSION                       ${PROJECT_VERSION}
    PUBLIC_HEADER                 include/bifrost/platform/bifrost_platform.h
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
    OUTPUT_NAME                   bf.PlatformLIB
)

# Shared Library

if(NOT EMSCRIPTEN)
  add_library(
      "${PROJECT_NAME}_shared"
      SHARED
        ${BF_PLATFORM_SOURCE_FILES}
  )

  target_include_directories(
      "${PROJECT_NAME}_shared"
      PUBLIC 
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/lib/include"
  )

  target_link_libraries(
      "${PROJECT_NAME}_shared"
      PUBLIC 
        "${BF_PLATFORM_LIB_FILES}"
  )

  target_compile_definitions("${PROJECT_NAME}_shared" PRIVATE BIFROST_PLATFORM_EXPORT)

  set_target_properties(
    "${PROJECT_NAME}_shared"
    PROPERTIES
      OUTPUT_NAME bf.PlatformDLL
  )

if(BF_OPT_GRAPHICS_VULKAN)
  if(${Vulkan_FOUND})
    target_include_directories(
      "${PROJECT_NAME}_shared" 
      PRIVATE 
        "${Vulkan_INCLUDE_DIR}"
     )
  endif()

  target_compile_definitions(
    "${PROJECT_NAME}_shared"
    PRIVATE
      GLFW_INCLUDE_VULKAN
    PUBLIC
      BF_PLATFORM_USE_VULKAN=1
  )
else()
  target_compile_definitions(
    "${PROJECT_NAME}_shared"
    PUBLIC
      BF_PLATFORM_USE_OPENGL=1
  )
endif()
  set_target_properties(
    "${PROJECT_NAME}_shared" 
    PROPERTIES
      VERSION       ${PROJECT_VERSION}
      PUBLIC_HEADER include/bifrost/platform/bifrost_platform.h
      OUTPUT_NAME   bf.PlatformDLL
  )
endif()

# Copy over needed DLLs into the executable's output directory

if(WIN32)
  configure_file("${BF_PLATFORM_LIB_DIR}/glfw3.dll"   "${EXECUTABLE_OUTPUT_PATH}glfw3.dll" COPYONLY)
endif()

# Test Application

add_executable(
  bfPlatformExample

  "test/platform_test.c"
)

target_link_libraries(
  bfPlatformExample

  PUBLIC
    BF_Platform_shared
)
