CXX = g++
CXXFLAGS = -std=c++17 -Wall -Werror

ifeq ($(OS), Windows_NT)
    LIB_EXT = dll
else
    LIB_EXT = so
endif

lcg_test: LCG_test.cpp LCG_encryption.cpp LCG_decryption.cpp LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) $^ -o $@

liblcg.$(LIB_EXT): LCG_encryption.cpp LCG_decryption.cpp LCG_generation_key.cpp
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

all: lcg_test liblcg.$(LIB_EXT)

clean:
	rm -f lcg_test liblcg.so liblcg.dll