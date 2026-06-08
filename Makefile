
CXX = g++

CXXFLAGS = -Wall -Wextra -I include
LDFLAGS = -ldl


all: build_dir build/libRSA.so build/main


build_dir:
	mkdir -p build


build/libRSA.so: crypto/RSA.cpp
	$(CXX) -fPIC -shared -o build/libRSA.so crypto/RSA.cpp


build/main: src/main.cpp src/menuRSA.cpp
	$(CXX) $(CXXFLAGS) src/main.cpp src/menuRSA.cpp -o build/main $(LDFLAGS)


clean:
	rm -rf build