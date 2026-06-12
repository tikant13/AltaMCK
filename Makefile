CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I include

ifeq ($(OS), Windows_NT)
    LIB_EXT = dll
    LDFLAGS = 
else
    LIB_EXT = so
    LDFLAGS = -ldl
endif

all: build_dir \
     build/libRSA.$(LIB_EXT) \
     build/libRC4.$(LIB_EXT) \
     build/libxor.$(LIB_EXT) \
     build/liblcg.$(LIB_EXT) \
     build/libspiral.$(LIB_EXT) \
     build/librailfence.$(LIB_EXT) \
     build/main

build_dir:
	mkdir -p build

build/libRSA.$(LIB_EXT): crypto/RSA.cpp
	$(CXX) -fPIC -shared $^ -o $@

build/libRC4.$(LIB_EXT): crypto/RC4.cpp
	$(CXX) -fPIC -shared $^ -o $@

build/libxor.$(LIB_EXT): crypto/XOR_encryption.cpp crypto/XOR_decryption.cpp crypto/XOR_generation_key.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

build/liblcg.$(LIB_EXT): crypto/LCG_encryption.cpp crypto/LCG_decryption.cpp crypto/LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

build/libspiral.$(LIB_EXT): crypto/Spiral_encrypt.cpp crypto/Spiral_decrypt.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

build/librailfence.$(LIB_EXT): crypto/RailFence_encrypt.cpp crypto/RailFence_decrypt.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

build/main: src/main.cpp \
            src/menuRSA.cpp \
            src/menuRC4.cpp \
            src/menuXOR.cpp \
            src/menuLCG.cpp \
            src/menuRailFence.cpp \
            src/menuSpiral.cpp \
            crypto/XOR_encryption.cpp \
            crypto/XOR_decryption.cpp \
            crypto/XOR_generation_key.cpp \
            crypto/LCG_encryption.cpp \
            crypto/LCG_decryption.cpp \
            crypto/LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

build/xor_test: crypto/XOR_encryption.cpp crypto/XOR_decryption.cpp crypto/XOR_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

build/rc4_test: crypto/RC4.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

build/lcg_test: crypto/LCG_encryption.cpp crypto/LCG_decryption.cpp crypto/LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf build

.PHONY: all build_dir clean

