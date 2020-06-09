set(LIB_NAME msclibrary)

# Generate ANTL parser files
set(PARSER_FILE ${CMAKE_CURRENT_SOURCE_DIR}/parser/Msc.g4)
set(ANTLR_JAR_LOCATION ${CMAKE_SOURCE_DIR}/src/3rdparty/antlr/antlr-4.8-complete.jar)

# ANTLR output files
set(ANTLR_SRC
    ${CMAKE_CURRENT_BINARY_DIR}/MscBaseVisitor.h
    ${CMAKE_CURRENT_BINARY_DIR}/MscLexer.h
    ${CMAKE_CURRENT_BINARY_DIR}/MscParser.h
    ${CMAKE_CURRENT_BINARY_DIR}/MscVisitor.h
    ${CMAKE_CURRENT_BINARY_DIR}/MscBaseVisitor.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/MscLexer.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/MscParser.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/MscVisitor.cpp
)

foreach(src_file ${ANTLR_SRC})
    set_source_files_properties(${src_file} PROPERTIES GENERATED TRUE)
endforeach()

find_package(Java COMPONENTS Runtime REQUIRED)
cmake_policy(SET CMP0071 OLD) # This makes AUTO_MOC skip the generated files
add_custom_command(
    OUTPUT ${ANTLR_SRC}
    COMMAND "${Java_JAVA_EXECUTABLE}" -jar "${ANTLR_JAR_LOCATION}" -Dlanguage=Cpp -no-listener -visitor ${PARSER_FILE} -o "${CMAKE_CURRENT_BINARY_DIR}"
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
    DEPENDS ${PARSER_FILE}
)

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
    datastatement.cpp
    datastatement.h
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
    mscgate.cpp
    mscgate.h
    mscinstance.cpp
    mscinstance.h
    mscinstanceevent.cpp
    mscinstanceevent.h
    msclibrary.cpp
    msclibrary.h
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
    mscreader.cpp
    mscreader.h
    mscresources.qrc
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
    PRIVATE cif
    PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
)

target_link_libraries(${LIB_NAME}
    asn1editor
    qobjectlistmodel
    templating
    Qt5::Core
    Qt5::Gui
    antlr4_static
)

target_compile_definitions(${LIB_NAME} PUBLIC ANTLR4CPP_STATIC)

source_group("Designer Files" REGULAR_EXPRESSION ".*.ui$")
source_group("Resource Files" REGULAR_EXPRESSION ".*.qrc$")
