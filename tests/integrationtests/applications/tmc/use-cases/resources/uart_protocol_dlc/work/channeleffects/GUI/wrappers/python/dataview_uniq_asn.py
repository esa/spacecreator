from functools import partial

import DV

from Stubs import (
    myassert, Clean, DataStream, COMMON)

class UINT40(COMMON):
    def __init__(self, ptr=None):
        super(UINT40, self).__init__("UINT40", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT32(COMMON):
    def __init__(self, ptr=None):
        super(UINT32, self).__init__("UINT32", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT24(COMMON):
    def __init__(self, ptr=None):
        super(UINT24, self).__init__("UINT24", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT16(COMMON):
    def __init__(self, ptr=None):
        super(UINT16, self).__init__("UINT16", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT14(COMMON):
    def __init__(self, ptr=None):
        super(UINT14, self).__init__("UINT14", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT12(COMMON):
    def __init__(self, ptr=None):
        super(UINT12, self).__init__("UINT12", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT11(COMMON):
    def __init__(self, ptr=None):
        super(UINT11, self).__init__("UINT11", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT10(COMMON):
    def __init__(self, ptr=None):
        super(UINT10, self).__init__("UINT10", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT8(COMMON):
    def __init__(self, ptr=None):
        super(UINT8, self).__init__("UINT8", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT7(COMMON):
    def __init__(self, ptr=None):
        super(UINT7, self).__init__("UINT7", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT6(COMMON):
    def __init__(self, ptr=None):
        super(UINT6, self).__init__("UINT6", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT5(COMMON):
    def __init__(self, ptr=None):
        super(UINT5, self).__init__("UINT5", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT4(COMMON):
    def __init__(self, ptr=None):
        super(UINT4, self).__init__("UINT4", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT3(COMMON):
    def __init__(self, ptr=None):
        super(UINT3, self).__init__("UINT3", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT2(COMMON):
    def __init__(self, ptr=None):
        super(UINT2, self).__init__("UINT2", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class UINT1(COMMON):
    def __init__(self, ptr=None):
        super(UINT1, self).__init__("UINT1", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_Bit(COMMON):
    def __init__(self, ptr=None):
        super(T_Bit, self).__init__("T_Bit", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class INT16(COMMON):
    def __init__(self, ptr=None):
        super(INT16, self).__init__("INT16", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class INT32(COMMON):
    def __init__(self, ptr=None):
        super(INT32, self).__init__("INT32", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class INT24(COMMON):
    def __init__(self, ptr=None):
        super(INT24, self).__init__("INT24", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_OCTET(COMMON):
    def __init__(self, ptr=None):
        super(T_OCTET, self).__init__("T_OCTET", ptr)
#

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        value = self.GetPyString(tryToDecodeAscii=False)
        value = "'" + value.hex().upper() + "'H"
        lines.append(""+value)

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_OnOffState(COMMON):
    # Allowed enumerants:
    off = 0
    on = 1
    allowed = [off, on]
    def __init__(self, ptr=None):
        super(T_OnOffState, self).__init__("T_OnOffState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'off', '1': 'on'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_NominalRedundantState(COMMON):
    # Allowed enumerants:
    redundant = 0
    nominal = 1
    allowed = [redundant, nominal]
    def __init__(self, ptr=None):
        super(T_NominalRedundantState, self).__init__("T_NominalRedundantState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'redundant', '1': 'nominal'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_ConnectionState(COMMON):
    # Allowed enumerants:
    disconnected = 0
    connected = 1
    allowed = [disconnected, connected]
    def __init__(self, ptr=None):
        super(T_ConnectionState, self).__init__("T_ConnectionState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'disconnected', '1': 'connected'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_SwitchState(COMMON):
    # Allowed enumerants:
    switch_open = 0
    switch_closed = 1
    allowed = [switch_open, switch_closed]
    def __init__(self, ptr=None):
        super(T_SwitchState, self).__init__("T_SwitchState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'switch-open', '1': 'switch-closed'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_FaultState(COMMON):
    # Allowed enumerants:
    fault = 0
    ok = 1
    allowed = [fault, ok]
    def __init__(self, ptr=None):
        super(T_FaultState, self).__init__("T_FaultState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'fault', '1': 'ok'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_SelectionState(COMMON):
    # Allowed enumerants:
    not_selected = 0
    selected = 1
    allowed = [not_selected, selected]
    def __init__(self, ptr=None):
        super(T_SelectionState, self).__init__("T_SelectionState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'not-selected', '1': 'selected'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_PresenceState(COMMON):
    # Allowed enumerants:
    absent = 0
    present = 1
    allowed = [absent, present]
    def __init__(self, ptr=None):
        super(T_PresenceState, self).__init__("T_PresenceState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'absent', '1': 'present'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_ForwardBackwardState(COMMON):
    # Allowed enumerants:
    backward = 0
    forward = 1
    allowed = [backward, forward]
    def __init__(self, ptr=None):
        super(T_ForwardBackwardState, self).__init__("T_ForwardBackwardState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'backward', '1': 'forward'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_LeftRightState(COMMON):
    # Allowed enumerants:
    left = 0
    right = 1
    allowed = [left, right]
    def __init__(self, ptr=None):
        super(T_LeftRightState, self).__init__("T_LeftRightState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'left', '1': 'right'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UsedUnusedState(COMMON):
    # Allowed enumerants:
    unused = 0
    used = 1
    allowed = [unused, used]
    def __init__(self, ptr=None):
        super(T_UsedUnusedState, self).__init__("T_UsedUnusedState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'unused', '1': 'used'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_MovingStillState(COMMON):
    # Allowed enumerants:
    stationary = 0
    moving = 1
    allowed = [stationary, moving]
    def __init__(self, ptr=None):
        super(T_MovingStillState, self).__init__("T_MovingStillState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'stationary', '1': 'moving'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_PlusMinusState(COMMON):
    # Allowed enumerants:
    plus = 0
    minus = 1
    allowed = [plus, minus]
    def __init__(self, ptr=None):
        super(T_PlusMinusState, self).__init__("T_PlusMinusState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'plus', '1': 'minus'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_ArmedDisarmedState(COMMON):
    # Allowed enumerants:
    disarmed = 0
    armed = 1
    allowed = [disarmed, armed]
    def __init__(self, ptr=None):
        super(T_ArmedDisarmedState, self).__init__("T_ArmedDisarmedState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'disarmed', '1': 'armed'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_ActiveInactiveState(COMMON):
    # Allowed enumerants:
    inactive = 0
    active = 1
    allowed = [inactive, active]
    def __init__(self, ptr=None):
        super(T_ActiveInactiveState, self).__init__("T_ActiveInactiveState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'inactive', '1': 'active'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_EnabledInhibitedState(COMMON):
    # Allowed enumerants:
    inhibited = 0
    enabled = 1
    allowed = [inhibited, enabled]
    def __init__(self, ptr=None):
        super(T_EnabledInhibitedState, self).__init__("T_EnabledInhibitedState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'inhibited', '1': 'enabled'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_FlagState(COMMON):
    # Allowed enumerants:
    clear = 0
    set = 1
    allowed = [clear, set]
    def __init__(self, ptr=None):
        super(T_FlagState, self).__init__("T_FlagState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'clear', '1': 'set'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_SuccessFailState(COMMON):
    # Allowed enumerants:
    fail = 0
    success = 1
    allowed = [fail, success]
    def __init__(self, ptr=None):
        super(T_SuccessFailState, self).__init__("T_SuccessFailState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'fail', '1': 'success'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_HighLowState(COMMON):
    # Allowed enumerants:
    low = 0
    high = 1
    allowed = [low, high]
    def __init__(self, ptr=None):
        super(T_HighLowState, self).__init__("T_HighLowState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'low', '1': 'high'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_OverrideState(COMMON):
    # Allowed enumerants:
    normal = 0
    overridden = 1
    allowed = [normal, overridden]
    def __init__(self, ptr=None):
        super(T_OverrideState, self).__init__("T_OverrideState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'normal', '1': 'overridden'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_LockedUnlockedState(COMMON):
    # Allowed enumerants:
    unlocked = 0
    locked = 1
    allowed = [unlocked, locked]
    def __init__(self, ptr=None):
        super(T_LockedUnlockedState, self).__init__("T_LockedUnlockedState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'unlocked', '1': 'locked'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_WarningState(COMMON):
    # Allowed enumerants:
    ok = 0
    warning = 1
    allowed = [ok, warning]
    def __init__(self, ptr=None):
        super(T_WarningState, self).__init__("T_WarningState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'ok', '1': 'warning'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_ValidInvalidState(COMMON):
    # Allowed enumerants:
    invalid = 0
    valid = 1
    allowed = [invalid, valid]
    def __init__(self, ptr=None):
        super(T_ValidInvalidState, self).__init__("T_ValidInvalidState", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'invalid', '1': 'valid'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UART_DLC_Header(COMMON):
    def __init__(self, ptr=None):
        super(T_UART_DLC_Header, self).__init__("T_UART_DLC_Header", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UART_DLC_FrameCRC(COMMON):
    def __init__(self, ptr=None):
        super(T_UART_DLC_FrameCRC, self).__init__("T_UART_DLC_FrameCRC", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_RMAPCRCTable(COMMON):
    def __init__(self, ptr=None):
        super(T_RMAPCRCTable, self).__init__("T_RMAPCRCTable", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append("{")
        def emitElem(path, i):
            state = path.GetState()
            if i > 0:
                lines.append(",")
            lines.append(" "+str(path[i].Get()))
            path.Reset(state)
        state = self.GetState()
        length = self.GetLength()
        self.Reset(state)
        list(map(partial(emitElem, self), range(length)))
        self.Reset(state)
        lines.append("}")

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UART_DLC_FrameBuffer(COMMON):
    def __init__(self, ptr=None):
        super(T_UART_DLC_FrameBuffer, self).__init__("T_UART_DLC_FrameBuffer", ptr)
#

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        value = self.GetPyString(tryToDecodeAscii=False)
        value = "'" + value.hex().upper() + "'H"
        lines.append(""+value)

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UART_DLC_SDU(COMMON):
    def __init__(self, ptr=None):
        super(T_UART_DLC_SDU, self).__init__("T_UART_DLC_SDU", ptr)
#

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        value = self.GetPyString(tryToDecodeAscii=False)
        value = "'" + value.hex().upper() + "'H"
        lines.append(""+value)

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UART_DLC_PDU(COMMON):
    # Ordered list of fields:
    children_ordered = ['dlc-header', 'dlc-payload', 'crc']

    def __init__(self, ptr=None):
        super(T_UART_DLC_PDU, self).__init__("T_UART_DLC_PDU", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append("{")
        lines.append("dlc-header ")
        lines.append(" "+str(self.dlc_header.Get()))
        lines.append(', ')
        lines.append("dlc-payload ")
        value = self.dlc_payload.GetPyString(tryToDecodeAscii=False)
        value = "'" + value.hex().upper() + "'H"
        lines.append(" "+value)
        lines.append(', ')
        lines.append("crc ")
        lines.append(" "+str(self.crc.Get()))
        lines.append("}")

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UART_DLC_RxErrorCode(COMMON):
    # Allowed enumerants:
    no_error = 0
    bad_start_char = 1
    crc_fail = 2
    frame_timeout = 3
    bad_request_type = 4
    allowed = [no_error, bad_start_char, crc_fail, frame_timeout, bad_request_type]
    def __init__(self, ptr=None):
        super(T_UART_DLC_RxErrorCode, self).__init__("T_UART_DLC_RxErrorCode", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'no-error', '1': 'bad-start-char', '2': 'crc-fail', '3': 'frame-timeout', '4': 'bad-request-type'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_Int32(COMMON):
    def __init__(self, ptr=None):
        super(T_Int32, self).__init__("T_Int32", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UInt32(COMMON):
    def __init__(self, ptr=None):
        super(T_UInt32, self).__init__("T_UInt32", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_Int8(COMMON):
    def __init__(self, ptr=None):
        super(T_Int8, self).__init__("T_Int8", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_UInt8(COMMON):
    def __init__(self, ptr=None):
        super(T_UInt8, self).__init__("T_UInt8", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_Boolean(COMMON):
    def __init__(self, ptr=None):
        super(T_Boolean, self).__init__("T_Boolean", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()!=0).upper())

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class T_Null_Record(COMMON):
    # Ordered list of fields:
    children_ordered = ['']

    def __init__(self, ptr=None):
        super(T_Null_Record, self).__init__("T_Null_Record", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append("{")
        lines.append("}")

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class MyInteger(COMMON):
    def __init__(self, ptr=None):
        super(MyInteger, self).__init__("MyInteger", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+str(self.Get()))

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class PID(COMMON):
    # Allowed enumerants:
    bcapp = 0
    bcdlct = 1
    channeleffects = 2
    erroredchannel = 3
    rtapp = 4
    rtdlcr = 5
    env = 6
    allowed = [bcapp, bcdlct, channeleffects, erroredchannel, rtapp, rtdlcr, env]
    def __init__(self, ptr=None):
        super(PID, self).__init__("PID", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'bcapp', '1': 'bcdlct', '2': 'channeleffects', '3': 'erroredchannel', '4': 'rtapp', '5': 'rtdlcr', '6': 'env'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


