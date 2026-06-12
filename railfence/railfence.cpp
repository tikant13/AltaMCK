#include "../cipher_api.h"
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

static vector<uint8_t> rfEncrypt(vector<uint8_t> data, int rails) {
    if (rails < 2) {
        return data;
    }
    vector<vector<uint8_t>> fence(rails);
    int rail = 0;
    int dir = 1;
    for (int i = 0; i < (int)data.size(); ++i) {
        fence[rail].push_back(data[i]);
        if (rail == 0) {
            dir = 1;
        }
        else if (rail == rails - 1) {
            dir = -1;
        }
        rail += dir;
    }
    vector<uint8_t> result;
    for (int i = 0; i < rails; ++i) {
        for (uint8_t b : fence[i]) {
            result.push_back(b);
        }
    }
    return result;
}

static vector<uint8_t> rfDecrypt(vector<uint8_t> data, int rails) {
    if (rails < 2) {
        return data;
    }
    vector<int> lengths(rails, 0);
    int rail = 0;
    int dir = 1;
    for (int i = 0; i < (int)data.size(); ++i) {
        lengths[rail]++;
        if (rail == 0) {
            dir = 1;
        }
        else if (rail == rails - 1) {
            dir = -1;
        }
        rail += dir;
    }
    vector<vector<uint8_t>> fence(rails);
    int pos = 0;
    for (int i = 0; i < rails; ++i) {
        for (int j = 0; j < lengths[i]; ++j) {
            fence[i].push_back(data[pos]);
            pos++;
        }
    }
    vector<int> id(rails, 0);
    rail = 0;
    dir = 1;
    vector<uint8_t> result;
    for (int i = 0; i < (int)data.size(); ++i) {
        result.push_back(fence[rail][id[rail]]);
        id[rail]++;
        if (rail == 0) {
            dir = 1;
        }
        else if (rail == rails - 1) {
            dir = -1;
        }
        rail += dir;
    }
    return result;
}

static vector<uint8_t> rfDoubleEncrypt(vector<uint8_t> data, int rails1, int rails2) {
    vector<uint8_t> temp = rfEncrypt(data, rails1);
    return rfEncrypt(temp, rails2);
}

static vector<uint8_t> rfDoubleDecrypt(vector<uint8_t> data, int rails1, int rails2) {
    vector<uint8_t> temp = rfDecrypt(data, rails2);
    return rfDecrypt(temp, rails1);
}

static const CipherInfo INFO = { "railfence", 2 };

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
