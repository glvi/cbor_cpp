
AM_DEFAULT_SOURCE_EXT = .cpp

lib_LTLIBRARIES = libglvi_cbor.la

libglvi_cbor_la_SOURCES = \
    glvi_cbor.cpp \
    glvi_cbor_u64.cpp \
    glvi_cbor_uint.cpp \
    glvi_cbor_nint.cpp \
    glvi_cbor_bstr.cpp \
    glvi_cbor_tstr.cpp \
    glvi_cbor_array.cpp \
    glvi_cbor_map.cpp \
    glvi_cbor_tag.cpp \
    glvi_cbor_simple.cpp \
    glvi_cbor_float.cpp \
    glvi_cbor_value.cpp \
    glvi_cbor_scanner.cpp

libglvi_cbor_ladir = $(includeDir)

libglvi_cbor_la_HEADERS = \
    glvi_cbor.h \
    glvi_cbor_array.h \
    glvi_cbor_bstr.h \
    glvi_cbor_float.h \
    glvi_cbor_int.h \
    glvi_cbor_map.h \
    glvi_cbor_nint.h \
    glvi_cbor_scanner_helper.h \
    glvi_cbor_scanner.h \
    glvi_cbor_simple.h \
    glvi_cbor_tag.h \
    glvi_cbor_tstr.h \
    glvi_cbor_u64.h \
    glvi_cbor_uint.h \
    glvi_cbor_value.h

check_PROGRAMS = \
    glvi_cbor_bstr_tests \
    glvi_cbor_tstr_tests \
    glvi_cbor_value_tests

glvi_cbor_bstr_tests_LDADD = -lglvi_cbor
glvi_cbor_tstr_tests_LDADD = -lglvi_cbor
glvi_cbor_value_tests_LDADD = -lglvi_cbor

TESTS = $(check_PROGRAMS)
