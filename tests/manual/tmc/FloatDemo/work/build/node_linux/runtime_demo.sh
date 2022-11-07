#!/bin/sh

GENERIC_LINUX_RUNTIME_COMMON_PATH="${HOME}/tool-inst/include/TASTE-Runtime-Common"
GENERIC_LINUX_RUNTIME_PATH="${HOME}/tool-inst/include/TASTE-Linux-Runtime"
GENERIC_LINUX_DRIVERS_PATH="${HOME}/tool-inst/include/TASTE-Linux-Drivers"

echo "Copying GENERIC_LINUX runtime"
mkdir -p "demo/runtime/"
cp -r "${GENERIC_LINUX_RUNTIME_PATH}"/src/* "demo/runtime/"

echo "Copying TASTE common runtime"
cp -r "${GENERIC_LINUX_RUNTIME_COMMON_PATH}"/src/Packetizer "demo/runtime/"
cp -r "${GENERIC_LINUX_RUNTIME_COMMON_PATH}"/src/Broker "demo/runtime/"
cp -r "${GENERIC_LINUX_RUNTIME_COMMON_PATH}"/src/RuntimeCommon "demo/runtime/"
cp -r "${GENERIC_LINUX_RUNTIME_COMMON_PATH}"/src/Escaper "demo/runtime/"

echo "Copying source code of device drivers"
mkdir -p "demo/drivers/"

