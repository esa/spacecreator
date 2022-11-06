#ifndef REQUEST_SIZE_H
#define REQUEST_SIZE_H

#include <stdint.h>

#include "dataview-uniq.h"

// struct used to calculate buffer size for drivers
struct GenericLinuxAllParametersStub
{
    union
    {
        // input ports

        // output ports

        uint8_t dummy_union_field[1];
    } all_types;
};

// GENERIC_PARTITION_BUFFER_SIZE should be even number
#define GENERIC_PARTITION_BUFFER_SIZE (((sizeof(struct GenericLinuxAllParametersStub) + 1) / 2) * 2)


#define ACTUATOR1_ACTUATORTC_REQUEST_SIZE (sizeof (asn1SccActuatorCommand))

#define ACTUATOR2_ACTUATORTC_REQUEST_SIZE (sizeof (asn1SccActuatorCommand))

#define EGSE_POLL_REQUEST_SIZE (1)

#define EGSE_SYSTEMTM_REQUEST_SIZE (sizeof (asn1SccSystemTelemetry))

#define MODEMANAGER_ACTUATORTM1_REQUEST_SIZE (sizeof (asn1SccActuatorStatus))

#define MODEMANAGER_ACTUATORTM2_REQUEST_SIZE (sizeof (asn1SccActuatorStatus))

#define MODEMANAGER_READING_REQUEST_SIZE (sizeof (asn1SccDataItem))

#define MODEMANAGER_SYSTEMTC_REQUEST_SIZE (sizeof (asn1SccSystemCommand))

#define SENSOR_TRIGGER_REQUEST_SIZE (1)


#endif
