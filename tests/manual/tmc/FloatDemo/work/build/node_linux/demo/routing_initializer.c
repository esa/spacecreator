/* File generated by kazoo for TASTE - do not edit */

#include "routing.h"

void initialize_routing() {
    
    
    
    
    
    // Initialize routing table of function ControlSystem
    
    controlsystem_controlsignal_set_all_routes_enabled(true);
    controlsystem_tm_set_all_routes_enabled(true);
    
    
    
    // Initialize routing table of function demo_Timer_Manager
    
    demo_timer_manager_sensor_trigger_set_all_routes_enabled(true);
    
    
    
    // Initialize routing table of function Host
    
    host_configuration_set_all_routes_enabled(true);
    host_tc_set_all_routes_enabled(true);
    
    
    
    // Initialize routing table of function Sensor
    
    sensor_reset_trigger_set_all_routes_enabled(true);
    sensor_set_trigger_set_all_routes_enabled(true);
    sensor_reading_set_all_routes_enabled(true);
    
    
}

