# Generating skeletons for QGenC

**1. Steps:**

* Open project `tests/manual/qgenc/projects/basicqgencfunction/basicqgencfunction.pro`
* Open the `interfaceview.xml` file
* Clean the project
* Right click on the interfaceview and select the "Generate/update code skeletons" option

Expected results:

* Several files should be generated including four files in work/qgenc/QGenC/src/
    namely: `qgenc_script.m`, `setInputsBusCreator.m`, `setOutputsBusCCreator.m`, `Simulink_DataView_asn.m`
