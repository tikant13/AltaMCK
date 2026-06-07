#include "LCG_generation_key.h"
#include "LCG_decryption.h"
#include <cstdint>
#include <vector>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

std::vector <uint8_t> decrypt(const std::vector <uint8_t>& data, const std::vector <uint64_t>& keys){
    std::vector <uint8_t> result(data.size());
    for(size_t i = 0; i < data.size(); ++i){
        result[i] = data[i]^(keys[i] & 0xFF);
    }    
    return result;
}

extern "C" EXPORT int decrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    std::vector<uint8_t> in(input.data, input.data + input.size);
    uint64_t m = 2147483647, a = 1664525, c = 1013904223, Xn = 12345;
    auto stream = generation_key(m, Xn, a, c, in.size());
    auto result = decrypt(in, stream);
    output->size = result.size();
    std::copy(result.begin(), result.end(), output->data);
    return 0;
}