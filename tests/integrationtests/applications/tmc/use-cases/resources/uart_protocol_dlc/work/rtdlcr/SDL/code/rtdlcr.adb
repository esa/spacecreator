-- This file was generated automatically by OpenGEODE: DO NOT MODIFY IT !

with System.IO;
use System.IO;

with Ada.Unchecked_Conversion;
with Ada.Numerics.Generic_Elementary_Functions;

package body rtdlcr is
   procedure p_0_IsValidUartHeader(inChar: in asn1SccUINT8;result: in out asn1SccT_ValidInvalidState);
   procedure p_0_CheckFrameCRC(rxFrame: in asn1SccT_UART_DLC_PDU;result: in out asn1SccT_SuccessFailState);
   procedure p_0_Decode(byteBuffer: in asn1SccT_UART_DLC_FrameBuffer;frame: in out asn1SccT_UART_DLC_PDU);
   procedure p_0_ResetDLCR;
   procedure p_0_ComputeCRC(rxFrame: in asn1SccT_UART_DLC_SDU;result: in out asn1SccT_UART_DLC_FrameCRC);
   procedure p_0_IsValidUartHeader(inChar: in asn1SccUINT8;result: in out asn1SccT_ValidInvalidState) is
      begin
         --  DECISION inChar (-1,-1)
         --  ANSWER C_UART_DLC_SYNC_VALUE (41,17)
         if (inChar) = asn1SccUINT8(c_UART_DLC_SYNC_VALUE) then
            --  result := valid (43,25)
            result := asn1Sccvalid;
            --  RETURN  (None,None) at 74, 370
            return;
            --  ANSWER else (None,None)
         else
            --  result := invalid (49,25)
            result := asn1Sccinvalid;
            --  RETURN  (None,None) at 296, 368
            return;
         end if;
      end p_0_IsValidUartHeader;
      

   procedure p_0_CheckFrameCRC(rxFrame: in asn1SccT_UART_DLC_PDU;result: in out asn1SccT_SuccessFailState) is
      computedcrc : asn1SccT_UART_DLC_FrameCRC;
      begin
         --  ComputeCRC(rxFrame.dlc_payload, computedCRC) (71,17)
         p_0_ComputeCRC(rxFrame.dlc_payload, computedCRC);
         --  DECISION computedCRC =
         --  rxFrame.crc (73,33)
         --  ANSWER true (76,17)
         if ((computedCRC = rxFrame.crc)) then
            --  writeln('Rx CRC pass') (78,25)
            Put ("Rx CRC pass");
            New_Line;
            --  result := success (80,25)
            result := asn1Sccsuccess;
            --  ANSWER else (None,None)
         else
            --  writeln('Rx CRC fail') (84,25)
            Put ("Rx CRC fail");
            New_Line;
            --  result := fail (86,25)
            result := asn1Sccfail;
         end if;
         --  RETURN  (None,None) at 749, 732
         return;
      end p_0_CheckFrameCRC;
      

   procedure p_0_Decode(byteBuffer: in asn1SccT_UART_DLC_FrameBuffer;frame: in out asn1SccT_UART_DLC_PDU) is
      begin
         --  COMMENT This ought not to give an OpenGeode
         --  warning. (113,12)
         --  frame.dlc_header := C_UART_DLC_SYNC_VALUE (111,17)
         frame.dlc_header := c_UART_DLC_SYNC_VALUE;
         --  TASK for  index in RANGE (0,c_UART_DLC_SDU_SIZE) :
         --    frame.dlc_payload(index) := byteBuffer(1+index)
         --  endfor (116,17)
         for index in 0 .. Integer (c_UART_DLC_SDU_SIZE) - 1 loop
            --  frame.dlc_payload(index) := byteBuffer(1+index) (117,2)
            frame.dlc_payload.Data(1 + Integer (index)) := byteBuffer.Data(1 + Integer ((1 + index)));
         end loop;
         --  frame.crc := byteBuffer(c_UART_DLC_SDU_SIZE+1) (120,17)
         frame.crc := Asn1UInt (byteBuffer.Data(1 + Integer ((c_UART_DLC_SDU_SIZE + 1))));
         --  RETURN  (None,None) at 330, 434
         return;
      end p_0_Decode;
      

   procedure p_0_ResetDLCR is
      begin
         --  rxCount := 0 (129,17)
         ctxt.rxCount := 0;
         --  TASK for index in range(0, C_UART_DLC_PDU_SIZE):
         --    rxFrameBuffer(index) := 0
         --  endfor (131,17)
         for index in 0 .. Integer (c_UART_DLC_PDU_SIZE) - 1 loop
            --  rxFrameBuffer(index) := 0 (132,2)
            ctxt.rxFrameBuffer.Data(1 + Integer (index)) := Interfaces.Unsigned_8 (0);
         end loop;
         --  RETURN  (None,None) at 243, 294
         return;
      end p_0_ResetDLCR;
      

   procedure p_0_ComputeCRC(rxFrame: in asn1SccT_UART_DLC_SDU;result: in out asn1SccT_UART_DLC_FrameCRC) is
      dataindex : asn1SccUINT16;
      crc : asn1SccUINT8;
      inputbyte : asn1SccUINT8;
      begin
         --  crc := 0 (154,17)
         crc := 0;
         --  dataIndex := 0 (155,0)
         dataIndex := 0;
         --  JOIN L1 (None,None) at None, None
         goto L1;
         --  CONNECTION L1 (157,12)
         <<L1>>
         --  DECISION dataIndex = length(rxFrame) (159,31)
         --  ANSWER true (161,17)
         if ((dataIndex = Asn1UInt (14))) then
            --  result := crc (163,25)
            result := crc;
            --  RETURN  (None,None) at 287, 640
            return;
            --  ANSWER false (167,17)
         else
            --  inputByte := rxFrame(dataIndex) (169,25)
            inputByte := Asn1UInt (rxFrame.Data(1 + Integer (dataIndex)));
            --  crc := c_RMAPCRCTable(crc xor inputByte) (171,25)
            crc := c_RMAPCRCTable.Data(1 + Integer ((crc xor inputByte)));
            --  dataIndex := dataIndex + 1 (173,25)
            dataIndex := (dataIndex + 1);
            --  JOIN L1 (175,25) at 538, 750
            goto L1;
         end if;
      end p_0_ComputeCRC;
      

   procedure DLCRxReset is
      begin
         case ctxt.state is
            when asn1Sccreceiving =>
               Execute_Transition (2);
            when asn1Sccidle =>
               Execute_Transition (2);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end DLCRxReset;
      

   procedure PHYRxChar(p1: in out asn1SccUINT8) is
      begin
         case ctxt.state is
            when asn1Sccreceiving =>
               ctxt.rxchar := p1;
               Execute_Transition (3);
            when asn1Sccidle =>
               ctxt.rxchar := p1;
               Execute_Transition (3);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end PHYRxChar;
      

   procedure PHYRxError is
      begin
         case ctxt.state is
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end PHYRxError;
      

   procedure FrameTimer is
      begin
         case ctxt.state is
            when asn1Sccreceiving =>
               Execute_Transition (1);
            when asn1Sccidle =>
               Execute_Transition (1);
            when others =>
               Execute_Transition (CS_Only);
         end case;
      end FrameTimer;
      

   procedure Execute_Transition (Id : Integer) is
      trId : Integer := Id;
      tmp2 : asn1SccT_UART_DLC_RxErrorCode;
      tmp33 : asn1SccT_UART_DLC_RxErrorCode;
      tmp50 : asn1SccT_UART_DLC_SDU;
      tmp29 : asn1SccT_UInt32;
      tmp53 : asn1SccT_UART_DLC_RxErrorCode;
      --  !! stack: _call_external_function line 1440
      begin
         while (trId /= -1) loop
            case trId is
               when 0 =>
                  --  ResetDLCR (181,13)
                  p_0_ResetDLCR;
                  --  NEXT_STATE IDLE (183,18) at 636, 105
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 1 =>
                  --  ResetDLCR (189,17)
                  p_0_ResetDLCR;
                  --  DLCRxError(FRAME_TIMEOUT) (191,19)
                  tmp2 := asn1Sccframe_timeout;
                  RI_0_DLCRxError(tmp2);
                  --  NEXT_STATE IDLE (193,22) at 1583, 256
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 2 =>
                  --  ResetDLCR (197,17)
                  p_0_ResetDLCR;
                  --  NEXT_STATE IDLE (199,22) at 1874, 195
                  trId := -1;
                  ctxt.State := asn1SccIDLE;
                  goto Continuous_Signals;
               when 3 =>
                  --  writeln('received char ', rxChar) (207,17)
                  Put ("received char ");
                  Put (asn1SccUINT8'Image (ctxt.rxChar));
                  New_Line;
                  --  rxFrameBuffer(rxCount) := rxChar (209,17)
                  ctxt.rxFrameBuffer.Data(1 + Integer (ctxt.rxCount)) := Interfaces.Unsigned_8 (ctxt.rxChar);
                  --  rxCount := (rxCount + 1) mod 256 (211,17)
                  ctxt.rxCount := ((ctxt.rxCount + 1) mod 256);
                  --  DECISION rxCount (-1,-1)
                  --  ANSWER 1 (215,17)
                  if (ctxt.rxCount) = asn1SccUINT8(1) then
                     --  IsValidUartHeader(rxChar, headerValid) (217,25)
                     p_0_IsValidUartHeader(ctxt.rxChar, ctxt.headerValid);
                     --  DECISION headerValid (-1,-1)
                     --  ANSWER valid (221,25)
                     if (ctxt.headerValid) = asn1Sccvalid then
                        --  set_timer(FRAME_TIMEOUT, FrameTimer) (223,33)
                        tmp29 := ctxt.FRAME_TIMEOUT;
                        SET_FrameTimer (tmp29);
                        --  NEXT_STATE RECEIVING (225,38) at 471, 709
                        trId := -1;
                        ctxt.State := asn1SccRECEIVING;
                        goto Continuous_Signals;
                        --  ANSWER invalid (227,25)
                     elsif (ctxt.headerValid) = asn1Sccinvalid then
                        --  DLCRxError(BAD_START_CHAR) (229,35)
                        tmp33 := asn1Sccbad_start_char;
                        RI_0_DLCRxError(tmp33);
                        --  ResetDLCR (231,33)
                        p_0_ResetDLCR;
                        --  NEXT_STATE IDLE (233,38) at 790, 759
                        trId := -1;
                        ctxt.State := asn1SccIDLE;
                        goto Continuous_Signals;
                     end if;
                     --  ANSWER C_UART_DLC_PDU_SIZE (236,17)
                  elsif (ctxt.rxCount) = asn1SccUINT8(c_UART_DLC_PDU_SIZE) then
                     --  reset_timer(FrameTimer) (238,25)
                     RESET_FrameTimer;
                     --  Decode(rxFrameBuffer, rxPDU) (240,25)
                     p_0_Decode(ctxt.rxFrameBuffer, ctxt.rxPDU);
                     --  CheckFrameCRC(rxPDU, crcResult) (242,25)
                     p_0_CheckFrameCRC(ctxt.rxPDU, ctxt.crcResult);
                     --  DECISION crcResult (-1,-1)
                     --  ANSWER success (246,25)
                     if (ctxt.crcResult) = asn1Sccsuccess then
                        --  DLCRxSDU(rxPDU.dlc_payload) (248,35)
                        tmp50 := ctxt.rxPDU.dlc_payload;
                        RI_0_DLCRxSDU(tmp50);
                        --  ANSWER fail (250,25)
                     elsif (ctxt.crcResult) = asn1Sccfail then
                        --  DLCRxError (CRC_FAIL) (252,35)
                        tmp53 := asn1Scccrc_fail;
                        RI_0_DLCRxError(tmp53);
                     end if;
                     --  ResetDLCR (255,25)
                     p_0_ResetDLCR;
                     --  NEXT_STATE IDLE (257,30) at 1238, 827
                     trId := -1;
                     ctxt.State := asn1SccIDLE;
                     goto Continuous_Signals;
                     --  ANSWER else (None,None)
                  else
                     --  NEXT_STATE RECEIVING (261,30) at 973, 451
                     trId := -1;
                     ctxt.State := asn1SccRECEIVING;
                     goto Continuous_Signals;
                  end if;
               when CS_Only =>
                  trId := -1;
                  goto Continuous_Signals;
               when others =>
                  null;
            end case;
            <<Continuous_Signals>>
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
end rtdlcr;