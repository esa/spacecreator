#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
# gdb --args \
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_length_entry.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
    --patcher-functions-filepath-prefix $TEST_OUTPUT_DIR/ --mapping-functions-module-filename lengthEntryPostEncoding

cd $TEST_OUTPUT_DIR

cp ../resources/test_length_entry_main.c .

# Compare and clean-up on success
$DIFF COM-N7SPACE-LENGTHENTRY.asn ../resources/test_length_entry_asn.output \
    && $DIFF COM-N7SPACE-LENGTHENTRY.acn ../resources/test_length_entry_acn.output \
    && asn1scc -c -ACN -mfm lengthEntryPostEncoding COM-N7SPACE-LENGTHENTRY.asn COM-N7SPACE-LENGTHENTRY.acn \
    && gcc -o lengthentry \
        test_length_entry_main.c \
        COM-N7SPACE-LENGTHENTRY.c \
        com-n7space-lengthentry-postencoding.c \
        asn1crt.c \
        asn1crt_encoding_acn.c \
        asn1crt_encoding.c \
        asn1crt_encoding_uper.c \
    && rm -r -f $TEST_OUTPUT_DIR
