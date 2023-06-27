#Taste::Instances_Max 1
#Taste::Instances_Min 1
#Taste::Startup_Priority 1
#TASTE_IV_Properties::is_instance_of Lock
SEDS_CONVERTER?=spacecreator.AppImage --sedsconverter
OPENGEODE?=opengeode
VISUAL?=kate

all: compile-linux

clean:
	rm -rf obj

edit:
	cd src && $(OPENGEODE) system_structure.pr lockb.pr

sdl-to-if:
	cd src &&\
	   sdl2if --output-dir ../../../build/modelchecking  ../src/system_structure.pr ../../../lock/SDL/src/lock.pr

# Ada
generate-code:
	mkdir -p code && cd code && \
	   $(OPENGEODE) --taste --simu --toAda  ../src/system_structure.pr ../../../lock/SDL/src/lock.pr && \
	:

compile-linux: generate-code
	       ADA_INCLUDE_PATH=../../../lock/SDL/code/:../../../dataview/Ada gprbuild -p -P lockb_ada.gpr
	:

