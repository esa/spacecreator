# Observer Makefile
OPENGEODE?=opengeode

all: obs2if

clean: 
	rm -rf *.if

obs2if: $(wildcard *.pr)
	sdl2if --output-dir ../../../build/modelchecking/properties $^ 


