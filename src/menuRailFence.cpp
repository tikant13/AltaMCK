#include "../include/menuRailFence.h"
#include "../include/cipher_api.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <dlfcn.h>

using namespace std;

static vector<uint8_t> readFile(const string& path) {
    ifstream file(path, ios::binary);
    return vector<uint8_t>(
        istreambuf_iterator<char>(file),
        istreambuf_iterator<char>()
    );
}

static void writeFile(const string& path, const vector<uint8_t>& data) {
    ofstream file(path, ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

static void printResult(const vector<uint8_t>& data, const string& label) {
    cout << "\n--- " << label << " (TEXT) ---\n";
    bool printable = true;
    for (uint8_t b : data) {
        if (b < 0x20 && b != '\n' && b != '\r' && b != '\t') {
            printable = false;
            break;
        }
    }
    if (printable) {
        cout.write(reinterpret_cast<const char*>(data.data()), data.size());
        cout << "\n";
    } else {
        cout << "[Бинарные данные - текстовый вывод невозможен]\n";
    }
}

static void checkRails(int& rails) {
    if (rails < 2) {
        cout << "Количество рельсов не может быть меньше 2. Установлено значение 2\n";
        rails = 2;
    }
}

static int askMode() {
    int mode = 0;
    while (mode != 1 && mode != 2) {
        cout << "Режим ввода:\n";
        cout << "1. Файл\n";
        cout << "2. Консоль\n>> ";
        cin >> mode;
    }
    return mode;
}

void menuRailFence() {
    void* handle = dlopen("./librailfence.so", RTLD_LAZY);
    if (!handle) {
        cerr << "Ошибка загрузки librailfence.so: " << dlerror() << "\n";
        return;
    }

    auto fnEncrypt = (decltype(&cipherEncrypt)) dlsym(handle, "cipherEncrypt");
    auto fnDecrypt = (decltype(&cipherDecrypt)) dlsym(handle, "cipherDecrypt");
    auto fnSize    = (decltype(&getOutputSize)) dlsym(handle, "getOutputSize");

    if (!fnEncrypt || !fnDecrypt || !fnSize) {
        cerr << "Ошибка: библиотека не экспортирует нужные функции\n";
        dlclose(handle);
        return;
    }

    int input = -1;
    string inFile, outFile;

    while (input != MenuRailFence::RF_EXIT) {
        cout << "\nDouble Rail Fence\n";
        cout << "1. Зашифровать\n";
        cout << "2. Расшифровать\n";
        cout << "0. Выйти в меню\n>> ";

        if (!(cin >> input)) {
            cout << "Ошибка! Введите числовое значение\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = -1;
            continue;
        }

        switch (input) {
            case MenuRailFence::Encrypt: {
                int mode = askMode();
                int rails1, rails2;
                cout << "Рельсы первого прохода (>= 2): ";
                cin >> rails1;
                cout << "Рельсы второго прохода (>= 2): ";
                cin >> rails2;
                checkRails(rails1);
                checkRails(rails2);

                vector<uint8_t> data;
                if (mode == 1) {
                    cout << "Введите имя входного файла: ";
                    cin >> inFile;
                    cout << "Введите имя выходного файла: ";
                    cin >> outFile;
                    data = readFile(inFile);
                } else {
                    cin.ignore();
                    cout << "Введите текст: ";
                    string text;
                    getline(cin, text);
                    data = vector<uint8_t>(text.begin(), text.end());
                }

                uint8_t key[2] = { (uint8_t)rails1, (uint8_t)rails2 };
                ConstBuf kb = { key, 2 };
                ConstBuf ib = { data.data(), data.size() };
                size_t outSize = fnSize(data.size());
                vector<uint8_t> outBuf(outSize);
                MutBuf mb = { outBuf.data(), outSize };
                int ret = fnEncrypt(kb, ib, &mb);
                if (ret != 0) {
                    cerr << "Ошибка шифрования: код " << ret << "\n";
                    break;
                }
                outBuf.resize(mb.size);

                if (mode == 1) {
                    writeFile(outFile, outBuf);
                    cout << "Зашифровано: " << outFile << "\n";
                } else {
                    printResult(outBuf, "Зашифрованные данные");
                }
                break;
            }
            case MenuRailFence::Decrypt: {
                int mode = askMode();
                int rails1, rails2;
                cout << "Рельсы первого прохода (>= 2): ";
                cin >> rails1;
                cout << "Рельсы второго прохода (>= 2): ";
                cin >> rails2;
                checkRails(rails1);
                checkRails(rails2);

                vector<uint8_t> data;
                if (mode == 1) {
                    cout << "Введите имя зашифрованного файла: ";
                    cin >> inFile;
                    cout << "Введите имя выходного файла: ";
                    cin >> outFile;
                    data = readFile(inFile);
                } else {
                    cin.ignore();
                    cout << "Введите зашифрованный текст: ";
                    string text;
                    getline(cin, text);
                    data = vector<uint8_t>(text.begin(), text.end());
                }

                uint8_t key[2] = { (uint8_t)rails1, (uint8_t)rails2 };
                ConstBuf kb = { key, 2 };
                ConstBuf ib = { data.data(), data.size() };
                size_t outSize = fnSize(data.size());
                vector<uint8_t> outBuf(outSize);
                MutBuf mb = { outBuf.data(), outSize };
                int ret = fnDecrypt(kb, ib, &mb);
                if (ret != 0) {
                    cerr << "Ошибка расшифрования: код " << ret << "\n";
                    break;
                }
                outBuf.resize(mb.size);

                if (mode == 1) {
                    writeFile(outFile, outBuf);
                    cout << "Расшифровано: " << outFile << "\n";
                } else {
                    printResult(outBuf, "Расшифрованные данные");
                }
                break;
            }
            case MenuRailFence::RF_EXIT:
                break;
            default:
                cout << "Ошибка! Такого действия нет\n";
        }
    }

    dlclose(handle);
}
