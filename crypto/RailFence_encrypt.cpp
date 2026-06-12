#include "RailFence_encrypt.h"
#include <vector>
#include <cstdint>

using namespace std;

vector<uint8_t> rfEncrypt(vector<uint8_t> data, int rails) {
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

vector<uint8_t> rfDoubleEncrypt(vector<uint8_t> data, int rails1, int rails2) {
    vector<uint8_t> temp = rfEncrypt(data, rails1);
    return rfEncrypt(temp, rails2);
}
