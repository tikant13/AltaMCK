#ifndef LCG_GENERATION_KEY_H
#define LCG_GENERATION_KEY_H

#include <cstdint>
#include <vector>

std::vector <uint64_t>  generation_key(uint64_t m, uint64_t Xn, uint64_t c, size_t size);

#endif