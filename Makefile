CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
BUILD = build

all: buildDir \
     $(BUILD)/librailfence.so \
     $(BUILD)/libspiral.so \
     $(BUILD)/main

buildDir:
	mkdir -p $(BUILD)

$(BUILD)/librailfence.so: railfence/railfence.cpp cipher_api.h
	$(CXX) $(CXXFLAGS) -shared -fPIC $< -o $@

$(BUILD)/libspiral.so: spiral/spiral.cpp cipher_api.h
	$(CXX) $(CXXFLAGS) -shared -fPIC $< -o $@

$(BUILD)/main: main/main.cpp menu/menuRailFence.cpp menu/menuSpiral.cpp cipher_api.h
	$(CXX) $(CXXFLAGS) main/main.cpp menu/menuRailFence.cpp menu/menuSpiral.cpp -o $@ -ldl

clean:
	rm -rf $(BUILD)

.PHONY: all buildDir clean
