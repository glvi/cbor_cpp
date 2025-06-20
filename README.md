[![C++ build](https://github.com/glvi/cbor_cpp/actions/workflows/cpp.yaml/badge.svg)](https://github.com/glvi/cbor_cpp/actions/workflows/cpp.yaml)
# cbor_cpp
C++ library for decoding Concise Binary Object Representation

## Tested on

- macOS 26
- macOS 15.5
- ubuntu 24.04

## Dependencies

### Requires

- A C++ compiler supporting C++23
- [GNU Autoconf](https://www.gnu.org/software/autoconf)
- [GNU Automake](https://www.gnu.org/software/automake)
- [GNU Autogen](https://www.gnu.org/software/autogen)
- [GNU Libtool](https://www.gnu.org/software/libtool)
- [DejaGnu](https://www.gnu.org/software/dejagnu)

## Check out
```sh
git clone https://github.com/glvi/cbor_cpp
```

## Build from source
```sh
autoreconf -fis
mkdir -p build
(cd build && ../configure CXXFLAGS=-std=c++23 CPPFLAGS=-I/usr/local/include)
gmake -j -C build check
```

You can choose any other name for the `build` directory.

Modify `CXXFLAGS` accordingly, if your compiler uses a different
directive to support C++23, or if you want to specify additional
compiler flags.

Modify `CPPFLAGS` accordingly, if your installation of DejaGnu resides
in a different location, or if you want to specify additional
preprocessor flags.
