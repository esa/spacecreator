#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=$HOME/.local/bin/opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_parameter_map

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_parameter_map.xml \
  --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/ --out $TEST_OUTPUT_DIR/interfaceview.xml
# Setup additional data
cp resources/test_parametermaphostprovider.system_structure output/provider_system_structure.pr
cp resources/test_parametermaphostrequirer.system_structure output/requirer_system_structure.pr
cp $TEST_OUTPUT_DIR/PARAMETERS.asn $TEST_OUTPUT_DIR/dataview-uniq.asn
# Rename the module to avoid naming conflicts
sed -i 's/PARAMETERS/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR
# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
$DIFF parametermaphostprovider.pr ../resources/test_parametermaphostprovider.output \
  && $DIFF parametermaphostrequirer.pr ../resources/test_parametermaphostrequirer.output \
  && $OPENGEODE --toAda provider_system_structure.pr parametermaphostprovider.pr \
  && $OPENGEODE --toAda requirer_system_structure.pr parametermaphostrequirer.pr \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn parametermaphostprovider_datamodel.asn \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn parametermaphostrequirer_datamodel.asn \
  && gcc -c parametermaphostprovider.adb \
  && gcc -c parametermaphostrequirer.adb \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
