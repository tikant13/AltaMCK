#include "cipher_api.h"
#include "RailFence_encrypt.h"
#include "RailFence_decrypt.h"
#include <vector>
#include <algorithm>

using namespace std;

static const CipherInfo INFO = {
    "railfence", 2
};

extern "C" const CipherInfo* getCipherInfo() {
    return &INFO;
}

extern "C" size_t getOutputSize(size_t inputSize) {
    return inputSize;
}

extern "C" int cipherEncrypt(ConstBuf key, ConstBuf input, MutBuf* output) {
    if (!output || !output->data) return 1;
    if (key.size < 2) return 3;
    int rails1 = key.data[0];
    int rails2 = key.data[1];
    vector<uint8_t> data(input.data, input.data + input.size);
    vector<uint8_t> res = rfDoubleEncrypt(data, rails1, rails2);
    if (output->size < res.size()) return 2;
    copy(res.begin(), res.end(), output->data);
    output->size = res.size();
    return 0;
}

extern "C" int cipherDecrypt(ConstBuf key, ConstBuf input, MutBuf* output) {
    if (!output || !output->data) return 1;
    if (key.size < 2) return 3;
    int rails1 = key.data[0];
    int rails2 = key.data[1];
    vector<uint8_t> data(input.data, input.data + input.size);
    vector<uint8_t> res = rfDoubleDecrypt(data, rails1, rails2);
    if (output->size < res.size()) return 2;
    copy(res.begin(), res.end(), output->data);
    output->size = res.size();
    return 0;
}
