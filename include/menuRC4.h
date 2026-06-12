#pragma once 

void menuRC4();

namespace MenuRC4{

    enum Type {
        EncryptionFile = 1,
        DecryptionFile = 2,
        EncryptionText = 3,
        DecryptionText = 4,
        RC4_EXIT = 0,
    };
}