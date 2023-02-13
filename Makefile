.PHONY : clean

%.app :
	clang++ -O3 -pedantic -o $@ $*/$*.cpp

clean :
	rm *.app