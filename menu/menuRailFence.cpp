#include "menuRailFence.h"
#include "../railfence/encrypt.h"
#include "../railfence/decrypt.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <limits>

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
                auto encrypted = rfDoubleEncrypt(data, rails1, rails2);
                writeFile(outFile, encrypted);
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
                auto decrypted = rfDoubleDecrypt(data, rails1, rails2);
                writeFile(outFile, decrypted);
                cout << "Расшифровано: " << outFile << "\n";
                break;
            }
            case MenuRailFence::RF_EXIT:
                break;
            default:
                cout << "Ошибка! Такого действия нет\n";
        }
    }
}
