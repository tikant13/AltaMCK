#include <iostream>
#include <string>
#include <limits>
#include "menu.h"

using namespace std;


bool login(){
    string password;

    cout << "Введите пароль: ";
    cin >> password;

    if ( password == "admin"){
        cout << "Авторизация успешна!" << endl;
        return true;
    } else {
        cout << "Неверный логин или пароль!" << endl;
        return false;
    }
}

int main(){

    if (!login()) {
        return 0;
    }

    int input = -1;
    string inFile, outFile;

    while (input != Menu::EXIT){

    cout << endl <<  "\t Меню " << endl;
    cout << "Выберите какой шифр хотите использовать: " << endl;
    cout << "1. RSA "<< endl;
    cout << "2. RC4 "<< endl;
    cout << "0. Выход" << endl;
    cout << ">> ";
   

    if (!(cin >> input)) {
        cout << "Ошибка! Введите числовое значение" << endl;
        cin.clear(); 
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
        input = -1;
        continue;
    }

    switch (input){
        case Menu::RSA:

            break;

        case Menu::RC4:

            break;

        case Menu::EXIT:
            break;
            
        default:
            cout << "Ошибка! Такого действия нет" << endl;
        
    }
}

    return 0;
}