#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_datatype_inside_component.xml \
    --out $TEST_OUTPUT_DIR/test_datatype_inside_component.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Compare and clean-up on success
$DIFF $TEST_OUTPUT_DIR/PKG.asn resources/test_datatype_inside_component_asn1.output && \
    $DIFF $TEST_OUTPUT_DIR/PKG.acn resources/test_datatype_inside_component_acn1.output && \
    asn1scc -c -o $TEST_OUTPUT_DIR $TEST_OUTPUT_DIR/PKG.asn $TEST_OUTPUT_DIR/PKG.acn && \
    $DIFF $TEST_OUTPUT_DIR/PKG-CONTAINER.asn resources/test_datatype_inside_component_asn2.output && \
    $DIFF $TEST_OUTPUT_DIR/PKG-CONTAINER.acn resources/test_datatype_inside_component_acn2.output && \
    asn1scc -c -o $TEST_OUTPUT_DIR $TEST_OUTPUT_DIR/PKG-CONTAINER.asn $TEST_OUTPUT_DIR/PKG-CONTAINER.acn && \
    rm -r -f $TEST_OUTPUT_DIR
