# README

foo-cpp is a CMake, Google Test Framework, C++14 project template that allows
you to get going quickly.

Just clone the repo as shown below and replace `foo` with your project's name
and start coding.


## Supported Compilers

* Visual Studio 2013
* Visual Studio 2015
* Clang 3.5 with libc++
* GCC 4.8 with libstdc++


## Supported Operating Systems

* Microsoft Windows 10
* Mac OS X 10.11
* RHEL 7
* Ubuntu 14.04 LTS and 16.04 LTS


## Source Code

foo-cpp's source code is available as a Git repository. To obtain it, type:

```bash
git clone --recursive https://github.com/bkircher/foo-cpp.git
```


## Building

To build the accompanied tests with debugging symbols and Address Sanitizer
enabled do something like this

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

<!-- vim: et sw=4 ts=4:
-->
