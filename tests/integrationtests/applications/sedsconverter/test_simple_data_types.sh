#!/bin/bash


SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_simple_data_types.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Compare and clean-up on success
$DIFF $TEST_OUTPUT_DIR/Com-n7space-SimpleDataTypes.asn resources/test_simple_data_types_asn.output && \
    $DIFF $TEST_OUTPUT_DIR/Com-n7space-SimpleDataTypes.acn resources/test_simple_data_types_acn.output && \
    asn1scc -c -o $TEST_OUTPUT_DIR $TEST_OUTPUT_DIR/Com-n7space-SimpleDataTypes.asn $TEST_OUTPUT_DIR/Com-n7space-SimpleDataTypes.acn && \
    rm -r -f $TEST_OUTPUT_DIR
