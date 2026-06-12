#include "menuSpiral.h"
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

static void checkMatrix(int& val, const string& name) {
    if (val < 2) {
        cout << name << " не может быть меньше 2. Установлено значение 2\n";
        val = 2;
    }
}

void menuSpiral() {
    void* handle = dlopen("./libspiral.so", RTLD_LAZY);
    if (!handle) {
        cerr << "Ошибка загрузки libspiral.so: " << dlerror() << "\n";
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

    while (input != MenuSpiral::SP_EXIT) {
        cout << "\nМаршрутная перестановка (Спираль)\n";
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
            case MenuSpiral::Encrypt: {
                int row, col;
                cout << "Введите имя входного файла: ";
                cin >> inFile;
                cout << "Введите имя выходного файла: ";
                cin >> outFile;
                cout << "Строки матрицы (>= 2): ";
                cin >> row;
                cout << "Столбцы матрицы (>= 2): ";
                cin >> col;
                checkMatrix(row, "Строки");
                checkMatrix(col, "Столбцы");
                auto data = readFile(inFile);
                uint8_t key[2] = { (uint8_t)row, (uint8_t)col };
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
            case MenuSpiral::Decrypt: {
                int row, col;
                cout << "Введите имя зашифрованного файла: ";
                cin >> inFile;
                cout << "Введите имя выходного файла: ";
                cin >> outFile;
                cout << "Строки матрицы (>= 2): ";
                cin >> row;
                cout << "Столбцы матрицы (>= 2): ";
                cin >> col;
                checkMatrix(row, "Строки");
                checkMatrix(col, "Столбцы");
                auto data = readFile(inFile);
                uint8_t key[2] = { (uint8_t)row, (uint8_t)col };
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
            case MenuSpiral::SP_EXIT:
                break;
            default:
                cout << "Ошибка! Такого действия нет\n";
        }
    }

    dlclose(handle);
}
