#include "menuXOR.h"
#include "XOR_generation_key.h"
#include "XOR_encryption.h"
#include "XOR_decryption.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <sstream>

// Читает содержимое файла в бинарном режиме и возвращает байты как вектор.
// ios::binary обязателен - без него на Windows \r\n преобразуется в \n
static std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

// Записывает байты в файл в виде hex-строки.
static void write_file_hex(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path);
    for (size_t i = 0; i < data.size(); ++i) {
        // std::setw(2) + std::setfill('0') гарантируют двузначный формат чтобы было так 0F, а не так F
        file << std::hex << std::uppercase
             << std::setw(2) << std::setfill('0')
             << static_cast<int>(data[i]);
        if ((i + 1) % 16 == 0)
            file << "\n";
        else
            file << " ";
    }
}

// Читает hex-строку из файла обратно в байты.
// Используется при дешифровании, читает файл, записанный через write_file_hex.
// std::hex заставляет (с помощью >>) парсить числа в шестнадцатеричной системе.
static std::vector<uint8_t> read_file_hex(const std::string& path) {
    std::ifstream file(path);
    std::vector<uint8_t> result;
    int val;
    while (file >> std::hex >> val) {
        result.push_back(static_cast<uint8_t>(val));
    }
    return result;
}

// Записывает байты в файл в бинарном режиме и сохраняет тсходный файл без изменений
static void write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Выводит результат операции в консоль двумя способами:
// 1) В hex - всегда, независимо от содержимого
// 2) Как текст - только если все байты являются печатаемыми символами (>= 0x20)
static void print_result(const std::vector<uint8_t>& data, const std::string& label) {
    std::cout << "\n--- " << label << " (HEX) ---\n";
    for (size_t i = 0; i < data.size(); ++i) {
        std::cout << std::hex << std::uppercase
                  << std::setw(2) << std::setfill('0')
                  << static_cast<int>(data[i]) << " ";
        if ((i + 1) % 16 == 0) std::cout << "\n";
    }
    // Возвращаем std::dec, иначе последующий вывод чисел будет в hex
    std::cout << std::dec << "\n";

    std::cout << "--- " << label << " (TEXT) ---\n";

    // Проверка: все ли байты являются печатаемыми символами
    bool printable = true;
    for (uint8_t b : data) {
        if (b < 0x20 && b != '\n' && b != '\r' && b != '\t') {
            printable = false;
            break;
        }
    }

    if (printable) {
        // reinterpret_cast нужен: std::cout.write ожидает char*, а у нас uint8_t*
        std::cout.write(reinterpret_cast<const char*>(data.data()), data.size());
        std::cout << "\n";
    } else {
        std::cout << "[Бинарные данные - текстовый вывод невозможен]\n";
    }
}

// Запрашивает у пользователя режим ввода: файл или консоль.
// Повторяет запрос до получения корректного значения
static int ask_mode() {
    int mode = 0;
    while (mode != 1 && mode != 2) {
        std::cout << "Режим ввода:\n";
        std::cout << "1. Файл\n";
        std::cout << "2. Консоль\n>> ";
        std::cin >> mode;
    }
    return mode;
}

void menuXOR() {
    int input = -1;
    std::string inFile, outFile, keyFile;

    // Ключ консольного режима хранится в памяти на всё время работы
    std::vector<uint8_t> console_key;
    bool console_key_ready = false; //ключ сгенерирован и готов к использованию

    while (input != MenuXOR::XOR_EXIT) {
        std::cout << "\nШифр XOR\n";
        std::cout << "1. Генерация ключа\n";
        std::cout << "2. Шифрование\n";
        std::cout << "3. Расшифрование\n";
        std::cout << "0. Выйти в меню\n>> ";
        std::cin >> input;

        switch (input) {

            case MenuXOR::KeyGen: {
                int mode = ask_mode();

                // generate_key(32) - генерирует 32 случайных байта.
                // 32 байта для корректной и стабильной работы программы
                auto key = generate_key(32);

                if (mode == 1) {
                    // Файловый режим: сохраняем ключ в бинарный файл
                    std::cout << "Введите путь для сохранения ключа: ";
                    std::cin >> keyFile;
                    save_key(key, keyFile);
                    std::cout << "Ключ сохранён: " << keyFile << "\n";
                } else {
                    // Консольный режим: ключ сохраняется только в памяти
                    console_key = key;
                    console_key_ready = true;
                    std::cout << "Ключ сгенерирован и готов к использованию\n";
                }

                print_result(key, "Сгенерированный ключ");
                break;
            }

            case MenuXOR::Encryption: {
                int mode = ask_mode();

                if (mode == 1) {
                    // Файловый режим для чтения данных и ключа из файлов
                    std::cout << "Введите путь входного файла: ";
                    std::cin >> inFile;
                    std::cout << "Введите путь выходного файла: ";
                    std::cin >> outFile;
                    std::cout << "Введите путь к ключу: ";
                    std::cin >> keyFile;

                    auto data      = read_file(inFile);
                    auto key       = read_file(keyFile);
                    auto encrypted = encrypt(data, key);

                    // Зашифрованный файл пишем в hex-формате для читаемости
                    write_file_hex(outFile, encrypted);
                    print_result(encrypted, "Зашифрованные данные");
                    std::cout << "Зашифровано -> " << outFile << "\n";

                } else {
                    // Консольный режим: проверяем наличие ключа
                    if (!console_key_ready) {
                        std::cout << "Сначала сгенерируйте ключ (пункт 1, режим консоль)\n";
                        break;
                    }

                    std::cout << "Используется сгенерированный ключ\n";
                    print_result(console_key, "Текущий ключ");

                    // std::cin.ignore() очищает буфер ввода от символа '\n',
                    // Без этого std::getline сразу вернёт пустую строку.
                    std::cin.ignore();
                    std::cout << "Введите текст для шифрования: ";
                    std::string text;
                    std::getline(std::cin, text);

                    // Преобразуем строку в вектор байт для передачи в encrypt()
                    std::vector<uint8_t> data(text.begin(), text.end());
                    auto encrypted = encrypt(data, console_key);

                    print_result(encrypted, "Зашифрованные данные");
                }
                break;
            }

            case MenuXOR::Decipher: {
                int mode = ask_mode();

                if (mode == 1) {
                    // Файловый режим: читаем зашифрованный файл в hex-формате
                    std::cout << "Введите путь зашифрованного файла: ";
                    std::cin >> inFile;
                    std::cout << "Введите путь выходного файла: ";
                    std::cin >> outFile;
                    std::cout << "Введите путь к ключу: ";
                    std::cin >> keyFile;

                    // read_file_hex - hex обратно в байты
                    auto data      = read_file_hex(inFile);
                    auto key       = read_file(keyFile);
                    auto decrypted = decrypt(data, key);

                    // Расшифрованный файл пишем в бинарном режиме, восстанавливаем данные
                    write_file(outFile, decrypted);
                    print_result(decrypted, "Расшифрованные данные");
                    std::cout << "Расшифровано -> " << outFile << "\n";

                } else {
                    // Консольный режим: проверяем наличие сгенерированного ключа
                    if (!console_key_ready) {
                        std::cout << "Сначала сгенерируйте ключ (пункт 1, режим консоль)\n";
                        break;
                    }

                    std::cout << "Используется сгенерированный ключ\n";

                    // Пользователь вводит hex-строку, полученную при шифровании
                    std::cout << "Введите зашифрованные данные в HEX: ";
                    std::cin.ignore();
                    std::string hexline;
                    std::getline(std::cin, hexline);

                    // std::istringstream позволяет парсить строку как поток -
                    // читаем hex-числа через >> с флагом std::hex
                    std::vector<uint8_t> data;
                    std::istringstream iss(hexline);
                    int val;
                    while (iss >> std::hex >> val) {
                        data.push_back(static_cast<uint8_t>(val));
                    }

                    auto decrypted = decrypt(data, console_key);
                    print_result(decrypted, "Расшифрованные данные");
                }
                break;
            }

            case MenuXOR::XOR_EXIT:
                break;

            default:
                std::cout << "Ошибка! Такого действия нет\n";
        }
    }
}