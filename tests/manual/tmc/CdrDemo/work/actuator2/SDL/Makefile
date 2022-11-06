# SDL
#Taste::Startup_Priority 1
#TASTE_IV_Properties::is_instance_of Actuator
SEDS_CONVERTER?=spacecreator.AppImage --sedsconverter
OPENGEODE?=opengeode
VISUAL?=kate

all: compile-linux

clean:
	rm -rf obj

edit:
	cd src && $(OPENGEODE) system_structure.pr actuator2.pr

sdl-to-if:
	cd src &&\
	   sdl2if --output-dir ../../../build/modelchecking  ../src/system_structure.pr ../../../actuator/SDL/src/actuator.pr

# Ada
generate-code:
	mkdir -p code && cd code && \
	   $(OPENGEODE) --taste --toAda  ../src/system_structure.pr ../../../actuator/SDL/src/actuator.pr && \
	:

compile-linux: generate-code
	       ADA_INCLUDE_PATH=../../../actuator/SDL/code/:../../../dataview/Ada gprbuild -p -P actuator2_ada.gpr
	:

