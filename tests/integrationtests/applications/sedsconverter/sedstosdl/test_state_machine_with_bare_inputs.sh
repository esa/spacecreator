#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
TEST_OUTPUT_DIR=output

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to SDL --skip-validation -i resources/state_machine_with_bare_inputs.xml \
    --sdl-filepath-prefix $TEST_OUTPUT_DIR/
# Compare and clean-up on success
diff $TEST_OUTPUT_DIR/Component.pr resources/state_machine_with_bare_inputs.output && rm -r -f $TEST_OUTPUT_DIR
