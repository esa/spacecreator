#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_alternate_set.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

cd $TEST_OUTPUT_DIR

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$DIFF interfaceview.xml ../resources/test_alternate_set.output \
  && $DIFF ALTERNATESET.asn ../resources/test_alternate_set_asn.output \
  && $DIFF ALTERNATESET.acn ../resources/test_alternate_set_acn.output \
  && asn1scc -c ALTERNATESET.asn ALTERNATESET.acn \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
