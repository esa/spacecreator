#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=$HOME/.local/bin/opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_transaction

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_transaction.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --seds-transaction-name-type CString \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/

# Setup additional data
cp resources/test_transaction_pinger.system_structure output/test_transaction_pinger.system_structure
cp resources/test_transaction_ponger.system_structure output/test_transaction_ponger.system_structure
cp $TEST_OUTPUT_DIR/TRANSACTION.asn $TEST_OUTPUT_DIR/dataview-uniq.asn

# Rename the module to avoid naming conflicts
sed -i 's/TRANSACTION/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn

cd $TEST_OUTPUT_DIR

# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
# This test uses Ada, as C backend in OpenGEODE is too buggy to handle this example
$DIFF pinger.pr ../resources/test_transaction_pinger.output \
    && $DIFF ponger.pr ../resources/test_transaction_ponger.output \
    && $OPENGEODE --toAda test_transaction_pinger.system_structure pinger.pr \
    && asn1scc -Ada --type-prefix asn1Scc -equal dataview-uniq.asn pinger_datamodel.asn \
    && gcc -c pinger.adb \
    && $OPENGEODE --toAda test_transaction_ponger.system_structure ponger.pr \
    && asn1scc -Ada --type-prefix asn1Scc -equal dataview-uniq.asn ponger_datamodel.asn \
    && gcc -c ponger.adb \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
