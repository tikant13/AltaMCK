#include "menuXOR.h"
#include "XOR_generation_key.h"
#include "XOR_encryption.h"
#include "XOR_decryption.h"
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

void menuXOR() {
    int input = -1;
    std::string inFile, outFile, keyFile;

    while(input != MenuXOR::XOR_EXIT) {
        std::cout << "\nШифр XOR\n";
        std::cout << "1. Генерация ключа\n";
        std::cout << "2. Шифрование\n";
        std::cout << "3. Расшифрование\n";
        std::cout << "0. Выйти в меню\n>> ";
        std::cin >> input;

        switch(input) {
            case MenuXOR::KeyGen: {
                std::cout << "Введите путь для сохранения ключа: ";
                std::cin >> keyFile;
                auto key = generate_key(32);
                save_key(key, keyFile);
                std::cout << "Ключ сохранён: " << keyFile << std::endl;
                break;
            }
            case MenuXOR::Encryption: {
                std::cout << "Введите имя входного файла: ";
                std::cin >> inFile;
                std::cout << "Введите имя выходного файла: ";
                std::cin >> outFile;
                std::cout << "Введите путь к ключу: ";
                std::cin >> keyFile;
                auto data = read_file(inFile);
                auto key = read_file(keyFile);
                auto encrypted = encrypt(data, key);
                write_file(outFile, encrypted);
                std::cout << "Зашифровано: " << outFile << std::endl;
                break;
            }
            case MenuXOR::Decipher: {
                std::cout << "Введите имя зашифрованного файла: ";
                std::cin >> inFile;
                std::cout << "Введите имя выходного файла: ";
                std::cin >> outFile;
                std::cout << "Введите путь к ключу: ";
                std::cin >> keyFile;
                auto data = read_file(inFile);
                auto key = read_file(keyFile);
                auto decrypted = decrypt(data, key);
                write_file(outFile, decrypted);
                std::cout << "Расшифровано: " << outFile << std::endl;
                break;
            }
            case MenuXOR::XOR_EXIT:
                break;
            default:
                std::cout << "Ошибка! Такого действия нет\n";
        }
    }
}