#include <iostream>
#include <stdint.h>
#include <limits>
#include "menuRC4.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

typedef void (*RC4_FileFunc)(const char*, const char*, const char*);
typedef void (*RC4_TextFunc)(const char*, char*, const char*);

void menuRC4(){

    int input = -1;
    std::string inFile, outFile;
    std::string key;

    while (input != MenuRC4::RC4_EXIT){

    std::cout << std::endl << "Шифр RC4 " << std::endl;
    std::cout << "1. Шифрование файла "<< std::endl;
    std::cout << "2. Расшифрование файла "<< std::endl;
    std::cout << "3. Шифрование текста "<< std::endl;
    std::cout << "4. Расшифрование текста"<< std::endl;
    std::cout << "0. Выйти в меню" << std::endl;
    std::cout << ">> ";
   

    std::cin >> input;

    switch (input){

        case MenuRC4::EncryptionFile: {
            std::cout << "Введите имя исходного файла: ";
            std::cin >> inFile;
            std::cout << "Введите имя для зашифрованного файла: ";
            std::cin >> outFile;
            std::cout << "Введите ключ для шифрования: ";
            std::cin >> key;
            

            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RC4.dll"); 
            #else
                void* hLib = dlopen("./libRC4.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось найти или загрузить библиотеку RC4." << std::endl;
                break;
            }

            #ifdef _WIN32
                RC4_FileFunc encrypt = (RC4_FileFunc)GetProcAddress(hLib, "decryptEncryptFile");
            #else
                RC4_FileFunc encrypt = (RC4_FileFunc)dlsym(hLib, "decryptEncryptFile");
            #endif

            if (encrypt) {
                encrypt(inFile.c_str(), outFile.c_str(), key.c_str()); 
            } else {
                std::cout << "Ошибка! Функция decryptEncryptFile не найдена в библиотеке." << std::endl;
            }

            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif

            break;
        }

        case MenuRC4::DecryptionFile: {
            std::cout << "Введите имя зашифрованного файла: ";
            std::cin >> inFile;
            std::cout << "Введите имя для расшифрованного файла: ";
            std::cin >> outFile;
            std::cout << "Введите ключ для расшифрования: ";
            std::cin >> key;

              if (!(std::cin >> key)) {
                std::cout << std::endl << "Ошибка! Введите числовое значение" << std::endl;
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                input = -1;
                continue;
            }
                
            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RC4.dll"); 
            #else
                void* hLib = dlopen("./libRC4.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось найти или загрузить библиотеку RC4." << std::endl;
                break;
            }

            #ifdef _WIN32
                RC4_FileFunc decrypt = (RC4_FileFunc)GetProcAddress(hLib, "decryptEncryptFile");
            #else
                RC4_FileFunc decrypt = (RC4_FileFunc)dlsym(hLib, "decryptEncryptFile");
            #endif

            if (decrypt) {
                decrypt(inFile.c_str(), outFile.c_str(), key.c_str());
            } else {
                std::cout << "Ошибка! Функция decryptEncryptFile не найдена в библиотеке." << std::endl;
            }
        
            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif
            
            break;
        }

        case MenuRC4::EncryptionText: {
            std::cout << "Введите текст для шифрования: ";
            std::string inputText;
            std::cin.ignore(); // Очистка буфера перед getline
            std::getline(std::cin, inputText);
            std::cout << "Введите ключ шифрования: ";
            std::cin >> key;

            if (!(std::cin >> key)) {
                std::cout << std::endl << "Ошибка! Введите числовое значение" << std::endl;
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
                input = -1;
                continue;
            }

            char outputText[1024] = {0}; // Буфер для зашифрованного текста

            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RC4.dll"); 
            #else
                void* hLib = dlopen("./libRC4.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось найти или загрузить библиотеку RC4." << std::endl;
                break;
            }

            #ifdef _WIN32
                RC4_TextFunc encryptText = (RC4_TextFunc)GetProcAddress(hLib, "encryptText");
            #else
                RC4_TextFunc encryptText = (RC4_TextFunc)dlsym(hLib, "encryptText");
            #endif

            if (encryptText) {
                encryptText(inputText.c_str(), outputText, key.c_str());
                std::cout << "Зашифрованный текст (в байтах/символах): " << outputText << std::endl;
            } else {
                std::cout << "Ошибка! Функция encryptText не найдена в библиотеке." << std::endl;
            }

            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif
                
            break;
        }
        
        case MenuRC4::DecryptionText: {
            std::cout << "Введите зашифрованный текст: ";
            std::string inputText;
            std::cin.ignore(); // Очистка буфера перед getline
            std::getline(std::cin, inputText);
            std::cout << "Введите ключ для расшифрования: ";
            std::cin >> key;

            if (!(std::cin >> key)) {
                std::cout << std::endl << "Ошибка! Введите числовое значение" << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                key = -1;
                continue;
            }

            char outputText[1024] = {0}; // Буфер для расшифрованного текста

            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RC4.dll"); 
            #else
                void* hLib = dlopen("./libRC4.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось найти или загрузить библиотеку RC4." << std::endl;
                break;
            }

            #ifdef _WIN32
                RC4_TextFunc decryptText = (RC4_TextFunc)GetProcAddress(hLib, "decryptText");
            #else
                RC4_TextFunc decryptText = (RC4_TextFunc)dlsym(hLib, "decryptText");
            #endif

            if (decryptText) {
                decryptText(inputText.c_str(), outputText, key.c_str());
                std::cout << "Расшифрованный текст: " << outputText << std::endl;
            } else {
                std::cout << "Ошибка! Функция decryptText не найдена в библиотеке." << std::endl;
            }

            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif

            break;
        }

        case MenuRC4::RC4_EXIT:
            break;
            
        default:
            std::cout << "Ошибка! Такого действия нет" << std::endl;
        
    }
}
}