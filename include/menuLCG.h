#pragma once 

void menuLCG();

namespace MenuLCG{

    enum Type {
        KeyGen = 1,
        Encryption = 2,
        Decipher = 3,
        LCG_EXIT = 0,
    };
}