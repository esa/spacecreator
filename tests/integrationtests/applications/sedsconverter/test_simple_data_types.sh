#!/bin/bash


SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
TEST_OUTPUT_DIR=output

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_simple_data_types.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/
# Compare and clean-up on success
diff $TEST_OUTPUT_DIR/Component.pr resources/state_machine_with_bare_inputs.output && rm -r -f $TEST_OUTPUT_DIR
