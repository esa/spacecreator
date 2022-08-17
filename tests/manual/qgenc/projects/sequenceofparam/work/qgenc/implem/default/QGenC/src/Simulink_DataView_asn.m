MyInteger = Simulink.AliasType;
MyInteger.BaseType = 'uint16';
MyInteger.Description = 'range is (0, 10000)';

MySeqOf_member_data=Simulink.BusElement;
MySeqOf_member_data.Name='element_data';
MySeqOf_member_data.DataType='uint16';
MySeqOf_member_data.Dimensions=3;

MySeqOf=Simulink.Bus;
MySeqOf.Elements = [MySeqOf_member_data ];

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

