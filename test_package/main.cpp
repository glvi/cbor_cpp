#include "glvi_cbor.hh"
#include <vector>
#include <string>

int main() {
    glvi_cbor();

    std::vector<std::string> vec;
    vec.push_back("test_package");

    glvi_cbor_print_vector(vec);
}
