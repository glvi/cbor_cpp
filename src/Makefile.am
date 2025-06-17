
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

check_PROGRAMS = \
    glvi_cbor_bstr_tests \
    glvi_cbor_tstr_tests \
    glvi_cbor_value_tests

glvi_cbor_bstr_tests_LDADD = -lglvi_cbor
glvi_cbor_tstr_tests_LDADD = -lglvi_cbor
glvi_cbor_value_tests_LDADD = -lglvi_cbor

TESTS = $(check_PROGRAMS)
