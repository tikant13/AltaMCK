#include <iostream>
#include <vector>
#include <string>
#include <cstdint>
using namespace std;


vector<uint8_t> encrypt(vector <uint8_t> data, int rails) {
  if (rails < 2) {
    return data;
  }
  vector<vector<uint8_t>> fence(rails);
  int rail = 0;
  int dir = 1;
  for (int i = 0; i < data.size(); ++i) {
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

vector<uint8_t> decrypt(vector <uint8_t> data, int rails) {
  if (rails < 2) {
    return data;
  }
  vector<int> lengths(rails, 0);
  int rail = 0;
  int dir = 1;
  for (int i = 0; i < data.size(); ++i) {
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
  for (int i = 0; i < data.size(); ++i) {
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

vector<uint8_t> doubleEncrypt(vector<uint8_t> data, int rails1, int rails2) {
  vector<uint8_t> temp = encrypt(data, rails1);
  return encrypt(temp, rails2);
}

vector<uint8_t> doubleDecrypt(vector<uint8_t> data, int rails1, int rails2) {
  vector<uint8_t> temp = decrypt(data, rails2);
  return decrypt(temp, rails1);
}

void checkRails(int& rails) {
  if (rails < 2) {
    cout << "Number of rails can't be lower than 2. Defaulting to 2" << endl;
  }
  rails = 2;
}

int main() {
  cout << "Enter the string to encrypt: ";
  string data;
  getline(cin, data);

  cout << "Number of rails for the first pass: ";
  int rails1;
  cin >> rails1;
  checkRails(rails1);

  cout << "Number of rails for the second pass: ";
  int rails2;
  cin >> rails2;
  checkRails(rails2);

  cout << endl;

  vector<uint8_t> bytes(data.begin(), data.end());

  vector<uint8_t> encrypted = doubleEncrypt(bytes, rails1, rails2);
  string enc = string(encrypted.begin(), encrypted.end());

  vector<uint8_t> decrypted = doubleDecrypt(encrypted, rails1, rails2);
  string dec = string(decrypted.begin(), decrypted.end());

  cout << "Encrypt: " << enc << endl;
  cout << "Decrypt: " << dec;

  cout << endl;
}

