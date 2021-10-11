mtype {
    UNNAMED_A,
    UNNAMED_B,
}
mtype :states {
    STATE_A,
    STATE_B,
    STATE_C,
}
typedef basic_types {
    bit field_one;
    bool field_two;
    byte field_three;
    pid field_four;
    short field_five;
    int field_six;
    chan field_seven;
}

typedef unsigned_types {
    unsigned field_one:4;
    unsigned field_two:8;
}

typedef array_types {
    bit field_bit[2];
    bool field_bool[3];
    byte field_byte[4];
    pid field_pid[5];
    short field_short[6];
    int field_int[7];
    chan field_chan[8];
}

typedef utype_refs {
    array_types field_one;
    basic_types field_two[2];
}

typedef mtype_refs {
    mtype field_one;
    mtype : states field_two;
    mtype field_three[2];
    mtype : states field_four[2];
}

