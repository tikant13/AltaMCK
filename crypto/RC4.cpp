#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cstring>
#include <algorithm>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif


extern "C" {

    // так как шифруется и расшифровывается одинаково, то используем тот же код
    EXPORT void decryptEncryptFile(const char* inputPath, const char* outputPath, const char* key) { 
        // открываем в бинарном режиме
        std::ifstream inFile(inputPath, std::ios::binary);
        std::ofstream outFile(outputPath, std::ios::binary);

        if (!inFile.is_open() || !outFile.is_open()) {
            std::cout << "Ошибка! Не удалось открыть файлы для обработки." << std::endl;
            return;
        }

        // Инициализация массива S
        unsigned char S[256];
        for (int i = 0; i < 256; i++) {
            S[i] = i;
        }

        int j = 0;
        int keyLen = strlen(key);
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + static_cast<unsigned char>(key[i % keyLen])) % 256;
            std::swap(S[i], S[j]);
        }

        // генерация гаммы и шифрование
        int i = 0;
        j = 0;
        char ch;
        
        while (inFile.get(ch)) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            std::swap(S[i], S[j]);
            unsigned char rnd = S[(S[i] + S[j]) % 256]; // Байт гаммы
            
            // Применяем xor для шифрования и расшифровки
            unsigned char out = static_cast<unsigned char>(ch) ^ rnd;
            outFile.put(static_cast<char>(out));
        }

        std::cout << "Успешно! Результат сохранен в файл: " << outputPath << std::endl;
        
        inFile.close();
        outFile.close();
    }

    // расшифровываем текст с консоли, результат переводим в 16 систему
    EXPORT void encryptText(const char* inputText, char* outputText, const char* key) {
        if (!inputText || !outputText) return;

        unsigned char S[256];
        for (int i = 0; i < 256; i++) S[i] = i;

        int j = 0;
        int keyLen = strlen(key);
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + static_cast<unsigned char>(key[i % keyLen])) % 256;
            std::swap(S[i], S[j]);
        }

        int i = 0;
        j = 0;
        std::stringstream hexStream;
        
        while (*inputText) {
            i = (i + 1) % 256;
            j = (j + S[i]) % 256;
            std::swap(S[i], S[j]);
            unsigned char rnd = S[(S[i] + S[j]) % 256];
            
            unsigned char out = static_cast<unsigned char>(*inputText) ^ rnd;
            
            // Форматируем каждый зашифрованный байт как двузначное 16-ричное число
            hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(out);
            inputText++;
        }

        std::string res = hexStream.str();
        for (size_t k = 0; k < res.length(); ++k) {
            outputText[k] = res[k];
        }
        outputText[res.length()] = '\0';
    }

    // расшифровываем текст из консоли 
    EXPORT void decryptText(const char* inputText, char* outputText, const char* key) {
        if (!inputText || !outputText) return;

        unsigned char S[256];
        for (int i = 0; i < 256; i++) S[i] = i;

        int j = 0;
        int keyLen = strlen(key);
        for (int i = 0; i < 256; i++) {
            j = (j + S[i] + static_cast<unsigned char>(key[i % keyLen])) % 256;
            std::swap(S[i], S[j]);
        }

        int i = 0;
        int j_idx = 0;
        std::string outStr;
        size_t len = strlen(inputText);
        
        // Читаем по два символа так как каждый байт записан двумя 16-ричными символами
        for (size_t k = 0; k < len; k += 2) {
            std::string byteString(inputText + k, 2);
            unsigned char ch = static_cast<unsigned char>(strtol(byteString.c_str(), NULL, 16));
            
            i = (i + 1) % 256;
            j_idx = (j_idx + S[i]) % 256;
            std::swap(S[i], S[j_idx]);
            unsigned char rnd = S[(S[i] + S[j_idx]) % 256];
            
            unsigned char m = ch ^ rnd;
            outStr += static_cast<char>(m);
        }

        for (size_t k = 0; k < outStr.length(); ++k) {
            outputText[k] = outStr[k];
        }
        outputText[outStr.length()] = '\0';
    }

}