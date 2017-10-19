# README
foo-cpp is a CMake, Google Test, C++17 project template that allows you to get
going quickly. Basically cargo(1) for the poor.

Just clone the repo as shown below and replace `foo` with your project's name
and start coding.

## Supported Compilers
Should work with the following compilers

* Visual Studio 2017
* Clang 5 with and without libc++
* GCC 7 with libstdc++

## Source Code
Create a shallow clone of this repository with a history truncated to only the
last revision.

```bash
git clone --recursive --depth=1 https://github.com/bkircher/foo-cpp.git
```

## Building
Build the accompanied tests with debugging symbols and Address Sanitizer
enabled like this

```bash
mkdir build-dir
cd build-dir/
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON /path/to/source
make
```

Type `make help` to see more configuration options.

Once you've build the project, you can run the tests with

```bash
./tests
```

Happy hacking!

<!-- vim: et sw=4 ts=4:
-->
