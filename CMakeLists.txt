# Replace 'foo' with your project's name

cmake_minimum_required(VERSION 3.1)
project(foo CXX)

set(GNUCXX_MINIMUM_VERSION "7.0")
set(CLANGCXX_MINIMUM_VERSION "5.0")
set(MSVC_MINIMUM_VERSION "19.0")
set(CXX_STANDARD_TAG "c++17")

if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  option(ENABLE_SANITIZE "Enable ASAN and UBSAN sanitizers." OFF)
endif()
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  option(ENABLE_PROFILE "Generate extra code to write profile information." OFF)
endif()

set(SANITIZE_CXXFLAGS)
set(SANITIZE_LDFLAGS)
if(ENABLE_SANITIZE)
  if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
     ${CMAKE_CXX_COMPILER_ID} STREQUAL "AppleClang" OR
     ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    list(APPEND SANITIZE_CXXFLAGS
      "-fsanitize=address,undefined -fno-omit-frame-pointer")
    list(APPEND SANITIZE_LDFLAGS
      "-fsanitize=address,undefined")
  else()
    message(WARNING "Option ENABLE_SANITIZE only supported with clang and gcc.")
  endif()
endif()

# Add -O0 _only_ in Debug configuration to prevent any optimization. This
# improves debugging experience (with gdb at least) but also eliminates any
# hardening. So make sure those binaries aren't shipped.
macro(patch_cmake_cxx_debug_flags)
  if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 ${CMAKE_CXX_FLAGS_DEBUG}")
  endif()
endmacro()

if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  patch_cmake_cxx_debug_flags()
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS CLANGCXX_MINIMUM_VERSION)
    message(FATAL_ERROR
      "Minimum required clang++ version: ${CLANGCXX_MINIMUM_VERSION}")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CXX_STANDARD_TAG}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fcolor-diagnostics")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  patch_cmake_cxx_debug_flags()
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GNUCXX_MINIMUM_VERSION)
    message(FATAL_ERROR
      "Minimum required g++ version: ${GNUCXX_MINIMUM_VERSION}")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CXX_STANDARD_TAG} -fdiagnostics-color=auto")

  if(ENABLE_PROFILE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
  endif()

  # https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
  string(CONCAT _gcc_extra_warnings
    " -Wcast-align"
    " -Wcast-qual"
    " -Wdisabled-optimization"
    " -Wdouble-promotion"
    " -Wduplicated-branches"
    " -Wduplicated-cond"
    " -Wformat=2"
    " -Wlogical-op"
    " -Wmissing-include-dirs"
    " -Wnull-dereference"
    " -Wold-style-cast"
    " -Wredundant-decls"
    " -Wrestrict"
    " -Wshadow")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${_gcc_extra_warnings}")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
  if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MSVC_MINIMUM_VERSION)
    message(FATAL_ERROR "Visual Studio must be at least 2017")
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:${CXX_STANDARD_TAG}")
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} /D_SCL_SECURE_NO_WARNINGS /DNOMINMAX")
else()
  message(AUTHOR_WARNING "Unsupported compiler: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Un-comment to print out compiler settings for different configuration types
#message(STATUS "Debug: ${CMAKE_CXX_FLAGS_DEBUG}")
#message(STATUS "Release: ${CMAKE_CXX_FLAGS_RELEASE}")
#message(STATUS "RelWithDebInfo: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
#message(STATUS "MinSizeRel: ${CMAKE_CXX_FLAGS_MINSIZEREL}")

set(foo_SOURCES main.cpp)
set(foo_HEADERS foo.hpp)

add_executable(foo ${foo_SOURCES})
set_target_properties(foo PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${SANITIZE_CXXFLAGS}"
  LINK_FLAGS "${SANITIZE_LDFLAGS}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(tests tests/test_foo.cpp)
set_target_properties(tests PROPERTIES
  LINKER_LANGUAGE CXX
  COMPILE_FLAGS "${SANITIZE_CXXFLAGS}"
  LINK_FLAGS "${SANITIZE_LDFLAGS}")
