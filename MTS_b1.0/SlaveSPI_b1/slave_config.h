#ifndef systemconfig_h
#define systemconfig_h

/*************************************
  Variables for slave
**************************************/
#define ID_length 3         //ID Length is predefined and library sensetive [Bytes]
#define name_length 6      //Length of na me in bytes (ASCII based)         [Bytes]
#define unit_length 4       //Lensgth of the unit header                     [Bytes]
#define range_length 8      //Length of the range header                    [Bytes]
#define prot_length 3       //Length of the protocol header                 [Bytes]

#define data_length 4       //Data size                [Bytes]

#define confirmationToken 0b10101010
#endif
