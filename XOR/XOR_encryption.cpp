#include "XOR_encryption.h"
#include <vector>

std::vector <uint8_t> expend_key(const std::vector <uint8_t>& key, size_t target_size){
    std::vector<uint8_t> expended(target_size);
    for(size_t i = 0; i < key.size() && i < target_size; ++i){
        expended[i] = key[i];
    }

    for(size_t i = key.size(); i < target_size; ++i){
        expended[i] = expended[i-1]^key[i % key.size()];
    }
    return expended;
}

std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
    auto expanded = expend_key(key, data.size());
    std::vector<uint8_t> result(data.begin(), data.end());
    
    // проход 1 — XOR с расширенным ключом
    for(size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ expanded[i];
    }
    
    // проход 2 — каскадный XOR
    for(size_t i = 1; i < result.size(); ++i) {
        result[i] = result[i] ^ result[i-1];
    }
    uint8_t checksum = 0;
    for(auto byte : data) {
        checksum ^= byte;
    }
    result.push_back(checksum);
    return result;
}