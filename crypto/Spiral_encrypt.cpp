#include "Spiral_encrypt.h"
#include <vector>
#include <cstdint>

using namespace std;

vector<uint8_t> spEncrypt(vector<uint8_t> data, int row, int column) {
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
