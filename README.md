# README
foo-cpp is a CMake, Catch test framework, C++17 project template that allows
you to get going quickly. Basically cargo(1) for the poor.

Just clone the repo as shown below and replace `foo` with your project's name
and start coding.

## Supported Compilers
Should work with the following compilers

* Visual Studio 2017
* Clang 5.0 with libc++
* GCC 7 with libstdc++

Note macOS users: only clang++ installed via Homebrew package manager is known
to work. AppleClang is currently not supported.

## Source Code
Create a shallow clone of this repository with a history truncated to only the
last revision.

```bash
git clone --depth=1 https://github.com/bkircher/foo-cpp.git
```

## Building
Build the accompanied tests with debugging symbols and Address Sanitizer
enabled like this

```bash
mkdir build-dir
cd build-dir/
cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZE=ON /path/to/source
make
```

On macOS, specify a cache file with `-C` to let CMake know where to find your
vanilla clang compiler

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
