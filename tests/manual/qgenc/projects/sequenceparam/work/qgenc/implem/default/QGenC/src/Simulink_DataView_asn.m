MyInteger = Simulink.AliasType;
MyInteger.BaseType = 'uint16';
MyInteger.Description = 'range is (0, 10000)';

MyReal = Simulink.AliasType;
MyReal.BaseType = 'double';
MyReal.Description = 'range is (0.0, 1000.0)';

MyBool = Simulink.AliasType;
MyBool.BaseType = 'boolean';
MyBool.Description = 'A simple BOOLEAN';

MySeq_elem01=Simulink.BusElement;
MySeq_elem01.Name='intSeqElement';
MySeq_elem01.DataType='uint16';
MySeq_elem01.Dimensions=1;

MySeq_elem02=Simulink.BusElement;
MySeq_elem02.Name='realSeqElement';
MySeq_elem02.DataType='double';
MySeq_elem02.Dimensions=1;

MySeq_elem03=Simulink.BusElement;
MySeq_elem03.Name='boolSeqElement';
MySeq_elem03.DataType='boolean';
MySeq_elem03.Dimensions=1;

MySeq = Simulink.Bus;
MySeq.Elements = [MySeq_elem01 MySeq_elem02 MySeq_elem03 ];

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

