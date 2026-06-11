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

// Записывает байты в файл в бинарном режиме.
// Используется для сохранения расшифрованных данных - восстанавливает исходный файл в начальном виде, без каких-либо измененийб
static void write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

// Записывает байты в файл в виде hex-строки
static void write_file_hex(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path);
    for (size_t i = 0; i < data.size(); ++i) {
        // std::setw(2) + std::setfill('0') гарантируют двузначный формат чтобы было 0F, а не F, например
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
// Используется при дешифровании - читает файл, записанный через write_file_hex.
// std::hex заставляет >> парсить числа в шестнадцатеричной системе.
static std::vector<uint8_t> read_file_hex(const std::string& path) {
    std::ifstream file(path);
    std::vector<uint8_t> result;
    int val;
    while (file >> std::hex >> val) {
        result.push_back(static_cast<uint8_t>(val));
    }
    return result;
}

// Сохраняет три параметра LCG (m, c, Xn) в бинарный файл.
// reinterpret_cast преобразует указатель на число в указатель на байты
// для побайтовой записи через file.write.
static bool save_key(const std::string& path, uint64_t m, uint64_t c, uint64_t Xn) {
    std::ofstream file(path, std::ios::binary);
    if (!file) return false;
    file.write(reinterpret_cast<const char*>(&m),  sizeof(m));
    file.write(reinterpret_cast<const char*>(&c),  sizeof(c));
    file.write(reinterpret_cast<const char*>(&Xn), sizeof(Xn));
    return file.good();
}

// Загружает параметры LCG из бинарного файла, созданного через save_key.
// Параметры передаются по ссылке - функция заполняет их напрямую.
// Возвращает false если файл не открылся или данные прочитаны неполностью.
static bool load_key(const std::string& path, uint64_t& m, uint64_t& c, uint64_t& Xn) {
    std::ifstream file(path, std::ios::binary);
    if (!file) return false;
    file.read(reinterpret_cast<char*>(&m),  sizeof(m));
    file.read(reinterpret_cast<char*>(&c),  sizeof(c));
    file.read(reinterpret_cast<char*>(&Xn), sizeof(Xn));
    return file.good();
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
        // reinterpret_cast нужен: std::cout.write ожидает char*, а у меня uint8_t*
        std::cout.write(reinterpret_cast<const char*>(data.data()), data.size());
        std::cout << "\n";
    } else {
        std::cout << "[Бинарные данные - текстовый вывод невозможен]\n";
    }
}

// Запрашивает у пользователя режим ввода: файл или консоль.
// Повторяет запрос до получения корректного значения (1 или 2).
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

void menuLCG() {
    // srand инициализируется один раз за время работы программы.
    // static bool гарантирует это даже при повторных вызовах menuLCG().
    // time(nullptr) даёт текущее время как seed - разный при каждом запуске.
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }

    int input = -1;
    std::string inFile, outFile, keyFile;

    // Параметры LCG консольного режима хранятся в памяти на всё время работы меню.
    // Это позволяет сгенерировать ключ один раз и использовать его
    // при последующих операциях шифрования и дешифрования без повторного ввода.
    uint64_t console_m  = 0;
    uint64_t console_c  = 0;
    uint64_t console_Xn = 0;
    bool console_key_ready = false; //ключ сгенерирован и готов к использованию

    while (input != MenuLCG::LCG_EXIT) {
        std::cout << "\nШифр LCG\n";
        std::cout << "1. Генерация ключа\n";
        std::cout << "2. Шифрование\n";
        std::cout << "3. Расшифрование\n";
        std::cout << "0. Выйти в меню\n>> ";
        std::cin >> input;

        switch (input) {

            case MenuLCG::KeyGen: {
                int mode = ask_mode();

                // m - простое число Мерсенна (2^31 - 1).
                // Простые числа Мерсенна обеспечивают максимальный период LCG, что критично для генерируемой случайной последовательности.
                uint64_t m  = 2147483647ULL;

                // Генерируем случайные c и Xn в диапазоне [1, m).
                // rand() возвращает 15-битное число, поэтому комбинируем два вызова
                // через сдвиг и OR чтобы получить достаточно случайное 64-битное значение.
                uint64_t c  = (static_cast<uint64_t>(rand()) << 32 | rand()) % m;
                uint64_t Xn = (static_cast<uint64_t>(rand()) << 32 | rand()) % m;

                // Защита от нулевых значений: нулевой c даёт константный поток,
                // нулевой Xn - предсказуемую последовательность с первого шага.
                if (c  == 0) c  = 1013904223ULL;
                if (Xn == 0) Xn = 12345ULL;

                if (mode == 1) {
                    // Файловый режим: сохраняем все три параметра в бинарный файл
                    std::cout << "Введите путь для сохранения ключа: ";
                    std::cin >> keyFile;
                    if (save_key(keyFile, m, c, Xn)) {
                        std::cout << "Ключ сохранён: " << keyFile << "\n";
                    } else {
                        std::cout << "Ошибка: не удалось сохранить ключ\n";
                        break;
                    }
                } else {
                    // Консольный режим: параметры сохраняются только в памяти
                    console_m  = m;
                    console_c  = c;
                    console_Xn = Xn;
                    console_key_ready = true;
                    std::cout << "Ключ сгенерирован и готов к использованию\n";
                }

                std::cout << "  m  = " << m  << "\n";
                std::cout << "  c  = " << c  << "\n";
                std::cout << "  Xn = " << Xn << "\n";
                break;
            }

            case MenuLCG::Encryption: {
                int mode = ask_mode();

                if (mode == 1) {
                    uint64_t m, c, Xn;
                    std::cout << "Введите путь к файлу ключа: ";
                    std::cin >> keyFile;
                    if (!load_key(keyFile, m, c, Xn)) {
                        std::cout << "Ошибка: не удалось загрузить ключ из " << keyFile << "\n";
                        break;
                    }

                    std::cout << "Введите путь входного файла: ";
                    std::cin >> inFile;
                    std::cout << "Введите путь выходного файла: ";
                    std::cin >> outFile;

                    auto data   = read_file(inFile);
                    // generation_key генерирует поток псевдослучайных байт длиной data.size()
                    // на основе параметров LCG - этот поток используется как ключевой материал
                    auto stream    = generation_key(m, Xn, c, data.size());
                    auto encrypted = encrypt(data, stream);

                    // Зашифрованный файл пишем в hex-формате для читаемости
                    write_file_hex(outFile, encrypted);
                    print_result(encrypted, "Зашифрованные данные");
                    std::cout << "Зашифровано -> " << outFile << "\n";

                } else {
                    // Консольный режим: проверяем наличие сгенерированного ключа
                    if (!console_key_ready) {
                        std::cout << "Сначала сгенерируйте ключ (пункт 1, режим консоль)\n";
                        break;
                    }

                    std::cout << "Используется ключ: m=" << console_m
                              << " c=" << console_c
                              << " Xn=" << console_Xn << "\n";

                    // std::cin.ignore() очищает буфер ввода от символа '\n',
                    // оставшегося после предыдущего std::cin >> input.
                    // Без этого std::getline сразу вернёт пустую строку.
                    std::cin.ignore();
                    std::cout << "Введите текст для шифрования: ";
                    std::string text;
                    std::getline(std::cin, text);

                    // Преобразуем строку в вектор байт для передачи в encrypt()
                    std::vector<uint8_t> data(text.begin(), text.end());
                    auto stream    = generation_key(console_m, console_Xn, console_c, data.size());
                    auto encrypted = encrypt(data, stream);

                    print_result(encrypted, "Зашифрованные данные");
                }
                break;
            }

            case MenuLCG::Decipher: {
                int mode = ask_mode();

                if (mode == 1) {
                    uint64_t m, c, Xn;
                    std::cout << "Введите путь к файлу ключа: ";
                    std::cin >> keyFile;
                    if (!load_key(keyFile, m, c, Xn)) {
                        std::cout << "Ошибка: не удалось загрузить ключ из " << keyFile << "\n";
                        break;
                    }

                    std::cout << "Введите путь зашифрованного файла: ";
                    std::cin >> inFile;
                    std::cout << "Введите путь выходного файла: ";
                    std::cin >> outFile;

                    // read_file_hex - hex обратно в байты
                    auto data      = read_file_hex(inFile);
                    auto stream    = generation_key(m, Xn, c, data.size());
                    auto decrypted = decrypt(data, stream);

                    // Расшифрованный файл пишем в бинарном режиме -
                    // восстанавливаем исходные данные без изменений
                    write_file(outFile, decrypted);
                    print_result(decrypted, "Расшифрованные данные");
                    std::cout << "Расшифровано -> " << outFile << "\n";

                } else {
                    // Консольный режим: проверяем наличие сгенерированного ключа
                    if (!console_key_ready) {
                        std::cout << "Сначала сгенерируйте ключ (пункт 1, режим консоль)\n";
                        break;
                    }

                    std::cout << "Используется ключ: m=" << console_m
                              << " c=" << console_c
                              << " Xn=" << console_Xn << "\n";

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

                    auto stream    = generation_key(console_m, console_Xn, console_c, data.size());
                    auto decrypted = decrypt(data, stream);

                    print_result(decrypted, "Расшифрованные данные");
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