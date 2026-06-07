#include "LCG_generation_key.h"
#include <cstdint>
#include <vector>
#include <cstdlib>//
#include <ctime> //

std::vector <uint64_t>  generation_key(uint64_t m, uint64_t Xn, uint64_t a, uint64_t c, size_t size){
    std::vector <uint64_t> stream (size);
    static bool seeded = false;
    if(!seeded){
    std::srand(static_cast<uint32_t>(time(nullptr)) ^ static_cast<uint32_t>(clock())); //clock() - processor's work time
    seeded = true;
    }
    static std::vector<uint64_t> steps;
    if(steps.empty()) {
        size_t randnum1 = (rand() % 19) + 39;
        size_t randnum2 = (rand() % 10) + 18;
        size_t randnum3 = (rand() % 7) + 22;
        std::vector<uint64_t> jumps{randnum2, randnum3};
        steps.resize(randnum1);
        for(size_t i = 0; i < steps.size(); ++i) {
            steps[i] = rand() + jumps[rand() % 2];
        }
    }
    for(size_t i = 0; i < size; ++i){
        uint64_t step = steps[Xn % steps.size()];
        Xn = (step * Xn + c) % m;
        stream[i] = Xn;
    }
    return stream;
}



//xxd encrypted.bin | head -5