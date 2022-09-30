-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body Bcdlct is
   procedure p_0_Encode(frame: in asn1SccT_UART_DLC_PDU;byteBuffer: in out asn1SccT_UART_DLC_FrameBuffer);
   procedure p_0_ComputeCRC(rxFrame: in asn1SccT_UART_DLC_SDU;result: in out asn1SccT_UART_DLC_FrameCRC);
   procedure p_0_Encode(frame: in asn1SccT_UART_DLC_PDU;byteBuffer: in out asn1SccT_UART_DLC_FrameBuffer) is
      begin
         --  byteBuffer(0) := frame.dlc_header (32,17)
         byteBuffer.Data(1) := Interfaces.Unsigned_8 (frame.dlc_header);
         --  TASK for  index in RANGE (0,c_UART_DLC_SDU_SIZE) :
         --     byteBuffer(1+index) := frame.dlc_payload(index)
         --  endfor (34,17)
         for index in 0 .. Integer (c_UART_DLC_SDU_SIZE) - 1 loop
            --  byteBuffer(1+index) := frame.dlc_payload(index) (35,3)
            byteBuffer.Data(1 + Integer ((1 + index))) := frame.dlc_payload.Data(1 + Integer (index));
         end loop;
         --  byteBuffer(C_UART_DLC_SDU_SIZE + 1) := txPDU.crc (38,17)
         byteBuffer.Data(1 + Integer ((c_UART_DLC_SDU_SIZE + 1))) := Interfaces.Unsigned_8 (ctxt.txPDU.crc);
         --  RETURN  (None,None) at 174, 280
         return;
      end p_0_Encode;
      

   procedure p_0_ComputeCRC(rxFrame: in asn1SccT_UART_DLC_SDU;result: in out asn1SccT_UART_DLC_FrameCRC) is
      dataindex : asn1SccUINT16;
      crc : asn1SccUINT8;
      inputbyte : asn1SccUINT8;
      begin
         --  crc := 0 (59,17)
         crc := 0;
         --  dataIndex := 0 (60,0)
         dataIndex := 0;
         --  JOIN L1 (None,None) at None, None
         goto L1;
         --  CONNECTION L1 (62,12)
         <<L1>>
         --  DECISION dataIndex = length(rxFrame) (64,31)
         --  ANSWER true (66,17)
         if ((dataIndex = Asn1UInt (14))) then
            --  result := crc (68,25)
            result := crc;
            --  RETURN  (None,None) at 180, 516
            return;
            --  ANSWER false (72,17)
         else
            --  inputByte := rxFrame(dataIndex) (74,25)
            inputByte := Asn1UInt (rxFrame.Data(1 + Integer (dataIndex)));
            --  crc := c_RMAPCRCTable(crc xor inputByte) (76,25)
            crc := c_RMAPCRCTable.Data(1 + Integer ((crc xor inputByte)));
            --  dataIndex := dataIndex + 1 (78,25)
            dataIndex := (dataIndex + 1);
            --  JOIN L1 (80,25) at 431, 626
            goto L1;
         end if;
      end p_0_ComputeCRC;
      

   procedure DLCTxReset is
      begin
         case ctxt.state is
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end DLCTxReset;
      

   procedure DLCTxSDU(p1: in out asn1SccT_UART_DLC_SDU) is
      begin
         case ctxt.state is
            when asn1Sccidle =>
               ctxt.txsdu := p1;
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end DLCTxSDU;
      

   procedure PHYTxError is
      begin
         case ctxt.state is
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end PHYTxError;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      Message_Pending : Asn1Boolean := True;
      tmp23 : asn1SccUINT8;
      --  !! stack: _call_external_function line 1440
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  NEXT_STATE IDLE (86,18) at 320, 60
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 1 =>
                  --  txPDU.dlc_header := C_UART_DLC_SYNC_VALUE (92,17)
                  ctxt.txPDU.dlc_header := c_UART_DLC_SYNC_VALUE;
                  --  txPDU.dlc_payload := txSDU (94,17)
                  ctxt.txPDU.dlc_payload := ctxt.txSDU;
                  --  ComputeCRC(txSDU, txPDU.crc) (96,17)
                  p_0_ComputeCRC(ctxt.txSDU, ctxt.txPDU.crc);
                  --  Encode(txPDU, txFrameBuffer) (98,17)
                  p_0_Encode(ctxt.txPDU, ctxt.txFrameBuffer);
                  --  txCount := 0 (100,17)
                  ctxt.txCount := 0;
                  --  NEXT_STATE TRANSMITTING (102,22) at 531, 412
                  trId := -1;
                  ctxt.State := asn1SccTRANSMITTING;
                  goto Continuous_Signals;
               when 2 =>
                  --  PHYTxChar( txFrameBuffer(txCount)) (109,19)
                  tmp23 := asn1SccUINT8 (ctxt.txFrameBuffer.Data(1 + Integer (ctxt.txCount)));
                  RI_0_PHYTxChar(tmp23);
                  --  txCount := txCount + 1 (111,17)
                  ctxt.txCount := (ctxt.txCount + 1);
                  --  NEXT_STATE TRANSMITTING (113,22) at 408, 683
                  trId := -1;
                  ctxt.State := asn1SccTRANSMITTING;
                  goto Continuous_Signals;
               when 3 =>
                  --  NEXT_STATE IDLE (117,22) at 697, 573
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when CS_Only =>
                  trId := -1;
                  goto Continuous_Signals;
               when others =>
                  null;
            end case;
            <<Continuous_Signals>>
            --  Process continuous signals
            if ctxt.Init_Done then
               Check_Queue (Message_Pending);
            end if;
            if Message_Pending or trId /= -1 then
               goto Next_Transition;
            end if;
            if ctxt.State = asn1Scctransmitting then
               --  Priority: 1
               --  DECISION txCount < C_UART_DLC_PDU_SIZE (107,25)
               --  ANSWER true (None,None)
               if ((ctxt.txCount < c_UART_DLC_PDU_SIZE)) then
                  trId := 2;
                  --  Priority: 1
                  --  DECISION txCount >= C_UART_DLC_PDU_SIZE (115,25)
                  --  ANSWER true (None,None)
               elsif ((ctxt.txCount >= c_UART_DLC_PDU_SIZE)) then
                  trId := 3;
               end if;
            end if;
            <<Next_Transition>>
         end loop;
      end Execute_Transition;
      

   procedure Startup is
      begin
         Execute_Transition (0);
         ctxt.Init_Done := True;
      end Startup;
   begin
      Startup;
end Bcdlct;