#pragma once
#include <vector>
#include <cstdint>

std::vector<uint8_t> rfEncrypt(std::vector<uint8_t> data, int rails);
std::vector<uint8_t> rfDoubleEncrypt(std::vector<uint8_t> data, int rails1, int rails2);

