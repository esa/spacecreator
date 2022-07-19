#!/usr/bin/python

import DV

FVname = "clock"

tc = {}
tm = {}
errCodes = {}

tc["Tick"] = {'nodeTypename': 'T-Null-Record', 'type': 'SEQUENCE', 'id': 'Tick', 'isOptional': False, 'alwaysPresent': False, 'alwaysAbsent': False, "children":[]}
errCodes = \
{1: {'name': 'MYINTEGER', 'constraint': ''}, 2: {'name': 'UPER_ENCODE_MYINTEGER', 'constraint': ''}, 3: {'name': 'UPER_DECODE_MYINTEGER', 'constraint': ''}, 4: {'name': 'ACN_ENCODE_MYINTEGER', 'constraint': ''}, 5: {'name': 'ACN_DECODE_MYINTEGER', 'constraint': ''}, 6: {'name': 'T_INT32', 'constraint': ''}, 7: {'name': 'UPER_ENCODE_T_INT32', 'constraint': ''}, 8: {'name': 'UPER_DECODE_T_INT32', 'constraint': ''}, 9: {'name': 'ACN_ENCODE_T_INT32', 'constraint': ''}, 10: {'name': 'ACN_DECODE_T_INT32', 'constraint': ''}, 11: {'name': 'T_UINT32', 'constraint': ''}, 12: {'name': 'UPER_ENCODE_T_UINT32', 'constraint': ''}, 13: {'name': 'UPER_DECODE_T_UINT32', 'constraint': ''}, 14: {'name': 'ACN_ENCODE_T_UINT32', 'constraint': ''}, 15: {'name': 'ACN_DECODE_T_UINT32', 'constraint': ''}, 16: {'name': 'T_INT8', 'constraint': ''}, 17: {'name': 'UPER_ENCODE_T_INT8', 'constraint': ''}, 18: {'name': 'UPER_DECODE_T_INT8', 'constraint': ''}, 19: {'name': 'ACN_ENCODE_T_INT8', 'constraint': ''}, 20: {'name': 'ACN_DECODE_T_INT8', 'constraint': ''}, 21: {'name': 'T_UINT8', 'constraint': ''}, 22: {'name': 'UPER_ENCODE_T_UINT8', 'constraint': ''}, 23: {'name': 'UPER_DECODE_T_UINT8', 'constraint': ''}, 24: {'name': 'ACN_ENCODE_T_UINT8', 'constraint': ''}, 25: {'name': 'ACN_DECODE_T_UINT8', 'constraint': ''}, 26: {'name': 'T_BOOLEAN', 'constraint': ''}, 27: {'name': 'UPER_ENCODE_T_BOOLEAN', 'constraint': ''}, 28: {'name': 'UPER_DECODE_T_BOOLEAN', 'constraint': ''}, 29: {'name': 'ACN_ENCODE_T_BOOLEAN', 'constraint': ''}, 30: {'name': 'ACN_DECODE_T_BOOLEAN', 'constraint': ''}, 31: {'name': 'T_NULL_RECORD', 'constraint': ''}}
