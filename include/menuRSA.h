#pragma once 

void menuRSA();

namespace MenuRSA{

    enum Type {
        KeyGen = 1,
        EncryptionFile = 2,
        DecryptionFile = 3,
        EncryptionText = 4,
        DecryptionText = 5,
        RSA_EXIT = 0,
    };
}