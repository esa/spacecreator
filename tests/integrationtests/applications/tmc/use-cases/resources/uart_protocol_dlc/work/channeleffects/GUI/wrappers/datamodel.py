#!/usr/bin/python

import DV

FVname = "channeleffects"

tc = {}
tm = {}
errCodes = {}

tc["CorruptChar"] = {'nodeTypename': 'UINT8', 'type': 'INTEGER', 'id': 'CorruptChar', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, 'minR': 0, 'maxR': 255}
tc["DropChar"] = {'nodeTypename': 'T-Null-Record', 'type': 'SEQUENCE', 'id': 'DropChar', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, "children":[]}
