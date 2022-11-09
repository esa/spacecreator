# Observer Makefile
OPENGEODE?=opengeode
SPACECREATOR?=spacecreator.AppImage
TOOL_INST       := $(shell taste-config --prefix)


MSC = $(wildcard *.msc)
OBS = $(patsubst %.msc, %.pr, $(wildcard *.msc))


all: obs2if

clean:
	rm -rf *.if

%.pr:  $(MSC)
	$(SPACECREATOR) --mscconverter --open-msc $< --template $(TOOL_INST)/share/modelchecking/msc2obs/src/msc2obs_propertycomment.tmplt --export $@
	sed -i 's/\(\w\)-/\1_/g' $@

obs2if: $(OBS)
	sdl2if --output-dir ../../../build/modelchecking/properties *.pr


