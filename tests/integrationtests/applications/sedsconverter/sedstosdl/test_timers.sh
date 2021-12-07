#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f output.asn DataView.aadl -aadlv2"
OPENGEODE=$HOME/.local/bin/opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_timers.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml \
  --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp resources/test_timers.system_structure output/system_structure.pr
cp $TEST_OUTPUT_DIR/TIMERS.asn $TEST_OUTPUT_DIR/dataview-uniq.asn
# Rename the module to avoid naming conflicts
sed -i 's/TIMERS/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR
# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
$DIFF Component.pr ../resources/test_timers.output \
  && $DIFF interfaceview.xml ../resources/test_timers.interfaceview \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2iv/interfaceview.tmplt \
  -x InterfaceView.aadl \
  && $OPENGEODE --toAda system_structure.pr Component.pr \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn component_datamodel.asn \
  && gcc -c component.adb \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
