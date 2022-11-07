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


#define ACTUATOR_CONTROLSIGNAL_REQUEST_SIZE (sizeof (asn1SccT_Float))

#define EGSE_POLL_REQUEST_SIZE (1)

#define EGSE_TM_REQUEST_SIZE (sizeof (asn1SccT_Int))

#define SENSOR_TRIGGER_REQUEST_SIZE (1)

#define SYSTEMCONTROL_READING_REQUEST_SIZE (sizeof (asn1SccT_Float))

#define SYSTEMCONTROL_REPORTING_REQUEST_SIZE (1)

#define SYSTEMCONTROL_TC_REQUEST_SIZE (sizeof (asn1SccT_Int))


#endif
