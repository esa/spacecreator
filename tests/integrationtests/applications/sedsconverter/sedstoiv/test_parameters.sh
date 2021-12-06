#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f output.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_parameters.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp $TEST_OUTPUT_DIR/PARAMETERS.asn $TEST_OUTPUT_DIR/output.asn
# Remove output ACN to avoid conflicts between the existing one and the ASN.1 file
# This cannot be replaced due to a reported bug in SEDS -> ASN.1 conversion
rm -f $TEST_OUTPUT_DIR/output.acn

cd $TEST_OUTPUT_DIR

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$DIFF interfaceview.xml ../resources/test_parameters.output \
  && $UPDATE_DATAVIEW \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2dv/interfaceview.tmplt \
  -x DeploymentView.aadl \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2iv/interfaceview.tmplt \
  -x InterfaceView.aadl \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
