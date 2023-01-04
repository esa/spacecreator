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


class MySeq(COMMON):
    # Ordered list of fields:
    children_ordered = ['inputdata', 'outputdata', 'validity']

    def __init__(self, ptr=None):
        super(MySeq, self).__init__("MySeq", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append("{")
        lines.append("inputdata ")
        lines.append(" "+str(self.inputdata.Get()))
        lines.append(', ')
        lines.append("outputdata ")
        lines.append(" "+str(self.outputdata.Get()))
        lines.append(', ')
        lines.append("validity ")
        lines.append(" "+{'0': 'valid', '1': 'invalid'}[str(self.validity.Get())])
        lines.append("}")

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class MyChoice(COMMON):
    def __init__(self, ptr=None):
        super(MyChoice, self).__init__("MyChoice", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        if self.kind.Get() == DV.MyChoice_a_PRESENT:
         lines.append("a: ")
         lines.append(" "+str(self.a.Get()!=0).upper())
        if self.kind.Get() == DV.MyChoice_b_PRESENT:
         lines.append("b: ")
         lines.append("{")
         lines.append(" inputdata ")
         lines.append("  "+str(self.b.inputdata.Get()))
         lines.append(', ')
         lines.append(" outputdata ")
         lines.append("  "+str(self.b.outputdata.Get()))
         lines.append(', ')
         lines.append(" validity ")
         lines.append("  "+{'0': 'valid', '1': 'invalid'}[str(self.b.validity.Get())])
         lines.append("}")

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class MyEnum(COMMON):
    # Allowed enumerants:
    hello = 0
    world = 1
    howareyou = 2
    allowed = [hello, world, howareyou]
    def __init__(self, ptr=None):
        super(MyEnum, self).__init__("MyEnum", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'hello', '1': 'world', '2': 'howareyou'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


class MySeqOf(COMMON):
    def __init__(self, ptr=None):
        super(MySeqOf, self).__init__("MySeqOf", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append("{")
        def emitElem(path, i):
            state = path.GetState()
            if i > 0:
                lines.append(",")
            lines.append(" "+{'0': 'hello', '1': 'world', '2': 'howareyou'}[str(path[i].Get())])
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


class PID(COMMON):
    # Allowed enumerants:
    function_1 = 0
    function_2 = 1
    env = 2
    allowed = [function_1, function_2, env]
    def __init__(self, ptr=None):
        super(PID, self).__init__("PID", ptr)

    def GSER(self):
        ''' Return the GSER representation of the value '''
        lines = []
        lines.append(""+{'0': 'function-1', '1': 'function-2', '2': 'env'}[str(self.Get())])

        return ' '.join(lines)

    def PrintAll(self):
        ''' Display a variable of this type '''
        print(self.GSER() + '\n')


