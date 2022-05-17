from functools import partial

import DV

from Stubs import (
    myassert, Clean, DataStream, COMMON)

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


