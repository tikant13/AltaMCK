CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror

ifeq ($(OS), Windows_NT)
    LIB_EXT = dll
else
    LIB_EXT = so
endif

xor_test: XOR_test.cpp XOR_encryption.cpp XOR_decryption.cpp XOR_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

libxor.$(LIB_EXT): XOR_encryption.cpp XOR_decryption.cpp XOR_generation_key.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

all: xor_test libxor.$(LIB_EXT)

clean:
	rm -f xor_test libxor.so libxor.dll