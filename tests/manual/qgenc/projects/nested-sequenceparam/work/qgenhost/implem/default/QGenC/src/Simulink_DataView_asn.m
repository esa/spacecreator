MyInteger = Simulink.AliasType;
MyInteger.BaseType = 'uint16';
MyInteger.Description = 'range is (0, 10000)';

MyReal = Simulink.AliasType;
MyReal.BaseType = 'double';
MyReal.Description = 'range is (0.0, 1000.0)';

MyBool = Simulink.AliasType;
MyBool.BaseType = 'boolean';
MyBool.Description = 'A simple BOOLEAN';

MySeqSimple_elem01=Simulink.BusElement;
MySeqSimple_elem01.Name='intSeqElement';
MySeqSimple_elem01.DataType='uint16';
MySeqSimple_elem01.Dimensions=1;

MySeqSimple_elem02=Simulink.BusElement;
MySeqSimple_elem02.Name='realSeqElement';
MySeqSimple_elem02.DataType='double';
MySeqSimple_elem02.Dimensions=1;

MySeqSimple_elem03=Simulink.BusElement;
MySeqSimple_elem03.Name='boolSeqElement';
MySeqSimple_elem03.DataType='boolean';
MySeqSimple_elem03.Dimensions=1;

MySeqSimple = Simulink.Bus;
MySeqSimple.Elements = [MySeqSimple_elem01 MySeqSimple_elem02 MySeqSimple_elem03 ];

MySeqComplex_elem01=Simulink.BusElement;
MySeqComplex_elem01.Name='simpleSeq1';
MySeqComplex_elem01.DataType='MySeqSimple';
MySeqComplex_elem01.Dimensions=1;

MySeqComplex_elem02=Simulink.BusElement;
MySeqComplex_elem02.Name='simpleSeq2';
MySeqComplex_elem02.DataType='MySeqSimple';
MySeqComplex_elem02.Dimensions=1;

MySeqComplex = Simulink.Bus;
MySeqComplex.Elements = [MySeqComplex_elem01 MySeqComplex_elem02 ];

T_Int32 = Simulink.AliasType;
T_Int32.BaseType = 'int32';
T_Int32.Description = 'range is (-2147483648, 2147483647)';

T_UInt32 = Simulink.AliasType;
T_UInt32.BaseType = 'uint32';
T_UInt32.Description = 'range is (0, 4294967295)';

T_Int8 = Simulink.AliasType;
T_Int8.BaseType = 'int8';
T_Int8.Description = 'range is (-128, 127)';

T_UInt8 = Simulink.AliasType;
T_UInt8.BaseType = 'uint8';
T_UInt8.Description = 'range is (0, 255)';

T_Boolean = Simulink.AliasType;
T_Boolean.BaseType = 'boolean';
T_Boolean.Description = 'A simple BOOLEAN';

