#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f dataview-uniq.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
XMLDIFF="xmldiff --ignored-attrs $(cat ../ignored_attributes.txt)"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_cross_reference_arguments.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

cd $TEST_OUTPUT_DIR

# Setup additional data
mv COM-N7SPACE-PACKAGE1.asn package1.asn
mv COM-N7SPACE-PACKAGE1.acn package1.acn
mv COM-N7SPACE-PACKAGE1-MEGACOMPONENT.asn package1-component.asn
mv COM-N7SPACE-PACKAGE1-MEGACOMPONENT.acn package1-component.acn
mv COM-N7SPACE-PACKAGE2.asn package2.asn
mv COM-N7SPACE-PACKAGE2.acn package2.acn

# Merge ASN files
sed -e '$s/$/\n/' -s package1.asn package1-component.asn package2.asn > dataview-uniq.asn
sed -e '$s/$/\n/' -s package1.acn package1-component.acn package2.acn > dataview-uniq.acn

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$XMLDIFF interfaceview.xml ../resources/test_cross_reference_arguments.output \
  && $DIFF package1.asn ../resources/test_cross_reference_arguments_package1_asn.output \
  && $DIFF package1.acn ../resources/test_cross_reference_arguments_package1_acn.output \
  && $DIFF package1-component.asn ../resources/test_cross_reference_arguments_package1_component_asn.output \
  && $DIFF package1-component.acn ../resources/test_cross_reference_arguments_package1_component_acn.output \
  && $DIFF package2.asn ../resources/test_cross_reference_arguments_package2_asn.output \
  && $DIFF package2.acn ../resources/test_cross_reference_arguments_package2_acn.output \
  && $UPDATE_DATAVIEW \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2dv/interfaceview.tmplt \
  -x DeploymentView.aadl \
  && $AADL_CONVERTER -o interfaceview.xml \
  -t ../resources/xml2iv/interfaceview.tmplt \
  -x InterfaceView.aadl \
  && cd .. \
  && rm -rf $TEST_OUTPUT_DIR
