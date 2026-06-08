#include <iostream>
#include <fstream>
#include <sstream>
#include <stdint.h>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

uint64_t binPower(uint64_t base, uint64_t power, uint64_t modulo){
    uint64_t result = 1;
    base %= modulo;
    while (power > 0){
        if (power % 2 == 1){ // если текущий бит равен 1
            result = (static_cast<unsigned __int128>(result) * base) % modulo;
        }
        base = (base * base) % modulo;
        power /= 2; // переходим к следующему биту
    }
    return result;
}

bool isPrime(uint64_t n){ 
    if (n <= 1){
        return false; 
    }
    for (uint64_t i=2; i * i <= n; ++i){
        if (n % i == 0){
            return false;
        }
    }
    return true;
}

uint64_t extEuclid(uint64_t a, uint64_t b, uint64_t &u, uint64_t &v) {
    
    u = 1; 
    v = 0;
    uint64_t uPrev = 0; 
    uint64_t vPrev = 1;

    while (b != 0) {
       
        uint64_t wholePart = a / b;
        uint64_t remainder = a % b;

        uint64_t nextu = u - wholePart * uPrev; 
        uint64_t nextv = v - wholePart * vPrev;

        a = b;
        b = remainder;
        u = uPrev;
        uPrev = nextu;
        v = vPrev;
        vPrev = nextv;
    }
    return a;
}

extern "C" {
    EXPORT void encryptFile(const char* inputPath, const char* outputPath, uint64_t e, uint64_t n) {
    
        std::ifstream inFile(inputPath, std::ios::binary); // открываем файл как бинарный
        std::ofstream outFile(outputPath); // зашифрованные данные пишем как текст 

        if (!inFile.is_open() || !outFile.is_open()) {
            std::cout << "Ошибка! Не удалось открыть файлы для шифрования" << std::endl;
            return;
        }

        char ch;

        while (inFile.get(ch)) {
            unsigned char m = static_cast<unsigned char>(ch);
            uint64_t c = binPower(m, e, n); 
            outFile << c << " "; // записываем и разделяем пробелом 

        }

        std::cout << std::endl << "Шифрование завершено. Файл сохранен: " << outputPath << std::endl;
        inFile.close();
        outFile.close();
    }

    EXPORT void decryptFile(const char* inputPath, const char* outputPath, uint64_t d, uint64_t n) {
    
        std::ifstream inFile(inputPath);
        std::ofstream outFile(outputPath, std::ios::binary);

        if (!inFile.is_open() || !outFile.is_open()) {
            std::cout << "Ошибка! Не удалось открыть файлы для шифрования" << std::endl;
            return;
        }

        uint64_t c;
        
        while (inFile >> c) { // Считываем числа, разделенные пробелами
            uint64_t m = binPower(c, d, n); 
            char ch = static_cast<char>(m);
            outFile.put(ch); 

        }

        std::cout << "Расшифрование завершено. Файл сохранен: " << outputPath << std::endl;
        inFile.close();
        outFile.close();
    }

}