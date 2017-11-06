[![Build Status](https://travis-ci.org/bkircher/foo-cpp.svg?branch=master)](https://travis-ci.org/bkircher/foo-cpp)

# foo-cpp
foo-cpp is a CMake, Catch test framework, C++17 project template that allows
you to get going quickly. Basically cargo(1) for the poor.

Just clone the repo as shown below and replace `foo` with your project's name
and start coding.

## Supported Compilers
Should work with the latest compilers, starting with

* Visual Studio 2017
* Clang 5.0 with libc++
* GCC 7 with libstdc++

Note macOS users: only Clang installed via Homebrew package manager is known to
work. AppleClang is currently not supported.

## Step-by-step
Create a shallow clone of this repository with a history truncated to only the
last revision.

Suppose you want to name your project _bar_, do

```bash
git clone --depth=1 https://github.com/bkircher/foo-cpp.git bar
cd bar/
sed -i.bak 's/foo/bar/g' *.txt tests/*.cpp *.?pp
git mv tests/test_foo.cpp tests/test_bar.cpp
git mv foo.hpp bar.hpp
git clean -dxf
```

And your done.

## Building
Build everything with debugging symbols and Address Sanitizer enabled like this

```bash
mkdir build-dir
cd build-dir/
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZE=ON /path/to/source
make
```

On macOS, specify a cache file with `-C` to let CMake know where to find your
vanilla Clang compiler

```bash
cmake -C /path/to/source/macos-brewed-clang.txt /path/to/source
```

Type `make help` to see more configuration options.

Once you've build the project, you can run the tests with

```bash
./tests
```

Happy hacking!

<!-- vim: et sw=4 ts=4:
-->
