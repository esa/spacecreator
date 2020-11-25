# Downloading asn1scc to distribute with the asn1 plugin

set(ASNSCC_DIR ${CMAKE_BINARY_DIR}/asn1scc_bin)

execute_process(
    COMMAND "${CMAKE_SOURCE_DIR}/scripts/asn1_compiler_dist.sh" "${ASNSCC_DIR}"
)

# Installing the asn compiler

set(IDE_LIBEXEC_DIR "lib/x86_64-linux-gnu/qtcreator/libexec")
install(DIRECTORY "${ASNSCC_DIR}/libexec/qtcreator/asn1scc" DESTINATION "${IDE_LIBEXEC_DIR}" USE_SOURCE_PERMISSIONS)
install(DIRECTORY "${ASNSCC_DIR}/libexec/qtcreator/asn1scc-Fuzzer" DESTINATION "${IDE_LIBEXEC_DIR}" USE_SOURCE_PERMISSIONS)
install(DIRECTORY "${ASNSCC_DIR}/share/qtcreator/asn1acn" DESTINATION "${IDE_DATA_PATH}" USE_SOURCE_PERMISSIONS)
