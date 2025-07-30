#define PY_SSIZE_T_CLEAN
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>

// Include the Goldcoin scrypt implementation
extern "C" {
    #include "../src/crypto/scrypt.h"
}

namespace py = pybind11;

std::string getPoWHash(const std::string& input) {
    std::vector<char> output(32);  // scrypt output is 256 bits = 32 bytes
    
    // Call the goldcoin scrypt function
    scrypt_1024_1_1_256(input.c_str(), output.data());
    
    return std::string(output.begin(), output.end());
}

PYBIND11_MODULE(goldcoin_crypto, m) {
    m.doc() = "Goldcoin crypto functions";
    
    m.def("getPoWHash", &getPoWHash, "Calculate scrypt hash",
          py::arg("input"));
}