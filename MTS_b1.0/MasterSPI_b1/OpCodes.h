/*****************************************************************************
  Baris Öztürk 2018,June Fontys Hogescholen

  This code is used for spi combined with the Arduino environment. Later to be
  used in The Mobile Sensor Box

*****************************************************************************/
#ifndef opcodes_h
#define opcodes_h

uint8_t opCode[]= {0b00000001,//send recognition token
                   0b00000010,//send sensor ID
                   0b00000011,//send sensor name 
                   0b00000100,//send sensor unit 
                   0b00000101,//send sensor range
                   0b00000110,//send sensor protocol
                   0b00000111,//tell slave to read sensor
                   0b00001000,//send sensor data
                   0b00001001,//reserved
                   0b00001010,//reserved
                   0b00001011};

#endif
