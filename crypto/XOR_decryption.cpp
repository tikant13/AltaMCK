#include "XOR_decryption.h"
#include <vector>
#include <iostream>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

static std::vector <uint8_t> expend_key(const std::vector <uint8_t>& key, size_t target_size){
    std::vector<uint8_t> expended(target_size);
    for(size_t i = 0; i < key.size() && i < target_size; ++i){
        expended[i] = key[i];
    }

    for(size_t i = key.size(); i < target_size; ++i){
        expended[i] = expended[i-1]^key[i % key.size()];
    }
    return expended;
}

std::vector<uint8_t> decrypt(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
    std::vector<uint8_t> result(data.begin(), data.end());
    
    uint8_t checksum = result.back();
    result.pop_back();
    auto expanded = expend_key(key, result.size());

    // проход 1 — обратный каскад
    for(size_t i = result.size() - 1; i > 0; --i) {
        result[i] = result[i] ^ result[i-1];
    }
    
    // проход 2 — XOR с расширенным ключом
    for(size_t i = 0; i < result.size(); ++i) {
        result[i] = result[i] ^ expanded[i];
    }
    uint8_t computed = 0;
    for(auto byte : result) {
        computed ^= byte;
    }
    if (computed != checksum){
        std::cerr << "Данные повреждены!" << std::endl;
    }
        
    return result;
}

// C ABI: обёртка для загрузки через dlopen/LoadLibrary
extern "C" EXPORT int decrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    std::vector<uint8_t> k(key.data, key.data + key.size);
    std::vector<uint8_t> in(input.data, input.data + input.size);
    auto result = decrypt(in, k);
    output->size = result.size();
    std::copy(result.begin(), result.end(), output->data);
    return 0;
}