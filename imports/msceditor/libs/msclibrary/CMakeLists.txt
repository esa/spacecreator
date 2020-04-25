set(LIB_NAME msclibrary)


# Generate ANTL parser files
# Check for better udage:
# https://github.com/antlr/antlr4/blob/master/runtime/Cpp/cmake/FindANTLR.cmake
# https://github.com/antlr/antlr4/tree/master/runtime/Cpp/cmake
set(ANTLR_CPPRT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/antlr/cpp_runtime)
set(ANTLR_RUNTIME_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../3rdparty/antlr/cpp_runtime/runtime)
if (WIN32)
    set(ANTLR_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/create_msc_parser.bat)
    set(ANTL_LIB antlr4-runtime)
else()
    set(ANTLR_COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/../../scripts/create_msc_parser.sh)
    set(ANTL_LIB antlr4-runtime_linux_x64)
    # for MacOS: antlr4-runtime_macos_x64
endif()

execute_process(COMMAND ${ANTLR_COMMAND}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

# cmake caches are problematic, so list the files in a variable
set(ANTLR_SRC
    parser/MscBaseVisitor.h
    parser/MscLexer.h
    parser/MscParser.h
    parser/MscVisitor.h
    parser/MscBaseVisitor.cpp
    parser/MscLexer.cpp
    parser/MscParser.cpp
    parser/MscVisitor.cpp
)
# End of generate ANTL parser files


add_library(${LIB_NAME} STATIC)

target_sources(${LIB_NAME} PRIVATE
    ${ANTLR_SRC}
    cif/cifblock.cpp
    cif/cifblock.h
    cif/cifblockfactory.cpp
    cif/cifblockfactory.h
    cif/cifblocks.cpp
    cif/cifblocks.h
    cif/cifline.cpp
    cif/cifline.h
    cif/ciflines.cpp
    cif/ciflines.h
    cif/cifparser.cpp
    cif/cifparser.h
    exceptions.cpp
    exceptions.h
    mscaction.cpp
    mscaction.h
    mscchart.cpp
    mscchart.h
    msccomment.cpp
    msccomment.h
    msccondition.cpp
    msccondition.h
    msccoregion.cpp
    msccoregion.h
    msccreate.cpp
    msccreate.h
    mscdocument.cpp
    mscdocument.h
    mscentity.cpp
    mscentity.h
    mscerrorlistener.cpp
    mscerrorlistener.h
    mscfile.cpp
    mscfile.h
    mscgate.cpp
    mscgate.h
    mscinstance.cpp
    mscinstance.h
    mscinstanceevent.cpp
    mscinstanceevent.h
    mscmessage.cpp
    mscmessage.h
    mscmessagedeclaration.cpp
    mscmessagedeclaration.h
    mscmessagedeclarationlist.cpp
    mscmessagedeclarationlist.h
    mscmodel.cpp
    mscmodel.h
    mscparameterlist.cpp
    mscparameterlist.h
    mscparservisitor.cpp
    mscparservisitor.h
    msctimer.cpp
    msctimer.h
    mscwriter.cpp
    mscwriter.h
    parser/Msc.g4
    parserdebughelper_p.cpp
    parserdebughelper_p.h
)

target_include_directories(${LIB_NAME}
    PUBLIC .
    private cif
    SYSTEM PRIVATE ${ANTLR_RUNTIME_PATH}/src
)

target_link_directories(${LIB_NAME}
    PUBLIC ${ANTLR_CPPRT_PATH}/dist
)

message("${ANTLR_CPPRT_PATH}/dist")

target_link_libraries(${LIB_NAME}
    asn1editor
    qobjectlistmodel
    Qt5::Core
    Qt5::Gui
    ${ANTL_LIB}
)

target_compile_definitions(${LIB_NAME} PUBLIC QT_NO_KEYWORDS)
