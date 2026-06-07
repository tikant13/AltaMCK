#include "XOR_encryption.h"
#include <vector>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

struct ConstBuffer { const uint8_t* data; size_t size; };
struct MutBuffer   { uint8_t* data;       size_t size; };

static std::vector<uint8_t> expend_key(const std::vector<uint8_t>& key, size_t target_size) {
    std::vector<uint8_t> expended(target_size);

    for (size_t i = 0; i < key.size() && i < target_size; ++i) {
        expended[i] = key[i];
    }

    for (size_t i = key.size(); i < target_size; ++i) {
        expended[i] = expended[i - 1] ^ key[i % key.size()];
    }

    return expended;
}

std::vector<uint8_t> encrypt(const std::vector<uint8_t>& data, const std::vector<uint8_t>& key) {
    auto expanded = expend_key(key, data.size());
    std::vector<uint8_t> result(data.begin(), data.end());

    for (size_t i = 0; i < data.size(); ++i) {
        result[i] = data[i] ^ expanded[i];
    }

    // каскадная зависимость: каждый байт зависит от предыдущего зашифрованного
    for (size_t i = 1; i < result.size(); ++i) {
        result[i] = result[i] ^ result[i - 1];
    }

    uint8_t checksum = 0;
    for (auto byte : data) {
        checksum ^= byte;
    }
    result.push_back(checksum);

    return result;
}

// C ABI: обёртка для загрузки через dlopen/LoadLibrary
extern "C" EXPORT int encrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output) {
    std::vector<uint8_t> k(key.data, key.data + key.size);
    std::vector<uint8_t> in(input.data, input.data + input.size);
    auto result = encrypt(in, k);
    output->size = result.size();
    std::copy(result.begin(), result.end(), output->data);
    return 0;
}