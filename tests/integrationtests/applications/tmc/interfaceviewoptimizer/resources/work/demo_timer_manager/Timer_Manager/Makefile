OPENGEODE?=opengeode

all: compile-linux

clean:
	rm -rf obj

edit:
	cd src && $(OPENGEODE) demo_timer_manager.pr

generate-code:
	mkdir -p code && cd code && \
	   cp ../../../dataview/dataview-uniq.asn ../src && \
	   $(OPENGEODE) --taste --simu --toAda ../src/demo_timer_manager.pr && \
	   mv *.gpr ..

compile-linux: generate-code
	CODE_PATH=code ADA_INCLUDE_PATH=../../../dataview/Ada gprbuild -p -P demo_timer_manager_ada.gpr
