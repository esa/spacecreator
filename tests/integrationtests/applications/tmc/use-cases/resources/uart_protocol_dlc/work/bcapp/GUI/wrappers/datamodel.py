#!/usr/bin/python

import DV

FVname = "bcapp"

tc = {}
tm = {}
errCodes = {}

tc["DLCTxReset"] = {'nodeTypename': 'T-Null-Record', 'type': 'SEQUENCE', 'id': 'DLCTxReset', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, "children":[]}
tc["DLCTxSDU"] = {'nodeTypename': 'T-UART-DLC-SDU',                        'type': 'OCTET STRING',                        'id': 'DLCTxSDU',                        'isOptional': False,                        'alwaysPresent': False,                        'alwaysAbsent': False,                        'minSize': 14,                        'maxSize': 14}
