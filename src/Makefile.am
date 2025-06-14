
AUTOMAKE_OPTIONS = dejagnu

lib_LTLIBRARIES = libglvi_cbor.la

libglvi_cbor_la_SOURCES = \
    glvi_cbor.cpp \
    glvi_cbor_uint.cpp \
    glvi_cbor_nint.cpp \
    glvi_cbor_bstr.cpp \
    glvi_cbor_tstr.cpp \
    glvi_cbor_array.cpp \
    glvi_cbor_tag.cpp \
    glvi_cbor_value.cpp \
    $(libglvi_cbor_la_HEADERS)

libglvi_cbor_la_HEADERS = \
    glvi_cbor.hh \
    glvi_cbor_u64.hh \
    glvi_cbor_uint.hh \
    glvi_cbor_nint.hh \
    glvi_cbor_int.hh \
    glvi_cbor_bstr.hh \
    glvi_cbor_tstr.hh \
    glvi_cbor_array.hh \
    glvi_cbor_map.hh \
    glvi_cbor_tag.hh \
    glvi_cbor_simple.hh \
    glvi_cbor_float.hh \
    glvi_cbor_value.hh

libglvi_cbor_ladir = $(includedir)

check_PROGRAMS = \
    glvi_cbor_bstr_tests \
    glvi_cbor_tstr_tests

glvi_cbor_bstr_tests_SOURCES = glvi_cbor_bstr_tests.cpp

glvi_cbor_tstr_tests_SOURCES = glvi_cbor_tstr_tests.cpp

TESTS = $(check_PROGRAMS)
