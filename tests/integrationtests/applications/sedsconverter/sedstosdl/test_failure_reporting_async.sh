#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_failure_reporting_async.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --seds-transaction-name-type CString \
  --seds-enable-failure-reporting \
  --seds-failure-reporting-type CoolBool \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/

# # Setup additional data
cp resources/test_failure_reporting_async_client.system_structure output/test_failure_reporting_async_client.system_structure
cp resources/test_failure_reporting_async_calculator.system_structure output/test_failure_reporting_async_calculator.system_structure
cp $TEST_OUTPUT_DIR/FAILUREREPORTING.asn $TEST_OUTPUT_DIR/dataview-uniq.asn

# # Rename the module to avoid naming conflicts
sed -i 's/FAILUREREPORTING/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn

cd $TEST_OUTPUT_DIR

# # Compare output against reference, and compile to make sure the reference is valid
# # Clean (rm) only if all steps pass
# # This test uses Ada, as C backend in OpenGEODE is too buggy to handle this example
$DIFF client.pr ../resources/test_failure_reporting_async_client.output \
    && $DIFF calculator.pr ../resources/test_failure_reporting_async_calculator.output \
    && $OPENGEODE --toAda test_failure_reporting_async_client.system_structure client.pr \
    && asn1scc -Ada --type-prefix asn1Scc -equal dataview-uniq.asn client_datamodel.asn \
    && gcc -c client.adb \
    && $OPENGEODE --toAda test_failure_reporting_async_calculator.system_structure calculator.pr \
    && asn1scc -Ada --type-prefix asn1Scc -equal dataview-uniq.asn calculator_datamodel.asn \
    && gcc -c calculator.adb \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
