#include <iostream>
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

vector<uint8_t> encrypt(vector<uint8_t> data, int row, int column) {
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
    return result;
}


vector<uint8_t> decrypt(vector<uint8_t> data, int row, int column) {
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
    return result;
}



void checkMatrixRow(int& row) {
    if (row < 2) {
        cout << "Number of rows can't be lower than 2. Defaulting to 2" << endl;
    }
    row = 2;
}

void checkMatrixCol(int& column) {
    if (column < 2) {
        cout << "Number of columns can't be lower than 2. Defaulting to 2" << endl;
    }
    column = 2;
}

int main() {
    cout << "Enter the string to encrypt: ";
    string data;
    getline(cin, data);

    cout << "Number of rows: ";
    int n;
    cin >> n;
    checkMatrixRow(n);
    cout << endl;

    cout << "Number of columns: ";
    int m;
    cin >> m;
    checkMatrixCol(m);
    cout << endl;

    vector<uint8_t> bytes(data.begin(), data.end());

    vector<uint8_t> encrypted = encrypt(bytes, n, m);
    string enc = string(encrypted.begin(), encrypted.end());

    vector<uint8_t> decrypted = decrypt(encrypted, n, m);
    string dec = string(decrypted.begin(), decrypted.end());

    cout << endl;
    cout << "Encrypt: " << enc << endl;
    cout << "Decrypt: " << dec << endl;
}

