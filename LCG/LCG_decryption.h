#ifndef LCG_DECRYPTION_H
#define LCG_DECRYPTION_H

#include "LCG_encryption.h"
#include <cstdint>
#include <vector>

std::vector<uint8_t> decrypt(const std::vector <uint8_t>& data, const std::vector <uint64_t>& keys);

extern "C" EXPORT int decrypt_data(ConstBuffer key, ConstBuffer input, MutBuffer* output);

#endif