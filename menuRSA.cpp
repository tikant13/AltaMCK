#include <iostream>
#include <stdint.h>
#include "menuRSA.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

typedef void (*RSA_Func)(const char*, const char*, uint64_t, uint64_t);

void menuRSA(){

    int input = -1;
    std::string inFile, outFile;
    uint64_t e, d, n;

    while (input != MenuRSA::RSA_EXIT){

    std::cout << std::endl <<  "Шифр RSA " << std::endl;
    std::cout << "1. Генерация ключей "<< std::endl;
    std::cout << "2. Шифрование "<< std::endl;
    std::cout << "3. Расшифрование "<< std::endl;
    std::cout << "0. Выйти в меню" << std::endl;
    std::cout << ">> ";
   

    std::cin >> input;

    switch (input){
        case MenuRSA::KeyGen:
            
            break;

        case MenuRSA::Encryption:

            std::cout << "Введите имя исходного файла: ";
            std::cin >> inFile;
            std::cout << "Введите имя для зашифрованного файла: ";
            std::cin >> outFile;
            std::cout << "Введите открытую экспоненту e: ";
            std::cin >> e;
            std::cout << "Введите n: ";
            std::cin >> n;

            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RSA.dll"); 
            #else
                void* hLib = dlopen("./libRSA.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось найти или загрузить библиотеку RSA." << std::endl;
                break;
            }

            // Ищем encryptFile внутри библиотеки
            #ifdef _WIN32
                RSA_Func encrypt = (RSA_Func)GetProcAddress(hLib, "encryptFile");
            #else
                RSA_Func encrypt = (RSA_Func)dlsym(hLib, "encryptFile");
            #endif

            if (encrypt) {
                encrypt(inFile.c_str(), outFile.c_str(), e, n); 
            } else {
                std::cout << "Ошибка! Функция encryptFile не найдена в библиотеке." << std::endl;
            }

            //выгружаем из памяти
            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif
            break;
        

        case MenuRSA::Decipher:

            std::cout << "Введите имя зашифрованного файла: ";
            std::cin >> inFile;
            std::cout << "Введите имя для расшифрованного файла: ";
            std::cin >> outFile;
            std::cout << "Введите закрытую экспоненту d: ";
            std::cin >> d;
            std::cout << "Введите n: ";
            std::cin >> n;
            
            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RSA.dll"); 
            #else
                void* hLib = dlopen("./libRSA.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось найти или загрузить библиотеку RSA." << std::endl;
                break;
            }

            #ifdef _WIN32
                RSA_Func decrypt = (RSA_Func)GetProcAddress(hLib, "decryptFile");
            #else
                RSA_Func decrypt = (RSA_Func)dlsym(hLib, "decryptFile");
            #endif

            if (decrypt) {
                decrypt(inFile.c_str(), outFile.c_str(), d, n);
            } else {
                std::cout << "Ошибка! Функция decryptFile не найдена в библиотеке." << std::endl;
            }

               
            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif
            
            break;
        
        case MenuRSA::RSA_EXIT:
            break;
            
        default:
            std::cout << "Ошибка! Такого действия нет" << std::endl;
        
    }
}
}