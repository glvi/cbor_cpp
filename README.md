[![C++ build](https://github.com/glvi/cbor_cpp/actions/workflows/cpp.yaml/badge.svg)](https://github.com/glvi/cbor_cpp/actions/workflows/cpp.yaml)
# cbor_cpp
C++ library for decoding Concise Binary Object Representation (CBOR).

Concise Binary Object Representation is an Internet Standard, see [STD94] below.

<dl>
  <dt>[STD94]</a></dt>
  <dd>
    <p>Internet Standard 94, &lt;https://www.rfc-editor.org/info/std94&gt;. At the time of writing, this STD comprises the following:</p>
    <p>Bormann, C. and P. Hoffman, "Concise Binary Object Representation (CBOR)", STD 94, RFC 8949, DOI 10.17487/RFC8949, December 2020, &lt;https://www.rfc-editor.org/info/rfc8949&gt;.</p>
  </dd>
</dl>

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

## Information security

The scanner has protection against excessive counts of bytes in byte
strings or text strings, or excessive counts of elements in arrays or
maps. These counts can be changed at the level of the `configure`
script. See `configure --help` for options
`--enable-cbor-bstr/tstr/array/map-count-max`. Default values are as follows.

Type  | Max   | Unit
:-----|------:|:------------------
bstr  | 32768 | bytes
tstr  | 32768 | bytes
array |  1024 | elements
map   |   512 | pairs of elements
