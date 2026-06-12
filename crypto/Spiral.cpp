#include "cipher_api.h"
#include "Spiral_encrypt.h"
#include "Spiral_decrypt.h"
#include <vector>
#include <algorithm>

using namespace std;

static const CipherInfo INFO = {
    "spiral", 2
};

extern "C" const CipherInfo* getCipherInfo() {
    return &INFO;
}

extern "C" size_t getOutputSize(size_t inputSize) {
    return inputSize + 256;
}

extern "C" int cipherEncrypt(ConstBuf key, ConstBuf input, MutBuf* output) {
    if (!output || !output->data) return 1;
    if (key.size < 2) return 3;
    int row = key.data[0];
    int column = key.data[1];
    if (row < 2) row = 2;
    if (column < 2) column = 2;
    vector<uint8_t> data(input.data, input.data + input.size);
    vector<uint8_t> res = spEncrypt(data, row, column);
    if (output->size < res.size()) return 2;
    copy(res.begin(), res.end(), output->data);
    output->size = res.size();
    return 0;
}

extern "C" int cipherDecrypt(ConstBuf key, ConstBuf input, MutBuf* output) {
    if (!output || !output->data) return 1;
    if (key.size < 2) return 3;
    int row = key.data[0];
    int column = key.data[1];
    if (row < 2) row = 2;
    if (column < 2) column = 2;
    vector<uint8_t> data(input.data, input.data + input.size);
    vector<uint8_t> res = spDecrypt(data, row, column);
    if (output->size < res.size()) return 2;
    copy(res.begin(), res.end(), output->data);
    output->size = res.size();
    return 0;
}
