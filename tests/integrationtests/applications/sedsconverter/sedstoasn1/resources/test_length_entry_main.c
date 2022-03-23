#include "COM-N7SPACE-LENGTHENTRY.h"
#include "asn1crt.h"
#include <stdio.h>

int testCrc8Packet()
{
    Crc8_Packet packet;
    Crc8_Packet_Initialize((&(packet)));
    packet.nice_Packet_header.version = 42;
    packet.nice_Packet_header.release = 19;
    packet.nice_Packet_body.data = 111;

    int errCode;

    BitStream bitStream;
    uint8_t encBuff[Crc8_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream, encBuff, Crc8_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    bool ret = Crc8_Packet_ACN_Encode(&packet, &bitStream, &errCode, TRUE);
    if(!ret) {
        return errCode;
    }

    uint8_t encodedLength = bitStream.buf[3];
    if(encodedLength != 6) { // check length
        printf("Crc8 packet length mismatch. %d != 8\n", encodedLength);
        return 1;
    }

    uint8_t encodedCrc = bitStream.buf[5];
    if(encodedCrc != 0x90) { // check CRC8
        printf("Crc8 packet CRC mismatch. 0x%02x != 0x90\n", encodedCrc);
        return 1;
    }

    BitStream_AttachBuffer(&bitStream, encBuff, Crc8_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    Crc8_Packet decodedPacket;
    ret = Crc8_Packet_ACN_Decode(&decodedPacket, &bitStream, &errCode);
    if(!ret) {
        return errCode;
    }
}

int main()
{
    int ret = testCrc8Packet();

    if(ret != 0) {
        return ret;
    }

    return 0;
}
