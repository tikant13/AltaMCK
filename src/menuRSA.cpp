#include <iostream>
#include <stdint.h>
#include "menuRSA.h"

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

typedef void (*RSA_Func)(const char*, const char*, uint64_t, uint64_t);
typedef bool (*KeyGenFunc)(uint64_t, uint64_t, uint64_t, uint64_t&, uint64_t&, uint64_t&);

void menuRSA(){

    int input = -1;
    std::string inFile, outFile;
    uint64_t e, d, n;

    while (input != MenuRSA::RSA_EXIT){

    std::cout << std::endl << "Шифр RSA " << std::endl;
    std::cout << "1. Генерация ключей "<< std::endl;
    std::cout << "2. Шифрование файла "<< std::endl;
    std::cout << "3. Расшифрование файла "<< std::endl;
    std::cout << "4. Шифрование текста "<< std::endl;
    std::cout << "5. Расшифрование текста"<< std::endl;
    std::cout << "0. Выйти в меню" << std::endl;
    std::cout << ">> ";
   

    std::cin >> input;

    switch (input){
        case MenuRSA::KeyGen:{
            uint64_t p, q, e;
            std::cout << "Введите простые p и q: ";
            std::cin >> p >> q;
            std::cout << "Выберите открытую экспоненту e: ";
            std::cin >> e;
            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RSA.dll"); 
            #else
                void* hLib = dlopen("./libRSA.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                std::cout << "Ошибка! Не удалось загрузить библиотеку RSA." << std::endl;
                break;
            }

            
            #ifdef _WIN32
                KeyGenFunc generateKeys = (KeyGenFunc)GetProcAddress(hLib, "generateKeys");
            #else
                KeyGenFunc generateKeys = (KeyGenFunc)dlsym(hLib, "generateKeys");
            #endif

            if (generateKeys) {
                uint64_t n = 0, d = 0, phi = 0;
                if (generateKeys(p, q, e, n, d, phi)) {
                    std::cout << std::endl << "Ключи: " << std::endl;
                    std::cout << "Открытый ключ: e = " << e << "  n = " << n << std::endl;
                    std::cout << "Закрытый ключ: d = " << d << "  n = " << n << std::endl;
                } else {
                    std::cout << "Ошибка! Неверные параметры (числа не простые или неверная экспонента e)." << std::endl;
                }

            } else {
                std::cout << "Ошибка! Функция generateKeys не найдена в библиотеке." << std::endl;
            }

            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif

            break;
        }

        case MenuRSA::EncryptionFile: {

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

            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif
            break;
        }

        case MenuRSA::DecryptionFile: {

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
        }

        case MenuRSA::EncryptionText: {
            std::cout << "Введите текст для шифрования: ";
            std::string inputText;
            std::cin.ignore(); // Очистка буфера перед getline
            std::getline(std::cin, inputText);
            std::cout << "Введите открытую экспоненту e: ";
            std::cin >> e;
            std::cout << "Введите n: ";
            std::cin >> n;

            char outputText[1024]; // Буфер для зашифрованного текста

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
                RSA_Func encryptText = (RSA_Func)GetProcAddress(hLib, "encryptText");
            #else
                RSA_Func encryptText = (RSA_Func)dlsym(hLib, "encryptText");
            #endif

            if (encryptText) {
                encryptText(inputText.c_str(), outputText, e, n);
                std::cout << "Зашифрованный текст: " << outputText << std::endl;
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
        
        case MenuRSA::DecryptionText: {
            std::cout << "Введите зашифрованный текст: ";
            std::string inputText;
            std::cin.ignore(); // Очистка буфера перед getline
            std::getline(std::cin, inputText);
            std::cout << "Введите закрытую экспоненту d: ";
            std::cin >> d;
            std::cout << "Введите n: ";
            std::cin >> n;

            char outputText[1024]; // Буфер для расшифрованного текста

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
                RSA_Func decryptText = (RSA_Func)GetProcAddress(hLib, "decryptText");
            #else
                RSA_Func decryptText = (RSA_Func)dlsym(hLib, "decryptText");
            #endif

            if (decryptText) {
                decryptText(inputText.c_str(), outputText, d, n);
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

        case MenuRSA::RSA_EXIT:
            break;
            
        default:
            std::cout << "Ошибка! Такого действия нет" << std::endl;
        
    }
}
}