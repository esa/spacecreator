# SDL
#TASTE::Is_Component_Type true
#Taste::Startup_Priority 1
SEDS_CONVERTER?=spacecreator.AppImage --sedsconverter
OPENGEODE?=opengeode
VISUAL?=kate

all: compile-linux

clean:
	rm -rf obj

edit:
	cd src && $(OPENGEODE) system_structure.pr actuator.pr

sdl-to-if:
	cd src &&\
	   sdl2if --output-dir ../../../build/modelchecking  ../src/actuator.pr ../src/system_structure.pr

# Ada
generate-code:
	mkdir -p code && cd code && \
	   $(OPENGEODE) --taste --toAda ../src/actuator.pr ../src/system_structure.pr && \
	:

compile-linux: generate-code
	:

