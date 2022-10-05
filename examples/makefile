UNAME_S:=$(shell uname -s)
ifeq ($(UNAME_S),Darwin)
	CXX:=clang++
else
	CXX:=g++
endif


all:
	$(CXX) -std=c++11 writeMhs.cpp -o writeMhs.exe -ladios2_cxx11 -lpthread
	$(CXX) -std=c++11 readMhs.cpp -o readMhs.exe -ladios2_cxx11 -lpthread
	$(CXX) -std=c++11 daemonMhs.cpp -o daemonMhs.exe -ladios2_cxx11 -lpthread
	$(CXX) -std=c++11 inquirerMhs.cpp -o inquirerMhs.exe -ladios2_cxx11 -lpthread


clean:
	rm -f *.exe
