#!/bin/bash

PROJECT_NAME=$(xmllint --xpath "/*/@projectName" interfaceview.xml | sed -e "s/projectName=//g" | sed -e "s/\\"//g")
PARTITION_NAME=$(echo ${PROJECT_NAME,,})

CD_INSTALLATION_DIRECTORY=$COMMUNICATION_DEVICE_PATH/%{CDBoard}/port_%{ProjectNameLowerCase}
FUNCTION_NAMES=$(xmllint --xpath "//*[local-name()='Function']/@name" interfaceview.xml)

mkdir -p $CD_INSTALLATION_DIRECTORY
cp -r snippets/. $CD_INSTALLATION_DIRECTORY
cp %{ProjectName}configuration.asn $CD_INSTALLATION_DIRECTORY
cp %{ProjectName}configuration.acn $CD_INSTALLATION_DIRECTORY
mkdir -p $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}

ASN1_TYPES_H_FILES=$(find work -name "C_ASN1_Types.h" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*' | head -n 1)
ASN1_TYPES_C_FILES=$(find work -name "C_ASN1_Types.c" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*' | head -n 1)
H_FILES=$(find work -name "*.h" ! -name "*C_ASN1_Types.h" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*')
C_FILES=$(find work -name "*.c" ! -name "*C_ASN1_Types.c" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*')
CC_FILES=$(find work -name "*.cc" ! -name "*C_ASN1_Types.c" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*')
ADS_FILES=$(find work -name "*.ads" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*')
ADB_FILES=$(find work -name "*.adb" -not -path 'work/build/*' -a -not -path 'work/dataview/*' -a -not -path 'work/broker/*' -a -not -path 'work/packetizer/*' -a -not -path 'work/binaries/*' -a -not -path 'work/Debug/*' -a -not -path 'work/Dump/*')
DRIVER_INTERFACE_FILES=$(find work/build/**/${PARTITION_NAME} -name "${PARTITION_NAME}.*")
THREAD_FILES=$(find work/build/**/${PARTITION_NAME} -name "thread_*" -a -not -name "thread_broker_receive.*" -a -not -name "thread_packetizer_trigger.*")
INTERFACE_FILES=$(find work/build/**/${PARTITION_NAME} -name "${PARTITION_NAME}_interface.h" -o -name "${PARTITION_NAME}_shared_interface.cc")
TRANSPORT_FILES=$(find work/build/**/${PARTITION_NAME} -name "driver_transport.*")
ROUTING_FILES=$(find work/build/**/${PARTITION_NAME} -name "routing.*")
DATA_VIEW_FILES=$(find work/dataview/C -name "dataview-uniq.*")
REQUEST_SIZE_FILE=work/build/**/${PARTITION_NAME}/request_size.h
SYSTEM_CONFIG_FILE=work/build/system_config.h

ALL_SOURCE_FILES=( "${ASN1_TYPES_H_FILES} ${ASN1_TYPES_C_FILES} ${H_FILES} ${C_FILES} ${CC_FILES} ${ADS_FILES} \\
${ADB_FILES} ${DRIVER_INTERFACE_FILES} ${THREAD_FILES} ${INTERFACE_FILES} ${TRANSPORT_FILES} ${ROUTING_FILES} \\
${DATA_VIEW_FILES} ${REQUEST_SIZE_FILE} ${SYSTEM_CONFIG_FILE}" )

for SOURCE_FILE_NAME in $ALL_SOURCE_FILES; do
    cp ${SOURCE_FILE_NAME} $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/
done

find $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase} -type f | xargs sed -i "s/asn1Scc/${PARTITION_NAME}_asn1Scc/g"
find $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase} -type f | xargs sed -i "s/request_size.h/${PARTITION_NAME}_request_size.h/g"
find $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase} -type f | xargs sed -i "s/routing.h/${PARTITION_NAME}_routing.h/g"
sed -i "s/GENERIC_PARTITION_BUFFER_SIZE/${PARTITION_NAME}_GENERIC_PARTITION_BUFFER_SIZE/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/request_size.h
sed -i "s/..\\/..\\/system_config.h/system_config.h/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/C_ASN1_Types.h
sed -i "s/${PARTITION_NAME}_asn1SccUint/asn1SccUint/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.h
sed -i "s/${PARTITION_NAME}_asn1SccUint/asn1SccUint/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.c
sed -i "s/${PARTITION_NAME}_asn1SccSint/asn1SccSint/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.h
sed -i "s/${PARTITION_NAME}_asn1SccSint/asn1SccSint/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.c
sed -i "/^} ${PARTITION_NAME}_asn1Scc%{ProjectNameUpperCase}_Private_Data;.*/a typedef ${PARTITION_NAME}_asn1Scc%{ProjectNameUpperCase}_Private_Data %{ProjectNameLowerCase}_private_data;" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.h
sed -i "s/GENERATED_ASN1SCC/GENERATED_${PARTITION_NAME}_ASN1SCC/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.h
sed -i "s/put(sender_pid,/put((asn1SccPID)0,/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/driver_transport.cc
sed -i '/^#include "dataview-uniq.h".*/a #include <drivers_config.h>' $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/%{ProjectNameLowerCase}.h

for name in $FUNCTION_NAMES; do
    FUNCTION_PID=$(echo ${name,,} | sed -e "s/name=/PID_/g" | sed -e "s/\\"//g")
    find $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase} -type f | xargs sed -i "s/${FUNCTION_PID}/${PARTITION_NAME}_${FUNCTION_PID}/g"
done

find $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase} -type f | xargs sed -i "s/PID_env/${PARTITION_NAME}_PID_env/g"

sed -i "s/ERR_/${PARTITION_NAME}_ERR_/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.h
sed -i "s/ERR_/${PARTITION_NAME}_ERR_/g" $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}/dataview-uniq.c

pushd $CD_INSTALLATION_DIRECTORY/%{ProjectNameLowerCase}
mv request_size.h "${PARTITION_NAME}_request_size.h"
mv routing.h "${PARTITION_NAME}_routing.h"
mv routing.c "${PARTITION_NAME}_routing.c"
popd