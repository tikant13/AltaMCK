#include "RailFence_decrypt.h"
#include <vector>
#include <cstdint>

using namespace std;

vector<uint8_t> rfDecrypt(vector<uint8_t> data, int rails) {
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

vector<uint8_t> rfDoubleDecrypt(vector<uint8_t> data, int rails1, int rails2) {
    vector<uint8_t> temp = rfDecrypt(data, rails2);
    return rfDecrypt(temp, rails1);
}
