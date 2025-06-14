
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
    glvi_cbor.h \
    glvi_cbor_u64.h \
    glvi_cbor_uint.h \
    glvi_cbor_nint.h \
    glvi_cbor_int.h \
    glvi_cbor_bstr.h \
    glvi_cbor_tstr.h \
    glvi_cbor_array.h \
    glvi_cbor_map.h \
    glvi_cbor_tag.h \
    glvi_cbor_simple.h \
    glvi_cbor_float.h \
    glvi_cbor_value.h

libglvi_cbor_ladir = $(includedir)

check_PROGRAMS = \
    glvi_cbor_bstr_tests \
    glvi_cbor_tstr_tests

glvi_cbor_bstr_tests_SOURCES = glvi_cbor_bstr_tests.cpp

glvi_cbor_tstr_tests_SOURCES = glvi_cbor_tstr_tests.cpp

TESTS = $(check_PROGRAMS)
