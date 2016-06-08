# Replace 'foo' with your project's name

cmake_minimum_required(VERSION 2.8.11)
project(foo CXX)

include(CheckCXXCompilerFlag)
if(NOT ${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    option(ENABLE_ASAN "Enable address sanitizer." OFF)
endif()
if(${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(_option_text "Link against libc++ instead of libstdc++.")
    if(${CMAKE_HOST_SYSTEM_NAME} STREQUAL "Darwin")
        option(ENABLE_LIBCXX ${_option_text} ON)
    else()
        option(ENABLE_LIBCXX ${_option_text} OFF)
    endif()
endif()

set(SANITIZE_CXXFLAGS)
set(SANITIZE_LDFLAGS)
if(ENABLE_ASAN)
    if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR
       "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        list(APPEND SANITIZE_CXXFLAGS
            "-fsanitize=address -fno-omit-frame-pointer")
        list(APPEND SANITIZE_LDFLAGS
            "-fsanitize=address")
    else()
        message(WARNING "Option ENABLE_ASAN only supported with clang and gcc.")
    endif()
endif()

set(GNUCXX_MINIMUM_VERSION "4.8")
set(CLANGCXX_MINIMUM_VERSION "3.5")
set(CXX_STANDARD_TAG "c++14")

# Helper macro. Add -O0 in Debug configuration to prevent any optimization.
# Makes gdb users much happier.
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
    if(ENABLE_LIBCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
    patch_cmake_cxx_debug_flags()
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GNUCXX_MINIMUM_VERSION)
        message(FATAL_ERROR
            "Minimum required g++ version: ${GNUCXX_MINIMUM_VERSION}")
    endif()

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS "4.9")
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -std=c++1y")
    else()
        set(CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS} -std=${CXX_STANDARD_TAG}")
    endif()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas")
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pedantic")
    endif()

    # Use colorized output on terminal if supported (GCC 4.9 onwards)
    CHECK_CXX_COMPILER_FLAG("-fdiagnostics-color=auto" GCC_HAS_COLOR)
    if("${GCC_HAS_COLOR}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
    endif()
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL "Intel")
    message(WARNING "Intel C++ not supported.")
elseif(${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)
    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 17.0)
        message(FATAL_ERROR "Visual Studio must be at least 2012")
    endif()

    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS
            "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    endif()
    set(CMAKE_CXX_FLAGS
        "${CMAKE_CXX_FLAGS} /D_SCL_SECURE_NO_WARNINGS /DNOMINMAX")

    if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS 18.0)
        # Make Google Test compile with Visual Studio 2012
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /D_VARIADIC_MAX=10")
    endif()
else()
    message(AUTHOR_WARNING
        "Could not determine compiler ID: ${CMAKE_CXX_COMPILER_ID}")
endif()

set(foo_SOURCES main.cpp)
set(foo_HEADERS foo.hpp)

set(GTEST_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty/gtest/googletest)
set(GTEST_INCLUDE_DIR ${GTEST_ROOT}/include)
set(GTEST_LIBRARIES gtest gtest_main)
add_subdirectory(${GTEST_ROOT})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${GTEST_INCLUDE_DIR})

# Feature test macros
include(CheckCXXSourceCompiles)

# Test support for noexcept specifier - negative: VS 2013
CHECK_CXX_SOURCE_COMPILES("
void func() noexcept { }
int main() { }
" HAS_NOEXCEPT_SPECIFIER)
if(HAS_NOEXCEPT_SPECIFIER)
    add_definitions(-DHAS_NOEXCEPT_SPECIFIER)
endif()

# N3656 - negative: libstdc++ prior 4.9.0
CHECK_CXX_SOURCE_COMPILES("
#include <memory>
int main() { std::unique_ptr<int> ptr = std::make_unique<int>(4); }
" HAS_MAKE_UNIQUE)
if(HAS_MAKE_UNIQUE)
    add_definitions(-DHAS_MAKE_UNIQUE)
endif()

# Default template arguments for function templates - negative: VS 2012
CHECK_CXX_SOURCE_COMPILES("
template <typename Fu = int> void func() { }
int main() { }
" HAS_DEFAULT_TEMPLATE_ARGS_FOR_FUNCTIONS)
if(HAS_DEFAULT_TEMPLATE_ARGS_FOR_FUNCTIONS)
    add_definitions(-DHAS_DEFAULT_TEMPLATE_ARGS_FOR_FUNCTIONS)
endif()

# Thread local storage - partial: VS 2012
CHECK_CXX_SOURCE_COMPILES("
int main() { thread_local int i; }
" HAS_THREAD_LOCAL_STORAGE)
if(HAS_THREAD_LOCAL_STORAGE)
    add_definitions(-DHAS_THREAD_LOCAL_STORAGE)
endif()

# N2326 defaulted and deleted functions - negative: VS 2012 of course
CHECK_CXX_SOURCE_COMPILES("
struct Fu { Fu() = default; Fu(const Fu&) = delete; };
int main() { }
" HAS_DEFAULT_AND_DELETE)
if(HAS_DEFAULT_AND_DELETE)
    add_definitions(-DHAS_DEFAULT_AND_DELETE)
endif()

# Variadic templates - negative: VS 2012
CHECK_CXX_SOURCE_COMPILES("
template <typename... T> void func(T...) { }
int main() { }
" HAS_VARIADIC_TEMPLATES)
if(HAS_VARIADIC_TEMPLATES)
    add_definitions(-DHAS_VARIADIC_TEMPLATES)
endif()

# Non-buggy <type_traits> implementation - negative VS 2013
CHECK_CXX_SOURCE_COMPILES("
#include <type_traits>
struct Fu { Fu(const Fu&) = delete; };
static_assert(!std::is_copy_constructible<Fu>::value, \"\");
int main() { }
" HAS_NON_BUGGY_TYPE_TRAITS)
if(HAS_NON_BUGGY_TYPE_TRAITS)
    add_definitions(-DHAS_NON_BUGGY_TYPE_TRAITS)
endif()

# N3671 - negative: _MSC_VER <= 1800, libstdc++ prior 4.9.0
CHECK_CXX_SOURCE_COMPILES("
#include <algorithm>
#include <cstring>
static const char source[] = \"boo\";
static const char dest[] = \"bar\";
int main() { std::equal(source, source + std::strlen(source),
                        dest, dest + std::strlen(dest)); }
" HAS_ROBUST_NONMODIFYING_SEQ_OPS)
if(HAS_ROBUST_NONMODIFYING_SEQ_OPS)
    add_definitions(-DHAS_ROBUST_NONMODIFYING_SEQ_OPS)
endif()

# Test support for initializer lists - negative: VS 2012
CHECK_CXX_SOURCE_COMPILES("
#include <vector>
int main() { std::vector<int> vec{1, 2, 3, 4, 5}; }
" HAS_INITIALIZER_LISTS)
if(HAS_INITIALIZER_LISTS)
    add_definitions(-DHAS_INITIALIZER_LISTS)
endif()

add_executable(foo ${foo_SOURCES})
set_target_properties(foo PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS "${SANITIZE_CXXFLAGS}"
    LINK_FLAGS "${SANITIZE_LDFLAGS}")

add_executable(tests tests/test_foo.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES})
set_target_properties(tests PROPERTIES
    LINKER_LANGUAGE CXX
    COMPILE_FLAGS "${SANITIZE_CXXFLAGS}"
    LINK_FLAGS "${SANITIZE_LDFLAGS}")

# vim:et sw=4 ts=4
