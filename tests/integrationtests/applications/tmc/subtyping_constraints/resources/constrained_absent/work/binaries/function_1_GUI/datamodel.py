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
errCodes = \
{1: {'name': 'MYINTEGER', 'constraint': '(0..10)'}, 2: {'name': 'UPER_ENCODE_MYINTEGER', 'constraint': ''}, 3: {'name': 'UPER_DECODE_MYINTEGER', 'constraint': ''}, 4: {'name': 'ACN_ENCODE_MYINTEGER', 'constraint': ''}, 5: {'name': 'ACN_DECODE_MYINTEGER', 'constraint': ''}, 31: {'name': 'MYSEQ_VALIDITY', 'constraint': 'valid|invalid'}, 36: {'name': 'MYSEQ', 'constraint': ''}, 16: {'name': 'MYSEQ_INPUTDATA_2', 'constraint': ''}, 26: {'name': 'MYSEQ_OUTPUTDATA_2', 'constraint': ''}, 37: {'name': 'UPER_ENCODE_MYSEQ', 'constraint': ''}, 17: {'name': 'UPER_ENCODE_MYSEQ_INPUTDATA_2', 'constraint': ''}, 27: {'name': 'UPER_ENCODE_MYSEQ_OUTPUTDATA_2', 'constraint': ''}, 32: {'name': 'UPER_ENCODE_MYSEQ_VALIDITY', 'constraint': ''}, 38: {'name': 'UPER_DECODE_MYSEQ', 'constraint': ''}, 18: {'name': 'UPER_DECODE_MYSEQ_INPUTDATA_2', 'constraint': ''}, 28: {'name': 'UPER_DECODE_MYSEQ_OUTPUTDATA_2', 'constraint': ''}, 33: {'name': 'UPER_DECODE_MYSEQ_VALIDITY', 'constraint': ''}, 39: {'name': 'ACN_ENCODE_MYSEQ', 'constraint': ''}, 19: {'name': 'ACN_ENCODE_MYSEQ_INPUTDATA_2', 'constraint': ''}, 29: {'name': 'ACN_ENCODE_MYSEQ_OUTPUTDATA_2', 'constraint': ''}, 34: {'name': 'ACN_ENCODE_MYSEQ_VALIDITY', 'constraint': ''}, 40: {'name': 'ACN_DECODE_MYSEQ', 'constraint': ''}, 20: {'name': 'ACN_DECODE_MYSEQ_INPUTDATA_2', 'constraint': ''}, 30: {'name': 'ACN_DECODE_MYSEQ_OUTPUTDATA_2', 'constraint': ''}, 35: {'name': 'ACN_DECODE_MYSEQ_VALIDITY', 'constraint': ''}, 81: {'name': 'MYCHOICE', 'constraint': ''}, 41: {'name': 'MYCHOICE_A', 'constraint': ''}, 76: {'name': 'MYCHOICE_B_2', 'constraint': ''}, 82: {'name': 'UPER_ENCODE_MYCHOICE', 'constraint': ''}, 42: {'name': 'UPER_ENCODE_MYCHOICE_A', 'constraint': ''}, 77: {'name': 'UPER_ENCODE_MYCHOICE_B_2', 'constraint': ''}, 83: {'name': 'UPER_DECODE_MYCHOICE', 'constraint': ''}, 43: {'name': 'UPER_DECODE_MYCHOICE_A', 'constraint': ''}, 78: {'name': 'UPER_DECODE_MYCHOICE_B_2', 'constraint': ''}, 84: {'name': 'ACN_ENCODE_MYCHOICE', 'constraint': ''}, 44: {'name': 'ACN_ENCODE_MYCHOICE_A', 'constraint': ''}, 79: {'name': 'ACN_ENCODE_MYCHOICE_B_2', 'constraint': ''}, 85: {'name': 'ACN_DECODE_MYCHOICE', 'constraint': ''}, 45: {'name': 'ACN_DECODE_MYCHOICE_A', 'constraint': ''}, 80: {'name': 'ACN_DECODE_MYCHOICE_B_2', 'constraint': ''}, 6: {'name': 'MYENUM', 'constraint': 'hello|world|howareyou'}, 7: {'name': 'UPER_ENCODE_MYENUM', 'constraint': ''}, 8: {'name': 'UPER_DECODE_MYENUM', 'constraint': ''}, 9: {'name': 'ACN_ENCODE_MYENUM', 'constraint': ''}, 10: {'name': 'ACN_DECODE_MYENUM', 'constraint': ''}, 96: {'name': 'MYSEQOF', 'constraint': '(SIZE(0..10))'}, 91: {'name': 'MYSEQOF_ELM_2', 'constraint': ''}, 97: {'name': 'UPER_ENCODE_MYSEQOF', 'constraint': ''}, 92: {'name': 'UPER_ENCODE_MYSEQOF_ELM_2', 'constraint': ''}, 98: {'name': 'UPER_DECODE_MYSEQOF', 'constraint': ''}, 93: {'name': 'UPER_DECODE_MYSEQOF_ELM_2', 'constraint': ''}, 99: {'name': 'ACN_ENCODE_MYSEQOF', 'constraint': ''}, 94: {'name': 'ACN_ENCODE_MYSEQOF_ELM_2', 'constraint': ''}, 100: {'name': 'ACN_DECODE_MYSEQOF', 'constraint': ''}, 95: {'name': 'ACN_DECODE_MYSEQOF_ELM_2', 'constraint': ''}, 101: {'name': 'T_INT32', 'constraint': '(-2147483648..2147483647)'}, 102: {'name': 'UPER_ENCODE_T_INT32', 'constraint': ''}, 103: {'name': 'UPER_DECODE_T_INT32', 'constraint': ''}, 104: {'name': 'ACN_ENCODE_T_INT32', 'constraint': ''}, 105: {'name': 'ACN_DECODE_T_INT32', 'constraint': ''}, 106: {'name': 'T_UINT32', 'constraint': '(0..4294967295)'}, 107: {'name': 'UPER_ENCODE_T_UINT32', 'constraint': ''}, 108: {'name': 'UPER_DECODE_T_UINT32', 'constraint': ''}, 109: {'name': 'ACN_ENCODE_T_UINT32', 'constraint': ''}, 110: {'name': 'ACN_DECODE_T_UINT32', 'constraint': ''}, 111: {'name': 'T_INT8', 'constraint': '(-128..127)'}, 112: {'name': 'UPER_ENCODE_T_INT8', 'constraint': ''}, 113: {'name': 'UPER_DECODE_T_INT8', 'constraint': ''}, 114: {'name': 'ACN_ENCODE_T_INT8', 'constraint': ''}, 115: {'name': 'ACN_DECODE_T_INT8', 'constraint': ''}, 116: {'name': 'T_UINT8', 'constraint': '(0..255)'}, 117: {'name': 'UPER_ENCODE_T_UINT8', 'constraint': ''}, 118: {'name': 'UPER_DECODE_T_UINT8', 'constraint': ''}, 119: {'name': 'ACN_ENCODE_T_UINT8', 'constraint': ''}, 120: {'name': 'ACN_DECODE_T_UINT8', 'constraint': ''}, 121: {'name': 'T_BOOLEAN', 'constraint': ''}, 122: {'name': 'UPER_ENCODE_T_BOOLEAN', 'constraint': ''}, 123: {'name': 'UPER_DECODE_T_BOOLEAN', 'constraint': ''}, 124: {'name': 'ACN_ENCODE_T_BOOLEAN', 'constraint': ''}, 125: {'name': 'ACN_DECODE_T_BOOLEAN', 'constraint': ''}, 126: {'name': 'T_NULL_RECORD', 'constraint': ''}, 131: {'name': 'PID', 'constraint': 'function-1|function-2|env'}, 132: {'name': 'UPER_ENCODE_PID', 'constraint': ''}, 133: {'name': 'UPER_DECODE_PID', 'constraint': ''}, 134: {'name': 'ACN_ENCODE_PID', 'constraint': ''}, 135: {'name': 'ACN_DECODE_PID', 'constraint': ''}}
