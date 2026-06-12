#include "menuSpiral.h"
#include "../spiral/encrypt.h"
#include "../spiral/decrypt.h"
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

static void checkMatrix(int& val, const string& name) {
    if (val < 2) {
        cout << name << " не может быть меньше 2. Установлено значение 2\n";
        val = 2;
    }
}

void menuSpiral() {
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
                auto encrypted = spEncrypt(data, row, col);
                writeFile(outFile, encrypted);
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
                auto decrypted = spDecrypt(data, row, col);
                writeFile(outFile, decrypted);
                cout << "Расшифровано: " << outFile << "\n";
                break;
            }
            case MenuSpiral::SP_EXIT:
                break;
            default:
                cout << "Ошибка! Такого действия нет\n";
        }
    }
}
