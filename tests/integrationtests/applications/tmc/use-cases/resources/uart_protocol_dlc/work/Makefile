# MAKEFILE_PATH   := $(abspath $(lastword $(MAKEFILE_LIST)))
MAKEFILE_PATH   := $(lastword $(MAKEFILE_LIST))
TOOL_INST       := $(shell taste-config --prefix)
OUTDIR          := $(dir $(MAKEFILE_PATH))
TASTE_SHARED_TYPES?=${HOME}/.local/share/QtProject/QtCreator/shared_types
ASN1_FILES      := ../data_model/basic_types.asn ../data_model/bilevel_types.asn ../data_model/uart_dlc.asn ${TOOL_INST}/share/taste-types/taste-types.asn ../uart_protocol_dlc.asn dataview/system.asn
CP_FILES        :=       # Context parameters
DATAVIEW_PATH   := ${OUTDIR}/dataview
DATAVIEW_UNIQ   := ${DATAVIEW_PATH}/dataview-uniq.asn
DATAVIEW_ACN    := ${DATAVIEW_PATH}/dataview-uniq.acn
DATAVIEW_AADLV1 := ${DATAVIEW_PATH}/dataview_aadlv1.aadl
DATAVIEW_AADLV2 := ${DATAVIEW_PATH}/dataview_aadlv2.aadl
#TASTE_SHARED_TYPES?=${TOOL_INST}/share/SharedTypes  # Ellidiss only

# Project cache folder is used to speed up all ASN.1-related processing
export PROJECT_CACHE=${HOME}/.taste_AST_cache



# Extract the list of properties from the current model checking configuration
# that is set in the GUI and saved in .mcconfig.xml when calling the model checkers
PROPERTIES_CONFIG := $(shell xmllint --xpath xmel/ModelCheckingWindow/Configuration/Properties/Property/@path modelchecking/configurations/.mcconfig.xml | grep -Po "(?<= path=\")[a-zA-Z0-9-_./]*(?=\")")

ifneq ($(shell xmllint --xpath "string(xmel/ModelCheckingWindow/Configuration/Subtyping/@name)" modelchecking/configurations/.mcconfig.xml),)
SUBTYPE := $(shell xmllint --xpath "string(xmel/ModelCheckingWindow/Configuration/Subtyping/@name)" modelchecking/configurations/.mcconfig.xml)
else
SUBTYPE := .default_subtypes.asn
endif

# Build an interactive simulator and model checker (experimental)
all:	${DATAVIEW_PATH}/iterators/asn1_ada_iterators.ads
	$(MAKE) -j -C build -f Makefile.taste simu
	mkdir -p simulation/mcsrc
	cp -u ${TOOL_INST}/share/simu-common/mc.ad[sb] simulation/mcsrc
	cp -u ${TOOL_INST}/share/simu-common/modelcheck.adb simulation/mcsrc
	cp -u ${TOOL_INST}/share/simu-common/mc.gpr simulation
	cp -u ${TOOL_INST}/share/simu-common/guilayout.ui simulation
	cp -u ${TOOL_INST}/share/simu-common/simu_params.ui simulation
	cp -u ${TOOL_INST}/share/simu-common/toolbar.ui simulation
	$(info [-] Processing observers ${PROPERTIES_CONFIG})
	cd simulation && ln -fs observers/observer.asn . && taste-process-observers ${PROPERTIES_CONFIG}

# Create the ASN.1 Iterators that are used by the native model checker
${DATAVIEW_PATH}/iterators/asn1_ada_iterators.ads:	${DATAVIEW_PATH}/built
	cd ${DATAVIEW_PATH} && taste-asn1-iterators --raw dataview-uniq.asn

obs-skeleton:
	sed 's/my_observer/$(NAME)/g' $(TOOL_INST)/share/modelchecking/skeletons/observer.pr > modelchecking/properties/$(NAME)/$(NAME).pr
	cp $(TOOL_INST)/share/modelchecking/skeletons/Makefile.observer modelchecking/properties/$(NAME)/Makefile
	cp ${PWD}/work/simulation/observers/observer.asn modelchecking/properties/$(NAME)/

msc-skeleton:
	sed 's/Untitled_Document/$(NAME)/g' $(TOOL_INST)/share/modelchecking/skeletons/msc.msc > modelchecking/properties/$(NAME)/$(NAME).msc
	cp $(TOOL_INST)/share/modelchecking/skeletons/Makefile.msc modelchecking/properties/$(NAME)/Makefile
	cp ${PWD}/work/simulation/observers/observer.asn modelchecking/properties/$(NAME)/

bsc-skeleton:
	sed 's/my_observer/$(NAME)/g' $(TOOL_INST)/share/modelchecking/skeletons/bsc.pr > modelchecking/properties/$(NAME)/$(NAME).pr
	cp $(TOOL_INST)/share/modelchecking/skeletons/Makefile.observer modelchecking/properties/$(NAME)/Makefile
	cp ${PWD}/work/simulation/observers/observer.asn modelchecking/properties/$(NAME)/

model-check : aadl2glueC build/system_config.h ${DATAVIEW_PATH}/built ifgen

build/modelchecking:
	mkdir -p build/modelchecking
	mkdir -p build/modelchecking/properties
	mkdir -p build/modelchecking/output


sdl-to-if : bcdlct/SDL/src/bcdlct_sdl2if_built 	demo_timer_manager/Timer_Manager/src/demo_timer_manager_built erroredchannel/SDL/src/erroredchannel_sdl2if_built rtdlcr/SDL/src/rtdlcr_sdl2if_built

bcdlct/SDL/src/bcdlct_sdl2if_built :
	$(MAKE) -C bcdlct/SDL generate-code
	$(MAKE) -C bcdlct/SDL sdl-to-if

demo_timer_manager/Timer_Manager/src/demo_timer_manager_built :
	$(MAKE) -C demo_timer_manager/Timer_Manager generate-code

erroredchannel/SDL/src/erroredchannel_sdl2if_built :
	$(MAKE) -C erroredchannel/SDL generate-code
	$(MAKE) -C erroredchannel/SDL sdl-to-if

rtdlcr/SDL/src/rtdlcr_sdl2if_built :
	$(MAKE) -C rtdlcr/SDL generate-code
	$(MAKE) -C rtdlcr/SDL sdl-to-if




modelchecking/subtypes:
	mkdir -p modelchecking/subtypes

modelchecking/subtypes/.default_subtypes.asn: modelchecking/subtypes
	find . -path ./binaries -prune -o -name subtype_*.asn -exec cat {} \; >  $@

obs-to-if: $(PROPERTIES_CONFIG)


$(PROPERTIES_CONFIG):
	$(MAKE) -C modelchecking/properties/$(dir $@)

aadl-to-if:
	cd Dump/IF &&\
	mv * ../../build/modelchecking

asn-to-if:
	cd Dump/IF &&\
	asn2if ../../dataview/dataview-uniq.asn dataview.if

ifgen: build/modelchecking sdl-to-if asn-to-if aadl-to-if obs-to-if
	$(MAKE) -C build -f Makefile.taste model-check



# Check that all runtimes used in the system are installed
check_targets:
	$(MAKE) -C build -f Makefile.taste check_targets

# Generate the language-specific data view needed before editing user code
dataview:	dataview/Ada/built
	# Copy the dataview to SDL/SEDS function BCDLCT (no context parameters)
	cp -u ${DATAVIEW_UNIQ} bcdlct/SDL/src
	# Compute MD5 of the dataview, needed to decide if code needs to be re-generated
	md5sum -c "bcdlct/SDL/src/dataview.md5"  &> /dev/null || md5sum  "bcdlct/SDL/src/dataview-uniq.asn" >  "bcdlct/SDL/src/dataview.md5"
	# Copy the dataview to SDL/SEDS function ErroredChannel (no context parameters)
	cp -u ${DATAVIEW_UNIQ} erroredchannel/SDL/src
	# Compute MD5 of the dataview, needed to decide if code needs to be re-generated
	md5sum -c "erroredchannel/SDL/src/dataview.md5"  &> /dev/null || md5sum  "erroredchannel/SDL/src/dataview-uniq.asn" >  "erroredchannel/SDL/src/dataview.md5"
	# Copy the dataview to SDL/SEDS function RTDLCR (no context parameters)
	cp -u ${DATAVIEW_UNIQ} rtdlcr/SDL/src
	# Compute MD5 of the dataview, needed to decide if code needs to be re-generated
	md5sum -c "rtdlcr/SDL/src/dataview.md5"  &> /dev/null || md5sum  "rtdlcr/SDL/src/dataview-uniq.asn" >  "rtdlcr/SDL/src/dataview.md5"

# Generate the complete dataview, including Python bindings if there is a GUI
${DATAVIEW_PATH}/built:  # simulation: no dependency to "dataview" (already built by top-level Makefile)
	touch $@

# Build based on the deployment view
build-default:	aadl2glueC build/system_config.h ${DATAVIEW_PATH}/built build/deploymentview_final/Makefile
	$(MAKE) -j -C build -f Makefile.taste

# Execute the generated binaries (used in Space Creator when using the Run button)
run: # Space creator will always build before, so no need to depend on build-default
	$(MAKE) -j -C build -f Makefile.taste run

# Run the Concurrency View analysis tool (cheddar/marzhin)
run_cv: build/ConcurrencyView.aadl
	TASTECV --load $^

# The Concurrency View needed by Cheddar is the concatenation of system.aadl and ocarina_components.aadl
build/ConcurrencyView.aadl:	build/system.aadl
	cat $^ $(shell taste-config --prefix)/share/ocarina/AADLv2/ocarina_components.aadl > $@

# Rule to invoke Ocarina (generation of POHI middleware code)
build/deploymentview_final/Makefile:	build/main.aadl ${DATAVIEW_AADLV2}
	$(MAKE) -C build -f Makefile.taste ocarina

# Optionally manage SQL Database (run taste-create-database to enable)
sql_db/DV.py:
	mkdir -p sql_db && touch $@

# Create dataview-uniq.asn by concatenating all asn1 input files
${DATAVIEW_UNIQ}:	${ASN1_FILES} modelchecking/subtypes/$(SUBTYPE)
	mkdir -p ${PROJECT_CACHE} && mkdir -p ${DATAVIEW_PATH}
	sed -e '$$s/$$/\n/' -s $^ > $@

# Create dataview-uniq.acn by concatenating all input files
${DATAVIEW_ACN}:	../uart_protocol_dlc.acn
	mkdir -p ${DATAVIEW_PATH}
	sed -e '$$s/$$/\n/' -s $^ > $@

# system_config.h is used to filter out non-needed ASN.1 encoder/decoders
build/system_config.h:	$(wildcard */*/wrappers/*_system_config.h)
	cat $^ > $@

#  Call DMT/aadl2glueC to generate glue code from the mini-cv.aadl files
aadl2glueC:	bcapp/GUI/wrappers/aadl2glueC_built bcdlct/SDL/wrappers/aadl2glueC_built channeleffects/GUI/wrappers/aadl2glueC_built demo_timer_manager/Timer_Manager/wrappers/aadl2glueC_built erroredchannel/SDL/wrappers/aadl2glueC_built rtapp/GUI/wrappers/aadl2glueC_built rtdlcr/SDL/wrappers/aadl2glueC_built

# Build dataview.aadl in AADLv1 format, used by DMT to generate glue code
${DATAVIEW_AADLV1}: ${DATAVIEW_UNIQ}
	asn2aadlPlus  $< $@

# Build dataview.aadl in AADLv2 format, used by Ocarina to generate glue code
${DATAVIEW_AADLV2}: ${DATAVIEW_UNIQ}
	asn2aadlPlus --aadlv2  $< $@

bcapp/GUI/wrappers/aadl2glueC_built: bcapp/GUI/wrappers/bcapp_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@

bcdlct/SDL/wrappers/aadl2glueC_built: bcdlct/SDL/wrappers/bcdlct_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@

channeleffects/GUI/wrappers/aadl2glueC_built: channeleffects/GUI/wrappers/channeleffects_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@

demo_timer_manager/Timer_Manager/wrappers/aadl2glueC_built: demo_timer_manager/Timer_Manager/wrappers/demo_timer_manager_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@

erroredchannel/SDL/wrappers/aadl2glueC_built: erroredchannel/SDL/wrappers/erroredchannel_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@

rtapp/GUI/wrappers/aadl2glueC_built: rtapp/GUI/wrappers/rtapp_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@

rtdlcr/SDL/wrappers/aadl2glueC_built: rtdlcr/SDL/wrappers/rtdlcr_mini_cv.aadl ${DATAVIEW_AADLV1}
	aadl2glueC -allboards -o $(dir $@) $^
	touch $@


# Compile the ASN.1 models in C
dataview/C/built: ${DATAVIEW_UNIQ} ${DATAVIEW_ACN} ${CP_FILES}
	mkdir -p $(dir $@)
	# asn2dataModel does not take the ACN files, and only calls asn1scc
	# asn2dataModel -o $(dir $@) -toC ${DATAVIEW_UNIQ} ${DATAVIEW_ACN}
	asn1scc -typePrefix asn1Scc ${ASN1SCC_FLAGS} -renamePolicy 3 -equal -fp AUTO -o $(dir $@) -c -uPER -ACN $^
	cp ${TOOL_INST}/share/AutoGUI/debug_messages.? dataview/C
	cp ${TOOL_INST}/share/AutoGUI/timeInMS.? dataview/C
	#cd dataview/C && msgPrinter ../dataview-uniq.asn  (disabled: this is not used anywhere)
	cd dataview/C && msgPrinterASN1 ../dataview-uniq.asn
	touch $@

# Compile the ASN.1 models in Python for the autogenerated GUIs
dataview/Python/built:	dataview/C/built bcapp/GUI/wrappers/aadl2glueC_built channeleffects/GUI/wrappers/aadl2glueC_built rtapp/GUI/wrappers/aadl2glueC_built sql_db/DV.py
	mkdir -p $(dir $@)
	asn2dataModel -o $(dir $@) -toPython ${DATAVIEW_UNIQ}
	$(MAKE) -C dataview/Python -f Makefile.python
	cp dataview/Python/* bcapp/GUI/wrappers/python
	cp dataview/C/timeInMS.[ch] dataview/C/debug_messages.c bcapp/GUI/wrappers/python
	cd bcapp/GUI/wrappers/python && \
	   gcc -g -fPIC -c  gui_api.c ../../src/queue_manager.c timeInMS.c debug_messages.c -I. -I../../src && \
	   gcc -g -shared -o PythonAccess.so gui_api.o queue_manager.o timeInMS.o debug_messages.o -lrt
	mkdir -p binaries/bcapp_GUI
	cp -f bcapp/GUI/wrappers/python/* binaries/bcapp_GUI
	cp -f bcapp/GUI/wrappers/*.py binaries/bcapp_GUI
	cp -f bcapp/GUI/wrappers/*.ui binaries/bcapp_GUI
	echo "errCodes = \\" >> binaries/bcapp_GUI/datamodel.py
	taste-asn1-errCodes binaries/bcapp_GUI/dataview-uniq.h >> binaries/bcapp_GUI/datamodel.py
	if [ -f sql_db/db.info ]; then cd binaries/bcapp_GUI && ln -fs ../../sql_db; fi
	#  Interface view must be placed in the gui folder to allow replaying MSCs
	cp -u ../InterfaceView.aadl binaries/bcapp_GUI
	cp dataview/Python/* channeleffects/GUI/wrappers/python
	cp dataview/C/timeInMS.[ch] dataview/C/debug_messages.c channeleffects/GUI/wrappers/python
	cd channeleffects/GUI/wrappers/python && \
	   gcc -g -fPIC -c  gui_api.c ../../src/queue_manager.c timeInMS.c debug_messages.c -I. -I../../src && \
	   gcc -g -shared -o PythonAccess.so gui_api.o queue_manager.o timeInMS.o debug_messages.o -lrt
	mkdir -p binaries/channeleffects_GUI
	cp -f channeleffects/GUI/wrappers/python/* binaries/channeleffects_GUI
	cp -f channeleffects/GUI/wrappers/*.py binaries/channeleffects_GUI
	cp -f channeleffects/GUI/wrappers/*.ui binaries/channeleffects_GUI
	echo "errCodes = \\" >> binaries/channeleffects_GUI/datamodel.py
	taste-asn1-errCodes binaries/channeleffects_GUI/dataview-uniq.h >> binaries/channeleffects_GUI/datamodel.py
	if [ -f sql_db/db.info ]; then cd binaries/channeleffects_GUI && ln -fs ../../sql_db; fi
	#  Interface view must be placed in the gui folder to allow replaying MSCs
	cp -u ../InterfaceView.aadl binaries/channeleffects_GUI
	cp dataview/Python/* rtapp/GUI/wrappers/python
	cp dataview/C/timeInMS.[ch] dataview/C/debug_messages.c rtapp/GUI/wrappers/python
	cd rtapp/GUI/wrappers/python && \
	   gcc -g -fPIC -c  gui_api.c ../../src/queue_manager.c timeInMS.c debug_messages.c -I. -I../../src && \
	   gcc -g -shared -o PythonAccess.so gui_api.o queue_manager.o timeInMS.o debug_messages.o -lrt
	mkdir -p binaries/rtapp_GUI
	cp -f rtapp/GUI/wrappers/python/* binaries/rtapp_GUI
	cp -f rtapp/GUI/wrappers/*.py binaries/rtapp_GUI
	cp -f rtapp/GUI/wrappers/*.ui binaries/rtapp_GUI
	echo "errCodes = \\" >> binaries/rtapp_GUI/datamodel.py
	taste-asn1-errCodes binaries/rtapp_GUI/dataview-uniq.h >> binaries/rtapp_GUI/datamodel.py
	if [ -f sql_db/db.info ]; then cd binaries/rtapp_GUI && ln -fs ../../sql_db; fi
	#  Interface view must be placed in the gui folder to allow replaying MSCs
	cp -u ../InterfaceView.aadl binaries/rtapp_GUI
	touch $@

dataview/Ada/built: ${DATAVIEW_UNIQ} ${CP_FILES}
	mkdir -p $(dir $@)
	asn2dataModel -allboards -o $(dir $@) -toAda $^
	touch $@
	cd dataview/Ada && rm -f gnat.cfg GPS_project.gpr IgnoredExaminerWarnings.wrn runSpark.sh

clean:
	rm -rf binaries dataview
	$(MAKE) -C build -f Makefile.taste clean

.PHONY: clean simu compile-all-linux function-instances-linux function-types-linux build-default dataview build-rtems-ada dataview build/modelchecking modelchecking/subtypes/$(SUBTYPE) obs-to-if aadl-to-if asn-to-if $(PROPERTIES_CONFIG) run_cv

