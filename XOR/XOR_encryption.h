#ifndef XOR_ENCRYPTION_H
#define XOR_ENCRYPTION_H

#include <vector>
#include <cstdint>

std::vector <uint8_t> encrypt(const std::vector<uint8_t>& data, const std::vector <uint8_t>& key);

#endif