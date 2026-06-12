#include "../include/menuSpiral.h"
#include "../include/cipher_api.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>
#include <iomanip>
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
    vector<uint8_t> trimmed = data;
    while (!trimmed.empty() && trimmed.back() == 0) {
        trimmed.pop_back();
    }
    cout << "\n--- " << label << " (TEXT) ---\n";
    bool printable = true;
    for (uint8_t b : trimmed) {
        if (b < 0x20 && b != '\n' && b != '\r' && b != '\t') {
            printable = false;
            break;
        }
    }
    if (printable) {
        cout.write(reinterpret_cast<const char*>(trimmed.data()), trimmed.size());
        cout << "\n";
    } else {
        cout << "\n--- " << label << " (HEX) ---\n";
        for (size_t i = 0; i < trimmed.size(); ++i) {
            cout << hex << uppercase
                 << setw(2) << setfill('0')
                 << static_cast<int>(trimmed[i]) << " ";
            if ((i + 1) % 16 == 0) cout << "\n";
        }
        cout << dec << "\n";
    }
}
static vector<uint8_t> hexToBytes(const string& hexStr) {
    vector<uint8_t> bytes;
    string cleanStr = "";
    for (char c : hexStr) {
        if (!isspace(c)) cleanStr += c;
    }
    for (size_t i = 0; i < cleanStr.length(); i += 2) {
        if (i + 1 < cleanStr.length()) {
            string byteString = cleanStr.substr(i, 2);
            uint8_t byte = (uint8_t) strtol(byteString.c_str(), nullptr, 16);
            bytes.push_back(byte);
        }
    }
    return bytes;
}

static void checkMatrix(int& val, const string& name) {
    if (val < 2) {
        cout << name << " не может быть меньше 2. Установлено значение 2\n";
        val = 2;
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

void menuSpiral() {
    void* handle = dlopen("./libspiral.so", RTLD_LAZY);
    if (!handle) {
        cerr << "Ошибка загрузки libspiral.so: " << dlerror() << "\n";
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
                int mode = askMode();
                int row, col;
                cout << "Строки матрицы (>= 2): ";
                cin >> row;
                cout << "Столбцы матрицы (>= 2): ";
                cin >> col;
                checkMatrix(row, "Строки");
                checkMatrix(col, "Столбцы");

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

                if (mode == 1) {
                    writeFile(outFile, outBuf);
                    cout << "Зашифровано: " << outFile << "\n";
                } else {
                    printResult(outBuf, "Зашифрованные данные");
                }
                break;
            }
            case MenuSpiral::Decrypt: {
                int mode = askMode();
                int row, col;
                cout << "Строки матрицы (>= 2): ";
                cin >> row;
                cout << "Столбцы матрицы (>= 2): ";
                cin >> col;
                checkMatrix(row, "Строки");
                checkMatrix(col, "Столбцы");

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
                    string hexText;
                    getline(cin, hexText);
                    data = hexToBytes(hexText);
                }

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

                if (mode == 1) {
                    writeFile(outFile, outBuf);
                    cout << "Расшифровано: " << outFile << "\n";
                } else {
                    printResult(outBuf, "Расшифрованные данные");
                }
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
