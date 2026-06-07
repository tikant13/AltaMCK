#ifndef XOR_DECRYPTION_H
#define XOR_DECRYPTION_H

#include <vector>
#include <cstdint>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

struct ConstBuffer { const uint8_t* data; size_t size; };
struct MutBuffer   { uint8_t* data;       size_t size; };

std::vector <uint8_t> decrypt(const std::vector<uint8_t>& data, const std::vector <uint8_t>& key);

extern "C" EXPORT int decrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output);

#endif