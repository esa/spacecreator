#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
XMLDIFF="xmldiff --check --ignored-attrs $(cat ../ignored_attributes.txt)"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_generic_type_mapping.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

cd $TEST_OUTPUT_DIR

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$XMLDIFF interfaceview.xml ../resources/test_generic_type_mapping.output \
  && $DIFF TYPEMAPPING.asn ../resources/test_generic_type_mapping_asn.output \
  && $DIFF TYPEMAPPING.acn ../resources/test_generic_type_mapping_acn.output \
  && $DIFF TYPEMAPPING-MEGACOMPONENT.asn ../resources/test_generic_type_mapping_component_asn.output \
  && $DIFF TYPEMAPPING-MEGACOMPONENT.acn ../resources/test_generic_type_mapping_component_acn.output \
  && asn1scc -c TYPEMAPPING.asn TYPEMAPPING.acn TYPEMAPPING-MEGACOMPONENT.asn TYPEMAPPING-MEGACOMPONENT.acn\
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
