#include <iostream>
#include <string>
#include <limits>
#include "menu.h"


bool login(){
    std::string password;

    std::cout << "Введите пароль: ";
    std::cin >> password;

    if ( password == "admin"){
        std::cout << "Авторизация успешна!" << std::endl;
        return true;
    } else {
        std::cout << "Неверный пароль!" << std::endl;
        return false;
    }
}

int main(){

    if (!login()) {
        return 0;
    }

    int input = -1;
    std::string inFile, outFile;

    while (input != Menu::EXIT){

    std::cout << std::endl <<  "\t Меню " << std::endl;
    std::cout << "Выберите какой шифр хотите использовать: " << std::endl;
    std::cout << "1. RSA "<< std::endl;
    std::cout << "2. RC4 "<< std::endl;
    std::cout << "0. Выход" << std::endl;
    std::cout << ">> ";
   

    if (!(std::cin >> input)) {
        std::cout << "Ошибка! Введите числовое значение" << std::endl;
        std::cin.clear(); 
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
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
            std::cout << "Ошибка! Такого действия нет" << std::endl;
        
    }
}

    return 0;
}