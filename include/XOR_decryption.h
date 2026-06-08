#ifndef XOR_DECRYPTION_H
#define XOR_DECRYPTION_H

#include "XOR_encryption.h"
#include <vector>
#include <cstdint>

std::vector <uint8_t> decrypt(const std::vector<uint8_t>& data, const std::vector <uint8_t>& key);

extern "C" EXPORT int decrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output);

#endif