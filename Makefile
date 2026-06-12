CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
BUILD = build

ifeq ($(OS), Windows_NT)
    LIB_EXT = dll
else
    LIB_EXT = so
endif

RF_SRC = railfence/encrypt.cpp railfence/decrypt.cpp
SP_SRC = spiral/encrypt.cpp spiral/decrypt.cpp
MENU_SRC = menu/menuRailFence.cpp menu/menuSpiral.cpp

all: build_dir \
     $(BUILD)/librailfence.$(LIB_EXT) \
     $(BUILD)/libspiral.$(LIB_EXT) \
     $(BUILD)/main \

build_dir:
	mkdir -p $(BUILD)

$(BUILD)/librailfence.$(LIB_EXT): $(RF_SRC)
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

$(BUILD)/libspiral.$(LIB_EXT): $(SP_SRC)
	$(CXX) $(CXXFLAGS) -shared -fPIC $^ -o $@

$(BUILD)/main: main/main.cpp $(MENU_SRC) $(RF_SRC) $(SP_SRC)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(BUILD)

.PHONY: all build_dir clean
