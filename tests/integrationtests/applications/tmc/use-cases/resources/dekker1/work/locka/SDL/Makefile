# SDL
#TASTE_IV_Properties::is_instance_of Lock
OPENGEODE?=opengeode

all: compile-linux

clean:
	rm -rf obj

edit:
	cd src && $(OPENGEODE) system_structure.pr locka.pr

sdl-to-if:
	cd src &&\
	   sdl2if --output-dir ../../../build/modelchecking  ../src/system_structure.pr ../../../lock/SDL/src/lock.pr

# Ada
generate-code:
	mkdir -p code && cd code && \
	   $(OPENGEODE) --taste --toAda  ../src/system_structure.pr ../../../lock/SDL/src/lock.pr && \
	:

compile-linux: generate-code
	       ADA_INCLUDE_PATH=../../../lock/SDL/code/:../../../dataview/Ada gprbuild -p -P locka_ada.gpr
	:

