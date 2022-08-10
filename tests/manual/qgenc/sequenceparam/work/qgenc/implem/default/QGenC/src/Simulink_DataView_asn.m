MyInteger = Simulink.AliasType;
MyInteger.BaseType = 'uint16';
MyInteger.Description = 'range is (0, 10000)';

% Values for MySeq-validity:
MySeq_validity_value_valid = 0;
MySeq_validity_value_invalid = 1;
MySeq_validity = Simulink.AliasType;
MySeq_validity.BaseType = 'int32';
MySeq_validity.Description = 'values of ENUMERATED MySeq-validity';


MySeq_elem01=Simulink.BusElement;
MySeq_elem01.Name='input_data';
MySeq_elem01.DataType='uint16';
MySeq_elem01.Dimensions=1;

MySeq_elem02=Simulink.BusElement;
MySeq_elem02.Name='output_data';
MySeq_elem02.DataType='uint16';
MySeq_elem02.Dimensions=1;

MySeq_elem03=Simulink.BusElement;
MySeq_elem03.Name='validity';
MySeq_elem03.DataType='int32';
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

