#include "../menu/menu.h"
#include "../menu/menuRailFence.h"
#include "../menu/menuSpiral.h"
#include <iostream>
#include <limits>

using namespace std;

int main() {
    int input = -1;

    while (input != Menu::EXIT) {
        cout << "\n\t Меню\n";
        cout << "Выберите шифр:\n";
        cout << "1. Double Rail Fence\n";
        cout << "2. Маршрутная перестановка (Спираль)\n";
        cout << "0. Выход\n>> ";

        if (!(cin >> input)) {
            cout << "Ошибка! Введите числовое значение\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            input = -1;
            continue;
        }

        switch (input) {
            case Menu::RAILFENCE:
                menuRailFence();
                break;
            case Menu::SPIRAL:
                menuSpiral();
                break;
            case Menu::EXIT:
                break;
            default:
                cout << "Ошибка! Такого действия нет\n";
        }
    }

    return 0;
}
