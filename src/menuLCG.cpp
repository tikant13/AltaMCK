#include "menuLCG.h"
#include "LCG_generation_key.h"
#include "LCG_encryption.h"
#include "LCG_decryption.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <iomanip>

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

static bool save_key(const std::string& path, uint64_t m, uint64_t c, uint64_t Xn) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(&m),  sizeof(m));
    file.write(reinterpret_cast<const char*>(&c),  sizeof(c));
    file.write(reinterpret_cast<const char*>(&Xn), sizeof(Xn));
    return file.good();
}

static bool load_key(const std::string& path, uint64_t& m, uint64_t& c, uint64_t& Xn) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    file.read(reinterpret_cast<char*>(&m),  sizeof(m));
    file.read(reinterpret_cast<char*>(&c),  sizeof(c));
    file.read(reinterpret_cast<char*>(&Xn), sizeof(Xn));
    return file.good();
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

void menuLCG() {
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    int input = -1;
    std::string inFile, outFile, keyFile;

    while (input != MenuLCG::LCG_EXIT) {
        std::cout << "\nШифр LCG\n";
        std::cout << "1. Генерация ключа\n";
        std::cout << "2. Шифрование\n";
        std::cout << "3. Расшифрование\n";
        std::cout << "0. Выйти в меню\n>> ";
        std::cin >> input;

        switch (input) {

            case MenuLCG::KeyGen: {
                std::cout << "Введите путь для сохранения ключа: ";
                std::cin >> keyFile;

                uint64_t m  = 2147483647ULL;
                uint64_t c  = (static_cast<uint64_t>(rand()) << 32 | rand()) % m;
                uint64_t Xn = (static_cast<uint64_t>(rand()) << 32 | rand()) % m;
                if (c  == 0) c  = 1013904223ULL;
                if (Xn == 0) Xn = 12345ULL;

                if (save_key(keyFile, m, c, Xn)) {
                    std::cout << "Ключ сохранён: " << keyFile << "\n";
                    std::cout << "  m  = " << m  << "\n";
                    std::cout << "  c  = " << c  << "\n";
                    std::cout << "  Xn = " << Xn << "\n";
                } else {
                    std::cout << "Ошибка: не удалось сохранить ключ\n";
                }
                break;
            }

            case MenuLCG::Encryption: {
                std::cout << "Введите путь к файлу ключа: ";
                std::cin >> keyFile;
                uint64_t m, c, Xn;
                if (!load_key(keyFile, m, c, Xn)) {
                    std::cout << "Ошибка: не удалось загрузить ключ из " << keyFile << "\n";
                    break;
                }

                std::cout << "Введите путь входного файла: ";
                std::cin >> inFile;
                std::cout << "Введите путь выходного файла: ";
                std::cin >> outFile;

                {
                    auto data      = read_file(inFile);
                    auto stream    = generation_key(m, Xn, c, data.size());
                    auto encrypted = encrypt(data, stream);

                    write_file_hex(outFile, encrypted);  // <- hex-текст в файл
                    print_result(encrypted, "Зашифрованные данные");
                    std::cout << "Зашифровано -> " << outFile << "\n";
                }
                break;
            }

            case MenuLCG::Decipher: {
                std::cout << "Введите путь к файлу ключа: ";
                std::cin >> keyFile;
                uint64_t m, c, Xn;
                if (!load_key(keyFile, m, c, Xn)) {
                    std::cout << "Ошибка: не удалось загрузить ключ из " << keyFile << "\n";
                    break;
                }

                std::cout << "Введите путь зашифрованного файла: ";
                std::cin >> inFile;
                std::cout << "Введите путь выходного файла: ";
                std::cin >> outFile;

                {
                    auto data      = read_file_hex(inFile);  // <- читаем hex обратно
                    auto stream    = generation_key(m, Xn, c, data.size());
                    auto decrypted = decrypt(data, stream);

                    write_file(outFile, decrypted);  // <- расшифрованный — обычный файл
                    print_result(decrypted, "Расшифрованные данные");
                    std::cout << "Расшифровано -> " << outFile << "\n";
                }
                break;
            }

            case MenuLCG::LCG_EXIT:
                break;

            default:
                std::cout << "Ошибка! Такого действия нет\n";
        }
    }
}