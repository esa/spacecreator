set(TEST_NAME tst_tmc_observers)
project(${TEST_NAME})

add_executable(${TEST_NAME})
add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})

target_sources(${TEST_NAME}
    PRIVATE     ${TEST_NAME}.cpp)
target_include_directories(${TEST_NAME}
    PRIVATE     ${CMAKE_SOURCE_DIR}/tests)
target_link_libraries(${TEST_NAME}
    PRIVATE     Qt${QT_VERSION_MAJOR}::Core
                Qt${QT_VERSION_MAJOR}::Test)

add_format_target(${TEST_NAME})

configure_file(test_input_monitoring.sh test_input_monitoring.sh COPYONLY)
configure_file(resources/InputMonitoring/interfaceview.xml resources/InputMonitoring/interfaceview.xml COPYONLY)
configure_file(resources/InputMonitoring/work/simulation/observer.asn resources/InputMonitoring/work/simulation/observer.asn COPYONLY)
configure_file(resources/InputMonitoring/work/model/SDL/src/system_structure.pr resources/InputMonitoring/work/model/SDL/src/system_structure.pr COPYONLY)
configure_file(resources/InputMonitoring/work/model/SDL/src/model.pr resources/InputMonitoring/work/model/SDL/src/model.pr COPYONLY)
configure_file(resources/InputMonitoring/work/model/SDL/src/dataview-uniq.asn resources/InputMonitoring/work/model/SDL/src/dataview-uniq.asn COPYONLY)
configure_file(resources/InputMonitoring/work/model/SDL/code/model_datamodel.asn resources/InputMonitoring/work/model/SDL/code/model_datamodel.asn COPYONLY)
configure_file(resources/InputMonitoring/work/modelchecking/properties/InputObserver/system_structure.pr resources/InputMonitoring/work/modelchecking/properties/InputObserver/system_structure.pr COPYONLY)
configure_file(resources/InputMonitoring/work/modelchecking/properties/InputObserver/InputObserver.pr resources/InputMonitoring/work/modelchecking/properties/InputObserver/InputObserver.pr COPYONLY)
configure_file(resources/InputMonitoring/work/modelchecking/properties/InputObserver/dataview-uniq.asn resources/InputMonitoring/work/modelchecking/properties/InputObserver/dataview-uniq.asn COPYONLY)
configure_file(resources/InputMonitoring/work/modelchecking/properties/InputObserver/inputobserver_datamodel.asn resources/InputMonitoring/work/modelchecking/properties/InputObserver/inputobserver_datamodel.asn COPYONLY)
configure_file(resources/InputMonitoring/work/modelchecking/properties/StopConditions/InputObserverFinished.scl resources/InputMonitoring/work/modelchecking/properties/StopConditions/InputObserverFinished.scl COPYONLY)

configure_file(test_input_monitoring_failure.sh test_input_monitoring_failure.sh COPYONLY)
configure_file(resources/InputMonitoringFailure/interfaceview.xml resources/InputMonitoringFailure/interfaceview.xml COPYONLY)
configure_file(resources/InputMonitoringFailure/work/simulation/observer.asn resources/InputMonitoringFailure/work/simulation/observer.asn COPYONLY)
configure_file(resources/InputMonitoringFailure/work/model/SDL/src/system_structure.pr resources/InputMonitoringFailure/work/model/SDL/src/system_structure.pr COPYONLY)
configure_file(resources/InputMonitoringFailure/work/model/SDL/src/model.pr resources/InputMonitoringFailure/work/model/SDL/src/model.pr COPYONLY)
configure_file(resources/InputMonitoringFailure/work/model/SDL/src/dataview-uniq.asn resources/InputMonitoringFailure/work/model/SDL/src/dataview-uniq.asn COPYONLY)
configure_file(resources/InputMonitoringFailure/work/model/SDL/code/model_datamodel.asn resources/InputMonitoringFailure/work/model/SDL/code/model_datamodel.asn COPYONLY)
configure_file(resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/system_structure.pr resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/system_structure.pr COPYONLY)
configure_file(resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/InputObserver.pr resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/InputObserver.pr COPYONLY)
configure_file(resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/dataview-uniq.asn resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/dataview-uniq.asn COPYONLY)
configure_file(resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/inputobserver_datamodel.asn resources/InputMonitoringFailure/work/modelchecking/properties/InputObserver/inputobserver_datamodel.asn COPYONLY)
configure_file(resources/InputMonitoringFailure/work/modelchecking/properties/StopConditions/InputObserverFinished.scl resources/InputMonitoringFailure/work/modelchecking/properties/StopConditions/InputObserverFinished.scl COPYONLY)

