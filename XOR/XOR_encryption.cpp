#include "XOR_encryption.h"
#include <vector>

std::vector <uint8_t> encrypt(const std::vector<uint8_t>& data, const std::vector <uint8_t>& key){
    std::vector <uint8_t> result(data.size());
    for(size_t i = 0; i < data.size(); ++i){
        result[i] = data[i]^key[i%key.size()];
    }
    return result;
}
