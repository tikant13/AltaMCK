#ifndef XOR_GENERATION_KEY_H
#define XOR_GENERATION_KEY_H

#include <vector>
#include <cstdint>
#include <string>

std::vector <uint8_t> generate_key(size_t key_size);
void save_key(const std::vector <uint8_t>& key, const std::string& path);


#endif