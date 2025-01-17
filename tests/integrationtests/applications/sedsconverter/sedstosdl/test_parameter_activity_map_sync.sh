#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_parameter_activity_map_sync.xml \
  --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/ --out $TEST_OUTPUT_DIR/interfaceview.xml

# Setup additional data
cp resources/test_parameteractivitymapsync_receiver.system_structure output/receiver_system_structure.pr
cp resources/test_parameteractivitymapsync_converter.system_structure output/converter_system_structure.pr
cp resources/test_parameteractivitymapsync_thermometer.system_structure output/thermometer_system_structure.pr
cp resources/test_parametermaphostrequirer.system_structure output/requirer_system_structure.pr
cp $TEST_OUTPUT_DIR/PARAMETERS.asn $TEST_OUTPUT_DIR/dataview-uniq.asn

# # Rename the module to avoid naming conflicts
sed -i 's/PARAMETERS/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR

# # Compare output against reference, and compile to make sure the reference is valid
# # Clean (rm) only if all steps pass
$DIFF receiver.pr ../resources/test_parameteractivitymapsync_receiver.output \
  && $DIFF converter.pr ../resources/test_parameteractivitymapsync_converter.output \
  && $DIFF thermometer.pr ../resources/test_parameteractivitymapsync_thermometer.output \
  && $OPENGEODE --toAda receiver_system_structure.pr receiver.pr \
  && $OPENGEODE --toAda converter_system_structure.pr converter.pr \
  && $OPENGEODE --toAda thermometer_system_structure.pr thermometer.pr \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn receiver_datamodel.asn \
  && gcc -c receiver.adb \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn converter_datamodel.asn \
  && gcc -c converter.adb \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn thermometer_datamodel.asn \
  && gcc -c thermometer.adb \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
