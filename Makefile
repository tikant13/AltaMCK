CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include
LDFLAGS = -ldl

ifeq ($(OS), Windows_NT)
    LIB_EXT = dll
else
    LIB_EXT = so
endif

all: build_dir build/libRSA.$(LIB_EXT) build/libxor.$(LIB_EXT) build/liblcg.$(LIB_EXT) build/main build/xor_test build/lcg_test

build_dir:
	mkdir -p build

build/libRSA.$(LIB_EXT): crypto/RSA.cpp
	$(CXX) -fPIC -shared $^ -o $@

build/libxor.$(LIB_EXT): crypto/XOR_encryption.cpp crypto/XOR_decryption.cpp crypto/XOR_generation_key.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

build/liblcg.$(LIB_EXT): crypto/LCG_encryption.cpp crypto/LCG_decryption.cpp crypto/LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

build/main: src/main.cpp src/menuRSA.cpp src/menuXOR.cpp src/menuLCG.cpp crypto/XOR_encryption.cpp crypto/XOR_decryption.cpp crypto/XOR_generation_key.cpp crypto/LCG_encryption.cpp crypto/LCG_decryption.cpp crypto/LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build/xor_test: crypto/XOR_test.cpp crypto/XOR_encryption.cpp crypto/XOR_decryption.cpp crypto/XOR_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

build/lcg_test: crypto/LCG_test.cpp crypto/LCG_encryption.cpp crypto/LCG_decryption.cpp crypto/LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf build