#include <iostream>
#include <stdint.h>
#include "menuRSA.h"

using namespace std;

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

typedef void (*RSA_Func)(const char*, const char*, uint64_t, uint64_t);

void menuRSA(){

    int input = -1;
    string inFile, outFile;
    uint64_t e, d, n;

    while (input != MenuRSA::RSA_EXIT){

    cout << endl <<  "Шифр RSA " << endl;
    cout << "1. Генерация ключей "<< endl;
    cout << "2. Шифрование "<< endl;
    cout << "3. Расшифрование "<< endl;
    cout << "0. Выйти в меню" << endl;
    cout << ">> ";
   

    cin >> input;

    switch (input){
        case MenuRSA::KeyGen:
            
            break;

        case MenuRSA::Encryption:

            cout << "Введите имя исходного файла: ";
            cin >> inFile;
            cout << "Введите имя для зашифрованного файла: ";
            cin >> outFile;
            cout << "Введите открытую экспоненту e: ";
            cin >> e;
            cout << "Введите n: ";
            cin >> n;

            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RSA.dll"); 
            #else
                void* hLib = dlopen("./libRSA.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                cout << "Ошибка! Не удалось найти или загрузить библиотеку RSA." << endl;
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
                cout << "Ошибка! Функция encryptFile не найдена в библиотеке." << endl;
            }

            //выгружаем из памяти
            #ifdef _WIN32
                FreeLibrary(hLib);
            #else
                dlclose(hLib);
            #endif
            break;
        

        case MenuRSA::Decipher:

            cout << "Введите имя зашифрованного файла: ";
            cin >> inFile;
            cout << "Введите имя для расшифрованного файла: ";
            cin >> outFile;
            cout << "Введите закрытую экспоненту d: ";
            cin >> d;
            cout << "Введите n: ";
            cin >> n;
            
            #ifdef _WIN32
                HINSTANCE hLib = LoadLibraryA("RSA.dll"); 
            #else
                void* hLib = dlopen("./libRSA.so", RTLD_LAZY);
            #endif

            if (!hLib) {
                cout << "Ошибка! Не удалось найти или загрузить библиотеку RSA." << endl;
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
                cout << "Ошибка! Функция decryptFile не найдена в библиотеке." << endl;
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
            cout << "Ошибка! Такого действия нет" << endl;
        
    }
}
}