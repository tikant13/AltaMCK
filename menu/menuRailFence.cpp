#include "menuRailFence.h"
#include "../cipher_api.h"
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

static void checkRails(int& rails) {
    if (rails < 2) {
        cout << "Количество рельсов не может быть меньше 2. Установлено значение 2\n";
        rails = 2;
    }
}

void menuRailFence() {
    void* handle = dlopen("./librailfence.so", RTLD_LAZY);
    if (!handle) {
        cerr << "Ошибка загрузки librailfence.so: " << dlerror() << "\n";
        return;
    }

    auto fnEncrypt = (decltype(&cipherEncrypt)) dlsym(handle, "cipherEncrypt");
    auto fnDecrypt = (decltype(&cipherDecrypt)) dlsym(handle, "cipherDecrypt");
    auto fnSize = (decltype(&getOutputSize)) dlsym(handle, "getOutputSize");

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
                int rails1, rails2;
                cout << "Введите имя входного файла: ";
                cin >> inFile;
                cout << "Введите имя выходного файла: ";
                cin >> outFile;
                cout << "Рельсы первого прохода (>= 2): ";
                cin >> rails1;
                cout << "Рельсы второго прохода (>= 2): ";
                cin >> rails2;
                checkRails(rails1);
                checkRails(rails2);
                auto data = readFile(inFile);
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
                writeFile(outFile, outBuf);
                cout << "Зашифровано: " << outFile << "\n";
                break;
            }
            case MenuRailFence::Decrypt: {
                int rails1, rails2;
                cout << "Введите имя зашифрованного файла: ";
                cin >> inFile;
                cout << "Введите имя выходного файла: ";
                cin >> outFile;
                cout << "Рельсы первого прохода (>= 2): ";
                cin >> rails1;
                cout << "Рельсы второго прохода (>= 2): ";
                cin >> rails2;
                checkRails(rails1);
                checkRails(rails2);
                auto data = readFile(inFile);
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
                writeFile(outFile, outBuf);
                cout << "Расшифровано: " << outFile << "\n";
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
