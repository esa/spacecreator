#!/bin/bash
# Adapted from https://github.com/n7space/asn1scc.IDE/blob/master/travis/dist.sh

DIST_DIR=$1
if [[ -d "$DIST_DIR" ]]
then
    exit 0
fi

echo "Installing ASN.1 in $DIST_DIR"

FUZZER_VERSION=0.9
PUSC_VERSION=1.1.0
DOWNLOAD_DIR=/tmp


echo "Copy lib to destination $DIST_DIR"
RESOURCE_DIST_DIR=${DIST_DIR}/share/qtcreator
LIBEXEC_DIST_DIR=${DIST_DIR}/libexec/qtcreator

echo "Downloading asn1scc to distribute with plugin"
wget -O ${DOWNLOAD_DIR}/asn1scc.7z "https://n7space-my.sharepoint.com/:u:/p/kgrochowski/EZSwzkHV-C1Mn1nppkUqKIcBjFpzsn67G_N_1RMW2Yr7dQ?download=1"
mkdir -p ${LIBEXEC_DIST_DIR}
7zr x -o${LIBEXEC_DIST_DIR} ${DOWNLOAD_DIR}/asn1scc.7z
rm ${DOWNLOAD_DIR}/asn1scc.7z
chmod ugo+x ${LIBEXEC_DIST_DIR}/asn1scc/daemon/asn1.daemon.exe

echo "Downloading asn1scc.Fuzzer to distribute with plugin"
wget -O ${DOWNLOAD_DIR}/fuzzer.7z "https://github.com/n7space/asn1scc.Fuzzer/releases/download/${FUZZER_VERSION}/asn1scc-Fuzzer-${FUZZER_VERSION}-linux-x64.tar.gz"
mkdir -p ${LIBEXEC_DIST_DIR}
cd ${LIBEXEC_DIST_DIR}
tar -xvf ${DOWNLOAD_DIR}/fuzzer.7z
rm ${DOWNLOAD_DIR}/fuzzer.7z

echo "Downloading PUS-C lib to distribute with plugin"
wget -O ${DOWNLOAD_DIR}/pusc-lib.7z "https://github.com/n7space/asn1-pusc-lib/releases/download/${PUSC_VERSION}/Asn1Acn-PusC-Library-${PUSC_VERSION}.7z"
PUSC_LIB_DIR=${RESOURCE_DIST_DIR}/asn1acn/libs/PUS-C
mkdir -p ${PUSC_LIB_DIR}
7zr x -o${PUSC_LIB_DIR} ${DOWNLOAD_DIR}/pusc-lib.7z
rm ${DOWNLOAD_DIR}/pusc-lib.7z
