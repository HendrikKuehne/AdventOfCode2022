.PHONY : clean

%.app : %.cpp
	clang++ -O3 -std=c++17 -pedantic -o $@ $*.cpp

clean :
	rm *.app