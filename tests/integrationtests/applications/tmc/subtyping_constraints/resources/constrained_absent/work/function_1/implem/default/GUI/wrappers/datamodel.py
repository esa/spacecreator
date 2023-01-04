#!/usr/bin/python

import DV

FVname = "function_1"

tc = {}
tm = {}
errCodes = {}

tm["PI_2"] = {'nodeTypename': 'MyChoice', 'type': 'CHOICE', 'id': 'PI_2', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, "choices":[{'nodeTypename': '', 'type': 'BOOLEAN', 'id': 'a', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, 'default': 'False'},
{'nodeTypename': '', 'type': 'SEQUENCE', 'id': 'b', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, "children":[{'nodeTypename': '', 'type': 'INTEGER', 'id': 'inputdata', 'isOptional': False, 'alwaysPresent': True, 'alwaysAbsent': False, 'minR': 0, 'maxR': 10},
{'nodeTypename': '', 'type': 'INTEGER', 'id': 'outputdata', 'isOptional': False, 'alwaysPresent': True, 'alwaysAbsent': False, 'minR': 0, 'maxR': 10},
{'nodeTypename': '', 'type': 'ENUMERATED', 'id': 'validity', 'isOptional': False, 'alwaysPresent': True, 'alwaysAbsent': False, 'values':["valid", "invalid"], "valuesInt": {"valid": 0, "invalid": 1}}]}], "choiceIdx": {"a": DV.MyChoice_a_PRESENT,"b": DV.MyChoice_b_PRESENT}}
tc["PI_1"] = {'nodeTypename': 'MySeqOf', 'type': 'SEQOF', 'id': 'PI_1', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, 'minSize': 0, 'maxSize': 10, 'seqoftype':{'nodeTypename': '', 'type': 'ENUMERATED', 'id': 'PI_1', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, 'values':["hello", "world", "howareyou"], "valuesInt": {"hello": 0, "world": 1, "howareyou": 2}}
}
