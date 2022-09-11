#!/usr/bin/python

import DV

FVname = "rtapp"

tc = {}
tm = {}
errCodes = {}

tm["DLCRxError"] = {'nodeTypename': 'T-UART-DLC-RxErrorCode', 'type': 'ENUMERATED', 'id': 'DLCRxError', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, 'values':["no-error", "bad-start-char", "crc-fail", "frame-timeout", "bad-request-type"], "valuesInt": {"no-error": 0, "bad-start-char": 1, "crc-fail": 2, "frame-timeout": 3, "bad-request-type": 4}}
tm["DLCRxSDU"] = {'nodeTypename': 'T-UART-DLC-SDU',                        'type': 'OCTET STRING',                        'id': 'DLCRxSDU',                        'isOptional': False,                        'alwaysPresent': False,                        'alwaysAbsent': False,                        'minSize': 14,                        'maxSize': 14}
tc["DLCRxReset"] = {'nodeTypename': 'T-Null-Record', 'type': 'SEQUENCE', 'id': 'DLCRxReset', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, "children":[]}
