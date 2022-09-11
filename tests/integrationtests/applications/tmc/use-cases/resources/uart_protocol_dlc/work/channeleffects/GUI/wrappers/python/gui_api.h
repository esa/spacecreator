#ifndef __HEADER_channeleffects_H__
#define __HEADER_channeleffects_H__

typedef unsigned char byte;

int OpenMsgQueueForReading(char *queueName);
void CloseMsgQueue(int queue_id);
int GetMsgQueueBufferSize(int queue_id);
int RetrieveMessageFromQueue(int queue_id, int maxSize, byte *pBuf);
int SendTC_CorruptChar(void *p_p1);
int SendTC_DropChar(void *p_no_param);

#endif
