#include "XOR_generation_key.h"
#include "XOR_encryption.h"
#include "XOR_decryption.h"
#include <iostream>
#include <fstream>
#include <string>

std::vector<uint8_t> read_file(const std::string& path) {
    std::ifstream file(path, std::ios::binary);
    return std::vector<uint8_t>(
        std::istreambuf_iterator<char>(file),
        std::istreambuf_iterator<char>()
    );
}

void write_file(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

void write_file_decimal(const std::string& path, const std::vector<uint8_t>& data) {
    std::ofstream file(path);
    for(size_t i = 0; i < data.size(); ++i) {
        file << static_cast<int>(data[i]);
        if(i < data.size() - 1) file << " ";
    }
}

int main() {
    // 1. Генерируем ключ
    auto key = generate_key(32);
    save_key(key, "key.bin");

    std::string filename;
    std::cout << "Введите имя файла: ";
    std::cin >> filename;

    // 2. Читаем любой файл (положи рядом любой файл)
    auto data = read_file(filename);//txt

    // 3. Шифруем
    auto encrypted = encrypt(data, key);
    write_file("encrypted.bin", encrypted);

    // 4. Дешифруем
    auto decrypted = decrypt(encrypted, key);
    write_file("decrypted_" + filename, decrypted);//txt

    // 5. Проверяем что исходный и расшифрованный совпадают
    if(data == decrypted) {
        std::cout << "Успех! Файл расшифрован верно." << std::endl;
    } else {
        std::cout << "Ошибка!" << std::endl;
    }

    write_file_decimal("encrypted_decimal.txt", encrypted);

    return 0;
}