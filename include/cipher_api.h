#pragma once
#include <stddef.h>
#include <stdint.h>

struct CipherInfo {
    const char* name;
    size_t keySize;
};

struct ConstBuf {
    const uint8_t* data;
    size_t size;
};

struct MutBuf {
    uint8_t* data;
    size_t size;
};

extern "C" const CipherInfo* getCipherInfo();
extern "C" size_t getOutputSize(size_t inputSize);
extern "C" int cipherEncrypt(ConstBuf key, ConstBuf input, MutBuf* output);
extern "C" int cipherDecrypt(ConstBuf key, ConstBuf input, MutBuf* output);
