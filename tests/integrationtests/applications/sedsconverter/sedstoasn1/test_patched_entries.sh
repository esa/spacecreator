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
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_patched_entries.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
    --patcher-functions-filepath-prefix $TEST_OUTPUT_DIR/ --mapping-functions-module-filename patchedEntriesPostEncoding

cd $TEST_OUTPUT_DIR

cp ../resources/test_patched_entries_main.c .

# Compare and clean-up on success
$DIFF COM-N7SPACE-PATCHED.asn ../resources/test_patched_entries_asn.output \
    && $DIFF COM-N7SPACE-PATCHED.acn ../resources/test_patched_entries_acn.output \
    && asn1scc -c -ACN -mfm patchedEntriesPostEncoding COM-N7SPACE-PATCHED.asn COM-N7SPACE-PATCHED.acn \
    && gcc -o patchedentries \
        test_patched_entries_main.c \
        COM-N7SPACE-PATCHED.c \
        patchingCommonLibrary.c \
        com-n7space-patched-postencoding.c \
        asn1crt.c \
        asn1crt_encoding_acn.c \
        asn1crt_encoding.c \
        asn1crt_encoding_uper.c \
    && ./patchedentries \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
