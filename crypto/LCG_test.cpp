#include "LCG_generation_key.h"
#include "LCG_encryption.h"
#include "LCG_decryption.h"
#include <iostream>
#include <cstdlib>
#include <string>

bool test1(){
    std::string text = "Hello LCG!";
    std::vector<uint8_t> data(text.begin(), text.end());

    uint64_t m = 2147483647, c = 1013904223, Xn = 12345;

    auto stream = generation_key(m, Xn, c, data.size());
    auto encrypted = encrypt(data, stream);
    auto stream2 = generation_key(m, Xn, c, encrypted.size());
    auto decrypted = decrypt(encrypted, stream2);
    return data == decrypted;
}

bool test2(){
    std::vector<uint8_t> data{0, 0, 0, 0, 0};

    uint64_t m = 2147483647, c = 1013904223, Xn = 12345;

    auto stream = generation_key(m, Xn, c, data.size());
    auto encrypted = encrypt(data, stream);
    auto stream2 = generation_key(m, Xn, c, encrypted.size());
    auto decrypted = decrypt(encrypted, stream2);
    return data == decrypted;
}

bool test3(){
    std::vector<uint8_t> data(1000);
    for(size_t i = 0; i < data.size(); ++i){
        data[i] = rand() % 255;
    }
    uint64_t m = 2147483647, c = 1013904223, Xn = 12345;

    auto stream = generation_key(m, Xn, c, data.size());
    auto encrypted = encrypt(data, stream);
    auto stream2 = generation_key(m, Xn, c, encrypted.size());
    auto decrypted = decrypt(encrypted, stream2);
    return data == decrypted;
}

int main(){
    std::cout << "TEST No1: " << (test1() ? "OK" : "FAIL") << "\n";
    std::cout << "TEST No2: " << (test2() ? "OK" : "FAIL") << "\n";
    std::cout << "TEST No3: " << (test3() ? "OK" : "FAIL") << "\n";
    return 0;
}