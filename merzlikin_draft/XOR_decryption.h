#ifndef XOR_DECRYPTION_H
#define XOR_DECRYPTION_H

#include <vector>
#include <cstdint>

std::vector <uint8_t> decrypt(const std::vector<uint8_t>& data, const std::vector <uint8_t>& key);

#endif