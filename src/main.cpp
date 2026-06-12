#include "menuXOR.h"
#include "menuLCG.h"
#include <iostream>
#include <string>
#include <limits>
#include "menu.h"
#include "menuRSA.h"
#include "menuRC4.h"

#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif



bool login(std::string& password){ // желательно бы куда-нибудь переместить, а то смешно выглядит 

    if ( password == "admin"){
        std::cout << "Авторизация успешна!" << std::endl;
        return true;
    } else {
        std::cout << "Неверный пароль!" << std::endl;
        return false;
    }
}

int main(){
    #ifdef _WIN32
        SetConsoleCP(65001);
        SetConsoleOutputCP(65001);
    #endif

    std::cout << "Введите пароль: ";
    std::string password;
    std::cin >> password;

    if (!login(password)) {
        return 0;
    }

    int input = -1;
    std::string inFile, outFile;

    while (input != Menu::EXIT){

    std::cout << std::endl <<  "\t Меню " << std::endl;
    std::cout << "Выберите какой шифр хотите использовать: " << std::endl;
    std::cout << "1. RSA "<< std::endl;
    std::cout << "2. RC4 "<< std::endl;
    std::cout << "3. XOR "<< std::endl;
    std::cout << "4. LCG "<< std::endl;
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
            menuRSA();
            break;

        case Menu::RC4:
            menuRC4();
            break;

        case Menu::XOR:
            menuXOR();
            break;

        case Menu::LCG:
            menuLCG();
            break;


        case Menu::EXIT:

            break;
            
        default:
            std::cout << "Ошибка! Такого действия нет" << std::endl;
        
    }
}

    return 0;
}