set(TEST_NAME tsti_sedstoasn1converter)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     main.cpp
                testbase.cpp
                tsti_arraydatatype.cpp
                tsti_containerdatatype.cpp
                testbase.h
                tsti_arraydatatype.h
                tsti_containerdatatype.h)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/src/libs)
target_link_libraries(${TEST_NAME}
    PRIVATE     Conversion::Common
                Conversion::Asn1Registrar;
                Conversion::SedsRegistrar;
                Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test
                CommonProjectWarnings)

configure_file(resources/array_onedimension.xml array_onedimension.xml COPYONLY)
configure_file(resources/array_multidimension.xml array_multidimension.xml COPYONLY)
configure_file(resources/container_simple.xml container_simple.xml COPYONLY)
configure_file(resources/container_extended.xml container_extended.xml COPYONLY)

add_format_target(${TEST_NAME})
