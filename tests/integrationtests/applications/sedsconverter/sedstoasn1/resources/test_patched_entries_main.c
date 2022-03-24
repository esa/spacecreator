#include "COM-N7SPACE-PATCHED.h"
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

    const uint8_t encodedLength = bitStream.buf[3];
    const uint8_t expectedLength = 6;
    if(encodedLength != expectedLength) { // check length
        printf("Crc8 packet length mismatch. %d != %d\n", encodedLength, expectedLength);
        return 1;
    }

    const uint8_t encodedCrc = bitStream.buf[5];
    const uint8_t expectedCrc = 0x90;
    if(encodedCrc != expectedCrc) { // check CRC8
        printf("Crc8 packet CRC mismatch. 0x%02X != 0x%02X\n", encodedCrc, expectedCrc);
        return 1;
    }

    BitStream_AttachBuffer(&bitStream, encBuff, Crc8_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    Crc8_Packet decodedPacket;
    ret = Crc8_Packet_ACN_Decode(&decodedPacket, &bitStream, &errCode);
    if(!ret) {
        return errCode;
    }

    return 0;
}

int testCrc16Packet()
{
    Crc16_Packet packet;
    Crc16_Packet_Initialize((&(packet)));
    packet.nice_Packet_header.version = 31;
    packet.nice_Packet_header.release = 41;
    packet.nice_Packet_body.data = 59;

    int errCode;

    BitStream bitStream;
    uint8_t encBuff[Crc16_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream, encBuff, Crc16_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    bool ret = Crc16_Packet_ACN_Encode(&packet, &bitStream, &errCode, TRUE);
    if(!ret) {
        return errCode;
    }

    const uint8_t encodedLength = bitStream.buf[5];
    const uint8_t expectedLength = 9;
    if(encodedLength != expectedLength) { // check length
        printf("Crc16 packet length mismatch. %d != %d\n", encodedLength, expectedLength);
        return 1;
    }

    const uint16_t encodedCrc = bitStream.buf[8] | bitStream.buf[7] << 8;
    const uint16_t expectedCrc = 0x0C16;
    if(encodedCrc != expectedCrc) { // check CRC16
        printf("Crc16 packet CRC mismatch. 0x%04X != 0x%04X\n", encodedCrc, expectedCrc);
        return 1;
    }

    BitStream_AttachBuffer(&bitStream, encBuff, Crc16_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    Crc16_Packet decodedPacket;
    ret = Crc16_Packet_ACN_Decode(&decodedPacket, &bitStream, &errCode);
    if(!ret) {
        return errCode;
    }

    return 0;
}

int testChecksumPacket()
{
    Checksum_Packet packet;
    Checksum_Packet_Initialize((&(packet)));
    packet.nice_Packet_header.version = 31;
    packet.nice_Packet_header.release = 41;
    packet.nice_Packet_body.data = 59;

    int errCode;

    BitStream bitStream;
    uint8_t encBuff[Checksum_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream, encBuff, Checksum_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    bool ret = Checksum_Packet_ACN_Encode(&packet, &bitStream, &errCode, TRUE);
    if(!ret) {
        return errCode;
    }

    const uint8_t encodedLength = bitStream.buf[3];
    const uint8_t expectedLength = 9;
    if(encodedLength != expectedLength) { // check length
        printf("Checksum packet length mismatch. %d != %d\n", encodedLength, expectedLength);
        return 1;
    }

    const uint32_t encodedChecksum = (bitStream.buf[5] << 24) | (bitStream.buf[6] << 16) | bitStream.buf[7] << 8 | bitStream.buf[8];
    const uint32_t expectedChecksum = 1512636425;
    if(encodedChecksum != expectedChecksum) { // check checksum
        printf("Checksum packet CRC mismatch. %d != %d\n", encodedChecksum, expectedChecksum);
        return 1;
    }

    BitStream_AttachBuffer(&bitStream, encBuff, Checksum_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    Checksum_Packet decodedPacket;
    ret = Checksum_Packet_ACN_Decode(&decodedPacket, &bitStream, &errCode);
    if(!ret) {
        return errCode;
    }

    return 0;
}

int testChecksumLongitudinalPacket()
{
    ChecksumLongitudinal_Packet packet;
    ChecksumLongitudinal_Packet_Initialize((&(packet)));
    packet.nice_Packet_header.version = 31;
    packet.nice_Packet_header.release = 41;
    packet.nice_Packet_body.data = 59;

    int errCode;

    BitStream bitStream;
    uint8_t encBuff[ChecksumLongitudinal_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream, encBuff, ChecksumLongitudinal_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    bool ret = ChecksumLongitudinal_Packet_ACN_Encode(&packet, &bitStream, &errCode, TRUE);
    if(!ret) {
        return errCode;
    }

    const uint8_t encodedLength = bitStream.buf[3];
    const uint8_t expectedLength = 6;
    if(encodedLength != expectedLength) { // check length
        printf("ChecksumLongitudinal packet length mismatch. %d != %d\n", encodedLength, expectedLength);
        return 1;
    }

    const uint32_t encodedChecksumLongitudinal = bitStream.buf[5];
    const uint32_t expectedChecksumLongitudinal = 0x77;
    if(encodedChecksumLongitudinal != expectedChecksumLongitudinal) { // check checksum
        printf("ChecksumLongitudinal packet CRC mismatch. %02x != %02x\n", encodedChecksumLongitudinal, expectedChecksumLongitudinal);
        return 1;
    }

    BitStream_AttachBuffer(&bitStream, encBuff, ChecksumLongitudinal_Packet_REQUIRED_BYTES_FOR_ACN_ENCODING);

    ChecksumLongitudinal_Packet decodedPacket;
    ret = ChecksumLongitudinal_Packet_ACN_Decode(&decodedPacket, &bitStream, &errCode);
    if(!ret) {
        return errCode;
    }

    return 0;
}

int main()
{
    int ret = testCrc8Packet();
    if(ret != 0) {
        return ret;
    }

    ret = testCrc16Packet();
    if(ret != 0) {
        return ret;
    }

    ret = testChecksumPacket();
    if(ret != 0) {
        return ret;
    }

    ret = testChecksumLongitudinalPacket();
    if(ret != 0) {
        return ret;
    }

    return 0;
}
