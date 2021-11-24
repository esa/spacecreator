#!/bin/bash

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f output.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR
cp resources/Makefile $TEST_OUTPUT_DIR/Makefile
# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_required_interfaces.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp $TEST_OUTPUT_DIR/INTERFACES.asn $TEST_OUTPUT_DIR/output.asn
# Remove output ACN to avoid conflicts between the existing one and the ASN.1 file
# This cannot be replaced due to a reported bug in SEDS -> ASN.1 conversion
rm -f $TEST_OUTPUT_DIR/output.acn
# Rename the module to avoid naming conflicts
sed -i 's/INTERFACES/OUTPUT-DATAVIEW/g' $TEST_OUTPUT_DIR/output.asn
# Change implementation language to C (SDL is not compatible with all interface types)
sed -i 's/language="SDL"/language="C"/g' $TEST_OUTPUT_DIR/interfaceview.xml

cd $TEST_OUTPUT_DIR

$UPDATE_DATAVIEW
# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts

$DIFF interfaceview.xml ../resources/test_required_interfaces.output \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t /home/taste/tool-inst/share/xml2dv/interfaceview.tmplt \
  -x DeploymentView.aadl \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t /home/taste/tool-inst/share/xml2aadl/interfaceview.tmplt \
  -x InterfaceView.aadl \
  && make skeletons \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
