#include "XOR_generation_key.h"
#include "XOR_encryption.h"
#include "XOR_decryption.h"
#include <iostream>
#include <cstdlib>
#include <string>

bool test1(){
    std::string text = "Hello XOR!";
    std::vector<uint8_t> data(text.begin(), text.end());

    std::vector<uint8_t> key = {1, 2, 3, 4, 5};

    auto encrypted = encrypt(data, key);
    auto decrypted = decrypt(encrypted, key);
    return data == decrypted;
}

bool test2(){
    std::vector<uint8_t> data{0, 0, 0, 0, 0};
    std::vector<uint8_t> key = {255, 128, 64, 32, 16};

    auto encrypted = encrypt(data, key);
    auto decrypted = decrypt(encrypted, key);
    return data == decrypted;
}

bool test3(){
    std::vector<uint8_t> data(1000);
    for(size_t i = 0; i < data.size(); ++i){
        data[i] = rand() % 255;
    }
    std::vector<uint8_t> key = {10, 20, 30, 40, 50};

    auto encrypted = encrypt(data, key);
    auto decrypted = decrypt(encrypted, key);
    return data == decrypted;
}

bool test4(){
    std::vector<uint8_t> data(1000);
    for(size_t i = 0; i < data.size(); ++i){
        data[i] = rand() % 255;
    }
    std::vector<uint8_t> generation_key(32);

    auto encrypted = encrypt(data, generation_key);
    auto decrypted = decrypt(encrypted, generation_key);
    return data == decrypted;
}

int main(){
    std::cout << "TEST No1: " << (test1() ? "OK" : "FAIL") << "\n";
    std::cout << "TEST No2: " << (test2() ? "OK" : "FAIL") << "\n";
    std::cout << "TEST No3: " << (test3() ? "OK" : "FAIL") << "\n";
    std::cout << "TEST No4: " << (test3() ? "OK" : "FAIL") << "\n";
    return 0;
}