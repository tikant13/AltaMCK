#include "LCG_generation_key.h"
#include "LCG_encryption.h"
#include <cstdint>
#include <vector>

std::vector<uint8_t> encrypt(const std::vector <uint8_t>& data, const std::vector <uint64_t>& keys){
    std::vector <uint8_t> result(data.size());
    for(size_t i = 0; i < data.size(); ++i){
        result[i] = data[i]^(keys[i] & 0xFF);
    }
    return result;
}

extern "C" EXPORT int lcg_encrypt_data(ConstBuffer /*key*/, ConstBuffer input, MutBuffer* output) {
    std::vector<uint8_t> in(input.data, input.data + input.size);
    uint64_t m = 2147483647, c = 1013904223, Xn = 12345;
    auto stream = generation_key(m, Xn, c, in.size());
    auto result = encrypt(in, stream);
    output->size = result.size();
    std::copy(result.begin(), result.end(), output->data);
    return 0;
}