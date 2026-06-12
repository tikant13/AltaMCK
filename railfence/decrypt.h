#pragma once
#include <vector>
#include <cstdint>

std::vector<uint8_t> rfDecrypt(std::vector<uint8_t> data, int rails);
std::vector<uint8_t> rfDoubleDecrypt(std::vector<uint8_t> data, int rails1, int rails2);
