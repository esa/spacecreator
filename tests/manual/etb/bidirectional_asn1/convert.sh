#!/bin/bash

set -e

mkdir -p output
rm -rf output/*

sedsconverter --from ASN.1 --to SEDS \
    -i bidirectional_enc.asn,bidirectional_enc.acn \
    --out bidirectional.xml
sedsconverter --from SEDS --to ASN.1 --skip-validation \
    -i bidirectional.xml \
    --asn1-filepath-prefix output/ --acn-filepath-prefix output/

pushd output > /dev/null

mv BIDIRECTIONAL.asn bidirectional_dec.asn
mv BIDIRECTIONAL.acn bidirectional_dec.acn

sed -i "s/Enc/Dec/g" bidirectional_dec.asn
sed -i "s/Enc/Dec/g" bidirectional_dec.acn

popd > /dev/null

mkdir -p encoder
rm -rf encoder/*
asn1scc -c -ACN -equal -o encoder bidirectional_enc.asn bidirectional_enc.acn

mkdir -p decoder
rm -rf decoder/*
asn1scc -c -ACN -equal -o decoder output/bidirectional_dec.asn output/bidirectional_dec.acn

cp encoder/asn1crt* test
cp encoder/bidirectional_enc.* test
cp decoder/bidirectional_dec.* test

mkdir -p test/build

pushd test/build > /dev/null

cmake -GNinja ..
ninja
./bidirectional

popd > /dev/null
