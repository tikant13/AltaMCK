#include "../cipher_api.h"
#include <vector>
#include <cstdint>
#include <algorithm>

using namespace std;

static vector<uint8_t> spEncrypt(vector<uint8_t> data, int row, int column) {
    if (row * column < (int)data.size()) {
        row = (data.size() + column - 1) / column;
    }
    vector<vector<uint8_t>> matrix(row, vector<uint8_t>(column, 0));
    int id = 0;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            if (id < (int)data.size()) {
                matrix[i][j] = data[id];
                id++;
            }
        }
    }
    vector<vector<uint8_t>> transposed(column, vector<uint8_t>(row, 0));
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            transposed[j][i] = matrix[i][j];
        }
    }
    int newRow = column;
    int newCol = row;
    int top = 0;
    int bottom = newRow - 1;
    int left = 0;
    int right = newCol - 1;
    vector<uint8_t> result;
    while (top <= bottom && left <= right) {
        {
            int i = top;
            for (int j = left; j <= right; ++j) {
                result.push_back(transposed[i][j]);
            }
            top++;
        }
        {
            if (top <= bottom) {
                int j = right;
                for (int i = top; i <= bottom; ++i) {
                    result.push_back(transposed[i][j]);
                }
                right--;
            }
        }
        {
            if (top <= bottom) {
                int i = bottom;
                for (int j = right; j >= left; --j) {
                    result.push_back(transposed[i][j]);
                }
                bottom--;
            }
        }
        {
            if (left <= right) {
                int j = left;
                for (int i = bottom; i >= top; --i) {
                    result.push_back(transposed[i][j]);
                }
                left++;
            }
        }
    }
    while (!result.empty() && result.back() == 0) {
        result.pop_back();
    }
    return result;
}

static vector<uint8_t> spDecrypt(vector<uint8_t> data, int row, int column) {
    if (row * column < (int)data.size()) {
        row = (data.size() + column - 1) / column;
    }
    int newRow = column;
    int newCol = row;
    vector<vector<uint8_t>> transposed(newRow, vector<uint8_t>(newCol, 0));
    int id = 0;
    int top = 0;
    int bottom = newRow - 1;
    int left = 0;
    int right = newCol - 1;
    while (top <= bottom && left <= right) {
        {
            int i = top;
            for (int j = left; j <= right; ++j) {
                if (id < (int)data.size()) {
                    transposed[i][j] = data[id++];
                }
            }
            top++;
        }
        {
            if (top <= bottom) {
                int j = right;
                for (int i = top; i <= bottom; ++i) {
                    if (id < (int)data.size()) {
                        transposed[i][j] = data[id++];
                    }
                }
                right--;
            }
        }
        {
            if (top <= bottom) {
                int i = bottom;
                for (int j = right; j >= left; --j) {
                    if (id < (int)data.size()) {
                        transposed[i][j] = data[id++];
                    }
                }
                bottom--;
            }
        }
        {
            if (left <= right) {
                int j = left;
                for (int i = bottom; i >= top; --i) {
                    if (id < (int)data.size()) {
                        transposed[i][j] = data[id++];
                    }
                }
                left++;
            }
        }
    }
    vector<vector<uint8_t>> matrix(row, vector<uint8_t>(column, 0));
    for (int i = 0; i < newRow; ++i) {
        for (int j = 0; j < newCol; ++j) {
            matrix[j][i] = transposed[i][j];
        }
    }
    vector<uint8_t> result;
    for (int i = 0; i < row; ++i) {
        for (int j = 0; j < column; ++j) {
            result.push_back(matrix[i][j]);
        }
    }
    while (!result.empty() && result.back() == 0) {
        result.pop_back();
    }
    return result;
}

static const CipherInfo INFO = { "spiral", 2 };

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
