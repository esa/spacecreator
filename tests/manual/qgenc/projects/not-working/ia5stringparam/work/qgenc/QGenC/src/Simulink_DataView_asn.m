MyInteger = Simulink.AliasType;
MyInteger.BaseType = 'uint16';
MyInteger.Description = 'range is (0, 10000)';

MyIA5String_member_data=Simulink.BusElement;
MyIA5String_member_data.Name='element_data';
MyIA5String_member_data.DataType='uint8';
MyIA5String_member_data.Dimensions=100;

MyIA5String_member_length=Simulink.BusElement;
MyIA5String_member_length.Name='length';
MyIA5String_member_length.DataType='int32';
MyIA5String_member_length.Dimensions=1;

MyIA5String=Simulink.Bus;
MyIA5String.Elements = [MyIA5String_member_data MyIA5String_member_length];

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

