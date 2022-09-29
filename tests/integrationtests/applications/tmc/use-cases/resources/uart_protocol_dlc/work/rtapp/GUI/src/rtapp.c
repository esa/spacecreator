/* Body file for GUI RTApp
 * Generated by TASTE/Kazoo on 2022-09-30 01:33:13
 * DO NOT EDIT THIS FILE MANUALLY - MODIFY THE KAZOO TEMPLATE IF NECESSARY
*/
#include <unistd.h>
#include <mqueue.h>

#include "queue_manager.h"
#include "rtapp.h"
#include "rtapp_enums_def.h"

typedef struct _PI_Messages {
   T_rtapp_PI_list msg_id;
   union {
      asn1SccT_UART_DLC_RxErrorCode dlcrxerror_param;
      asn1SccT_UART_DLC_SDU dlcrxsdu_param;
   } msg_data;
} PI_Messages;

typedef struct _RI_Messages {
   T_rtapp_RI_list msg_id;
   union {
   } msg_data;
} RI_Messages;

// Queues of messages going from the binary to the user (PIs, or TMs)
static mqd_t rtapp_PI_queue_id,
             rtapp_PI_Python_queue_id;

// Queues of messages going from the user to the binary GUI (RIs, or TCs)
static mqd_t rtapp_RI_queue_id;


void rtapp_startup(void)
{
   unsigned msgsize_max = 8192;
   FILE *f = fopen("/proc/sys/fs/mqueue/msgsize_max", "r");
   fscanf(f, "%d", &msgsize_max);

   if (msgsize_max < sizeof (PI_Messages) || msgsize_max < sizeof (RI_Messages)) {
      printf("[ERROR] The GUI is passing a message which parameter size "
             "exceeds your system limit (which is %d bytes per message).\n"
             "You can extend this limit by running: \n"
             "    echo NUMBER | sudo tee /proc/sys/fs/mqueue/msgsize_max\n"
             " ...  with NUMBER > %ld\n"
             "You can also make it permanent (check TASTE wiki)\n\n",
             msgsize_max,
             sizeof(PI_Messages) > sizeof(RI_Messages) ? sizeof(PI_Messages):sizeof(RI_Messages));
      exit(1);
   }

    char *gui_queue_name = NULL;
    int  len;

    len = snprintf (gui_queue_name, 0, "%d_rtapp_RI_queue", geteuid());
    gui_queue_name = (char *) malloc ((size_t) len + 1);
    if (NULL != gui_queue_name) {
       snprintf (gui_queue_name, len + 1, "%d_rtapp_RI_queue", geteuid());

       create_exchange_queue(gui_queue_name, 5, sizeof(RI_Messages), &rtapp_RI_queue_id);

       free (gui_queue_name);
       gui_queue_name = NULL;
    }

    len = snprintf (gui_queue_name, 0, "%d_rtapp_PI_queue", geteuid());
    gui_queue_name = (char *) malloc ((size_t) len + 1);
    if (NULL != gui_queue_name) {
       snprintf (gui_queue_name, len + 1, "%d_rtapp_PI_queue", geteuid());

       create_exchange_queue(gui_queue_name, 10, sizeof(PI_Messages), &rtapp_PI_queue_id);

       free (gui_queue_name);
       gui_queue_name = NULL;
    }

    len = snprintf (gui_queue_name, 0, "%d_rtapp_PI_Python_queue", geteuid());
    gui_queue_name = (char *) malloc ((size_t) len + 1);
    if (NULL != gui_queue_name) {
       snprintf (gui_queue_name, len + 1, "%d_rtapp_PI_Python_queue", geteuid());

       /* Extra queue for the TM sent to the Python mappers */
       create_exchange_queue(gui_queue_name, 10, sizeof (PI_Messages), &rtapp_PI_Python_queue_id);

       free (gui_queue_name);
       gui_queue_name = NULL;
    }
}

//  Function polling the message queue for incoming message from the user
void rtapp_PI_Poll(void)
{
   struct mq_attr msgq_attr;
   char* msgcontent = NULL;

   T_rtapp_RI_list message_received_type;

   if ((msgcontent = (char*)malloc(sizeof(RI_Messages))) == NULL) {
      perror("Error when allocating memory in GUI polling function");
      exit (-1);
   }

   mq_getattr(rtapp_RI_queue_id, &msgq_attr);

   while (!retrieve_message_from_queue(rtapp_RI_queue_id,
                                       sizeof(RI_Messages),
                                       msgcontent,
                                       (int *)&message_received_type)) {
      switch(message_received_type) {
         case i_DLCRxReset:
            rtapp_RI_DLCRxReset();
            break;
       default : break;
      }
   }

   free(msgcontent);
   return;
}

void rtapp_PI_DLCRxError
      (const asn1SccT_UART_DLC_RxErrorCode *IN_p1)

{
   write_message_to_queue
      (rtapp_PI_queue_id,
       sizeof(asn1SccT_UART_DLC_RxErrorCode),
       (void*)IN_p1,
       i_DLCRxError);

   write_message_to_queue
      (rtapp_PI_Python_queue_id,
       sizeof(asn1SccT_UART_DLC_RxErrorCode),
       (void*)IN_p1,
       i_DLCRxError);
}


void rtapp_PI_DLCRxSDU
      (const asn1SccT_UART_DLC_SDU *IN_p1)

{
   write_message_to_queue
      (rtapp_PI_queue_id,
       sizeof(asn1SccT_UART_DLC_SDU),
       (void*)IN_p1,
       i_DLCRxSDU);

   write_message_to_queue
      (rtapp_PI_Python_queue_id,
       sizeof(asn1SccT_UART_DLC_SDU),
       (void*)IN_p1,
       i_DLCRxSDU);
}


