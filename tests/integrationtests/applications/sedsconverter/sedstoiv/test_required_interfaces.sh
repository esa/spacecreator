#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f dataview-uniq.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
XMLDIFF="xmldiff --check --ignored-attributes $(cat ../ignored_attributes.txt)"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_required_interfaces.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

cd $TEST_OUTPUT_DIR

# Merge ASN files
sed -e '$s/$/\n/' -s INTERFACES.asn INTERFACES-REQUIRER.asn > dataview-uniq.asn
sed -e '$s/$/\n/' -s INTERFACES.acn INTERFACES-REQUIRER.acn > dataview-uniq.acn

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$XMLDIFF interfaceview.xml ../resources/test_required_interfaces.output \
  && $UPDATE_DATAVIEW \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2dv/interfaceview.tmplt \
  -x DeploymentView.aadl \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2iv/interfaceview.tmplt \
  -x InterfaceView.aadl \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
