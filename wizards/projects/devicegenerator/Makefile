KAZOO?=kazoo
SPACECREATOR?=spacecreator.AppImage

# Get the list of ASN.1 files from Space Creator project file:
DISTFILES=$(shell qmake %{ProFile} -o /tmp/null 2>&1)
ASN1_FILES=$(shell find ${DISTFILES} 2>/dev/null | egrep '\.asn$$|\.asn1$$')

# Explicit configuration support
ASN1_DRV_CONFIG=%{ProjectName}configuration.asn
ASN1_DRV_PRIVATE=%{ProjectName}privatedata.asn
ASN1_DRIVER_EXPLICIT_CONFIG=$(ASN1_DRV_CONFIG) $(ASN1_DRV_PRIVATE)

SNIPPET_INSTALLATION_DIRECTORY=$(COMMUNICATION_DEVICE_PATH)/%{CDBoard}/port_%{ProjectNameLowerCase}
SNIPPETS_INSTALLATOR=install_communication_devices.py

all:	release

release:	work/glue_release
	rm -rf work/glue_debug
	rm -rf work/glue_coverage
#	$(MAKE) -C work check_targets
	$(MAKE) -C work

debug:	work/glue_debug
	rm -rf work/glue_release
	rm -rf work/glue_coverage
#	$(MAKE) -C work check_targets
	$(MAKE) -C work

run:
	@echo "run Hello World!"

install:
	mkdir -p $(SNIPPET_INSTALLATION_DIRECTORY) && cp -r snippets/. $(SNIPPET_INSTALLATION_DIRECTORY)
	$(SNIPPETS_INSTALLATOR) -c ${COMMUNICATION_DEVICE_PATH}

uninstall:
	rm -r $(SNIPPET_INSTALLATION_DIRECTORY) && $(SNIPPETS_INSTALLATOR) -c ${COMMUNICATION_DEVICE_PATH}

uninstall-all:
	rm -r $(COMMUNICATION_DEVICE_PATH)/*
	$(SNIPPETS_INSTALLATOR) -c ${COMMUNICATION_DEVICE_PATH}

work/glue_release:	InterfaceView.aadl DeploymentView.aadl DataView.aadl
	sed -i 's/CoverageEnabled => true/CoverageEnabled => false/g' DeploymentView.aadl || :
	$(KAZOO) -p --glue --gw -o work --cv_template_dir=communication_device
	touch $@

work/glue_debug:	InterfaceView.aadl DeploymentView.aadl DataView.aadl
	sed -i 's/CoverageEnabled => true/CoverageEnabled => false/g' DeploymentView.aadl || :
	$(KAZOO) --debug -p --glue --gw -o work --cv_template_dir=communication_device
	touch $@

InterfaceView.aadl:	interfaceview.xml
	$(SPACECREATOR) --aadlconverter  -o $^ -t $(shell taste-config --prefix)/share/xml2aadl/interfaceview.tmplt -x $@

%: %.dv.xml Default_Deployment.aadl
	# Build using deployment view $^
	@# We must update the .aadl only if the dv.xml file has changed (more recent timestamp)
	if [ $< -nt $@.aadl ]; then 	$(SPACECREATOR) --dvconverter -o $< -t  $(shell taste-config --prefix)/share/dv2aadl/deploymentview.tmplt -x $@.aadl; fi;
	rsync --checksum $@.aadl DeploymentView.aadl

Default_Deployment.aadl:	interfaceview.xml
	# Create/update a default deployment view for Linux target, if none other is provided
	$(SPACECREATOR) --aadlconverter  -o $^ -t $(shell taste-config --prefix)/share/xml2dv/interfaceview-%{CDBoard}.tmplt -x $@ || exit 1
	rsync --checksum $@ DeploymentView.aadl

DeploymentView.aadl:	Default_Deployment.aadl

DataView.aadl: ${ASN1_FILES} $(ASN1_DRIVER_EXPLICIT_CONFIG)
	taste-update-data-view $^

clean:
	rm -rf work/build work/dataview work/glue_simu
	rm -f *.aadl  # Interface and Deployment views in AADL are generated
	rm -f work/glue_release work/glue_debug work/glue_coverage work/skeletons_built
	find work -type d -name "wrappers" -exec rm -rf {} + || :
	find work -type d -name "*_GUI" -exec rm -rf {} + || :
	find work -type d -path "*/QGenC/xmi" -exec rm -rf {} + || : 
	find work -type d -path "*/QGenC/src/.qgeninfo" -exec rm -rf {} + || : 
	find work -type f -path "*/QGenC/src/*.h" -exec rm -f {} + || : 
	find work -type f -path "*/QGenC/src/*.c" -exec rm -f {} + || : 
