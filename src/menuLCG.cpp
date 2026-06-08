#include "menuLCG.h"
#include "LCG_generation_key.h"
#include "LCG_encryption.h"
#include "LCG_decryption.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

static std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

static void write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void menuLCG() {
    int input = -1;
    std::string inFile, outFile, keyFile;

    while(input != MenuLCG::LCG_EXIT) {
        std::cout << "\nШифр LCG\n";
        std::cout << "1. Генерация ключа\n";
        std::cout << "2. Шифрование\n";
        std::cout << "3. Расшифрование\n";
        std::cout << "0. Выйти в меню\n>> ";
        std::cin >> input;

        switch(input) {
            case MenuLCG::KeyGen: {
                std::cout << "Введите путь для сохранения ключа: ";
                std::cin >> keyFile;
                uint64_t m = 2147483647, c = 1013904223, Xn = 12345;
                std::cout << "Параметры LCG: m=" << m << " c=" << c << " Xn=" << Xn << std::endl;
                break;
            }
            case MenuLCG::Encryption: {
                std::cout << "Введите имя входного файла: ";
                std::cin >> inFile;
                std::cout << "Введите имя выходного файла: ";
                std::cin >> outFile;
                uint64_t m = 2147483647, c = 1013904223, Xn = 12345;
                auto data = read_file(inFile);
                auto stream = generation_key(m, Xn, c, data.size());
                auto encrypted = encrypt(data, stream);
                write_file(outFile, encrypted);
                std::cout << "Зашифровано: " << outFile << std::endl;
                break;
            }
            case MenuLCG::Decipher: {
                std::cout << "Введите имя зашифрованного файла: ";
                std::cin >> inFile;
                std::cout << "Введите имя выходного файла: ";
                std::cin >> outFile;
                uint64_t m = 2147483647, c = 1013904223, Xn = 12345;
                auto data = read_file(inFile);
                auto stream = generation_key(m, Xn, c, data.size());
                auto decrypted = decrypt(data, stream);
                write_file(outFile, decrypted);
                std::cout << "Расшифровано: " << outFile << std::endl;
                break;
            }
            case MenuLCG::LCG_EXIT:
                break;
            default:
                std::cout << "Ошибка! Такого действия нет\n";
        }
    }
}