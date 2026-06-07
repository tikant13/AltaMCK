#include "XOR_generation_key.h"
#include <fstream>

std::vector <uint8_t> generate_key(size_t key_size){
    std::vector <uint8_t> key(key_size);
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    urandom.read(reinterpret_cast<char*>(key.data()), key_size);
    for(size_t i = key.size() - 1; i > 0; --i) {
    size_t j = key[i] % (i + 1);
    std::swap(key[i], key[j]);
    }
    uint8_t sum = 0;
    for(auto byte : key) sum += byte;
    for(size_t i = 0; i < key.size(); ++i) {
    key[i] ^= (sum + i) & 0xFF;//255 in 16cc; faster, then %256
}
    return key;
}

void save_key(const std::vector <uint8_t>& key, const std::string& path){
        std::ofstream file(path, std::ios::binary);
        file.write(reinterpret_cast<const char*>(key.data()), key.size());

}