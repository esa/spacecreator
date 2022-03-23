#include "COM-N7SPACE-LENGTHENTRY.h"
#include "asn1crt.h"

int main()
{
    Nice_Packet packet;
    Nice_Packet_Initialize((&(packet)));
    packet.nice_Packet_header.version = 42;
    packet.nice_Packet_header.release = 19;
    packet.nice_Packet_body.data = 111;

    int errCode;

    BitStream bitStream;
    uint8_t encBuff[Nice_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream, encBuff, Nice_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    bool ret = Nice_Packet_ACN_Encode(&packet, &bitStream, &errCode, TRUE);
    if(!ret) {
        return errCode;
    }

    if(bitStream.buf[3] != 8) { // check length
        return 1;
    }

    if(bitStream.buf[7] != 0xAD) { // check CRC8
        return 1;
    }

    BitStream_AttachBuffer(&bitStream, encBuff, Nice_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    Nice_Packet decodedPacket;
    ret = Nice_Packet_ACN_Decode(&decodedPacket, &bitStream, &errCode);
    if(!ret) {
        return errCode;
    }

    return 0;
}
