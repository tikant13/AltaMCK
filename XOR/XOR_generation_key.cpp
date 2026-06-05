#include "XOR_generation_key.h"
#include <fstream>

std::vector <uint8_t> generate_key(size_t key_size){
    std::vector <uint8_t> key(key_size);
    std::ifstream urandom("/dev/urandom", std::ios::binary);
    urandom.read(reinterpret_cast<char*>(key.data()), key_size);
    return key;
}

void save_key(const std::vector <uint8_t>& key, const std::string& path){
        std::ofstream file(path, std::ios::binary);
        file.write(reinterpret_cast<const char*>(key.data()), key.size());

}
