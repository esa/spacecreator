set(TEST_NAME tst_simulinkxmlimporter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_testXML(${TEST_NAME})


target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs
                ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                SIMULINK::XmlImporter
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                Qt${QT_VERSION_MAJOR}::Xml
                CommonProjectWarnings)

configure_file(resources/Complex.xml Complex.xml COPYONLY)

configure_file(resources/dimensions/DimensionsEmpty.xml DimensionsEmpty.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVecEmpty.xml DimensionsVecEmpty.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec2Float6p1.xml DimensionsVec2Float6p1.xml COPYONLY)
configure_file(resources/dimensions/DimensionsFloat6p1.xml DimensionsFloat6p1.xml COPYONLY)
configure_file(resources/dimensions/DimensionsZero.xml DimensionsZero.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec20.xml DimensionsVec20.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec22sc22.xml DimensionsVec22sc22.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec23Spaces.xml DimensionsVec23Spaces.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec23456.xml DimensionsVec23456.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec2Var.xml DimensionsVec2Var.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec23.xml DimensionsVec23.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec2sc2sc2.xml DimensionsVec2sc2sc2.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec2sc2.xml DimensionsVec2sc2.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVec2.xml DimensionsVec2.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVecIntMaxIntMax.xml DimensionsVecIntMaxIntMax.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVecIntMax.xml DimensionsVecIntMax.xml COPYONLY)
configure_file(resources/dimensions/DimensionsIntMaxPlusOne.xml DimensionsIntMaxPlusOne.xml COPYONLY)
configure_file(resources/dimensions/DimensionsCell234.xml DimensionsCell234.xml COPYONLY)
configure_file(resources/dimensions/DimensionsMinusOne.xml DimensionsMinusOne.xml COPYONLY)
configure_file(resources/dimensions/DimensionsTwo.xml DimensionsTwo.xml COPYONLY)
configure_file(resources/dimensions/DimensionsIntMax.xml DimensionsIntMax.xml COPYONLY)
configure_file(resources/dimensions/DimensionsVariable.xml DimensionsVariable.xml COPYONLY)

add_format_target(${TEST_NAME})
