/*****************************************************************************
  Baris Öztürk 2018,April Fontys Hogescholen

 Configuration file for the Mobile Sensor Box environment

*****************************************************************************/
#ifndef systemconfig_h
#define systemconfig_h

/*************************************
  Variables for system configuration
**************************************/
#define ID_length 3        //ID Length is predefined and library sensetive [Bytes]
#define name_length 6       //Length of name in bytes (ASCII based)         [Bytes]
#define unit_length 4       //Length of the unit header                     [Bytes]
#define range_length 8      //Length of the range header                    [Bytes]
#define prot_length 3       //Length of the protocol header                 [Bytes]

#define data_length 4       //expected number of data                [Bytes]

#define debug_f             //Enable this flag for MTS debugging    
        
#define serial_db Serial

/**************************************************
  Variables for interface between microcontrollers
***************************************************/
#define SPI_f
#define maxnSlaves 7
/*
  Variables for system configuration according to SPI interface, use 0
  if slavepin not in use
*/
#ifdef SPI_f
#define slave0Pin SS
#define slave1Pin 0
#define slave2Pin 0
#define slave3Pin 0
#define slave4Pin 0
#define slave5Pin 0 
#define slave6Pin 0
#define slave7Pin 0
#endif

#define MasterWaitTime 1 //[ms]
#define confirmationToken 0b10101010

#endif
