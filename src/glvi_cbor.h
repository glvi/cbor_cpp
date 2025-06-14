#pragma once

#include <vector>
#include <string>


#ifdef _WIN32
  #define GLVI_CBOR_EXPORT __declspec(dllexport)
#else
  #define GLVI_CBOR_EXPORT
#endif

GLVI_CBOR_EXPORT void glvi_cbor();
GLVI_CBOR_EXPORT void glvi_cbor_print_vector(const std::vector<std::string> &strings);
