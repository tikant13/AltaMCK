#include "menuXOR.h"
#include "XOR_generation_key.h"
#include "XOR_encryption.h"
#include "XOR_decryption.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

static std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

static void write_file_hex(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path);
    for (size_t i = 0; i < data.size(); ++i) {
        file << std::hex << std::uppercase
             << std::setw(2) << std::setfill('0')
             << static_cast<int>(data[i]);
        if ((i + 1) % 16 == 0)
            file << "\n";
        else
            file << " ";
    }
}

static std::vector<uint8_t> read_file_hex(const std::string& path) {
    std::ifstream file(path);
    std::vector<uint8_t> result;
    int val;
    while (file >> std::hex >> val) {
        result.push_back(static_cast<uint8_t>(val));
    }
    return result;
}

static void write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

static void print_result(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << "\n--- " << label << " (HEX) ---\n";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << std::hex << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0) std::cout << "\n";
    }
    std::cout << std::dec << "\n";

    std::cout << "--- " << label << " (TEXT) ---\n";
    bool printable = true;
    for (uint8_t b : data) {
        if (b < 0x20 && b != '\n' && b != '\r' && b != '\t') {
            printable = false;
            break;
        }
    }
    if (printable) {
        std::cout.write(reinterpret_cast<const char*>(data.data()), data.size());
        std::cout << "\n";
    } else {
        std::cout << "[Бинарные данные — текстовый вывод невозможен]\n";
    }
}

void menuXOR() {
    int input = -1;
    std::string inFile, outFile, keyFile;

    while (input != MenuXOR::XOR_EXIT) {
        std::cout << "\nШифр XOR\n";
        std::cout << "1. Генерация ключа\n";
        std::cout << "2. Шифрование\n";
        std::cout << "3. Расшифрование\n";
        std::cout << "0. Выйти в меню\n>> ";
        std::cin >> input;

        switch (input) {

                case MenuXOR::KeyGen: {
                std::cout << "Введите путь для сохранения ключа: ";
                std::cin >> keyFile;
                auto key = generate_key(32);
                save_key(key, keyFile);
                std::cout << "Ключ сохранён: " << keyFile << "\n";
                print_result(key, "Сгенерированный ключ");  // <- добавить эту строку
                break;
    }

            case MenuXOR::Encryption: {
                std::cout << "Введите путь входного файла: ";
                std::cin >> inFile;
                std::cout << "Введите путь выходного файла: ";
                std::cin >> outFile;
                std::cout << "Введите путь к ключу: ";
                std::cin >> keyFile;

                auto data      = read_file(inFile);
                auto key       = read_file(keyFile);
                auto encrypted = encrypt(data, key);

                write_file_hex(outFile, encrypted);  // hex-текст в файл
                print_result(encrypted, "Зашифрованные данные");
                std::cout << "Зашифровано -> " << outFile << "\n";
                break;
            }

            case MenuXOR::Decipher: {
                std::cout << "Введите путь зашифрованного файла: ";
                std::cin >> inFile;
                std::cout << "Введите путь выходного файла: ";
                std::cin >> outFile;
                std::cout << "Введите путь к ключу: ";
                std::cin >> keyFile;

                auto data      = read_file_hex(inFile);  // читаем hex обратно
                auto key       = read_file(keyFile);
                auto decrypted = decrypt(data, key);

                write_file(outFile, decrypted);  // расшифрованный — обычный бинарный/текстовый
                print_result(decrypted, "Расшифрованные данные");
                std::cout << "Расшифровано -> " << outFile << "\n";
                break;
            }

            case MenuXOR::XOR_EXIT:
                break;

            default:
                std::cout << "Ошибка! Такого действия нет\n";
        }
    }
}