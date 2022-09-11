#ifndef __HEADER_bcapp_H__
#define __HEADER_bcapp_H__

typedef unsigned char byte;

int OpenMsgQueueForReading(char *queueName);
void CloseMsgQueue(int queue_id);
int GetMsgQueueBufferSize(int queue_id);
int RetrieveMessageFromQueue(int queue_id, int maxSize, byte *pBuf);
int SendTC_DLCTxReset(void *p_no_param);
int SendTC_DLCTxSDU(void *p_p1);

#endif
