#include "Spiral_decrypt.h"
#include <vector>
#include <cstdint>

using namespace std;

vector<uint8_t> spDecrypt(vector<uint8_t> data, int row, int column) {
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
