#ifndef LCG_ENCRYPTION_H
#define LCG_ENCRYPTION_H

#include <cstdint>
#include <vector>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

struct ConstBuffer { const uint8_t* data; size_t size; };
struct MutBuffer   { uint8_t* data;       size_t size; };

std::vector<uint8_t> encrypt(const std::vector <uint8_t>& data, const std::vector <uint64_t>& keys);

extern "C" EXPORT int encrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output);

#endif