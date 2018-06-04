/*****************************************************************************
  Baris Öztürk 2018 Fontys Hogescholen

  This code is used for The Mobile Sensor Box combined with the Arduino environ
  ment. USI focuses on communication between master and slaves

  Environment might change overtime to a different hardware solution

*****************************************************************************/
#include "MTS.h"

/***************************************************************************
  Function description : 
  Request amount of data from the corresponding adress slave -> I2C
  Command slave to do something -> SPI

  returns: nothing
****************************************************************************/
void MTS::request(uint8_t location, uint8_t opCode)
{
  #ifdef SPI_f

    #ifdef debug_f
    serial_db.println("-----------------------------");
    
    serial_db.print("Requesting from pin: ");
    serial_db.println(location);
    serial_db.print("OpCode :");
    serial_db.println(opCode,BIN);
    #endif
    
  /*enable slave*/
  digitalWrite(location, LOW);

  /* Put OpCode in SPI register */
  SPDR = opCode;
  /* Wait for transmission complete */
  while(!(SPSR & (1<<SPIF)));
  
  delay(MasterWaitTime);//wait for slave
  
  #endif
}

/***************************************************************************
  Function description : 
  Reads the buffer where sensor data gets stored in

  returns: nothing
****************************************************************************/
int MTS::fillBuffer(uint8_t *Input_p,int expectedBytes)
{
  int rv;

  #ifdef SPI_f
  uint8_t incomingData[expectedBytes];

  int i = 0;
  do
  {
    #ifdef debug_f
    serial_db.print("i = ");
    serial_db.println(i);
    #endif

    /* Start transmission */
    SPDR = 0b00000000;
    /* Wait for transmission complete */
    while(!(SPSR & (1<<SPIF)));

    //wait for slave
    delay(MasterWaitTime);
    
    #ifdef debug_f
    serial_db.print("SPDR HOLDS:");
    for(byte mask = 0x80; mask; mask >>= 1)
    {
    if(mask  & SPDR)
       Serial.print('1');
    else
       Serial.print('0');
    }
    //serial_db.print(SPDR,BIN);
    serial_db.print("  ");
    serial_db.print(SPDR,DEC);
    serial_db.print("  ");
    serial_db.println((char)SPDR);
    #endif
    
    incomingData[i] = SPDR;
    
    i++;
  }while(i != expectedBytes);

  
  memcpy(Input_p, incomingData, sizeof(incomingData)); // store SPI buffer

  #endif

  return rv;
}

/***************************************************************************
  Function description : 
  Selects the sensor adress according to the used interface

  returns:  -The adress of the corresponding sensor UPON SUCCES
            -0  if invalid slave has been chosen 
****************************************************************************/
int MTS::identify(int slave)
{ 
  #ifdef SPI_f
  int slaveSelect;

  if(slave < maxnSlaves && slave >= 0)
  {
      switch(slave) {

          case 0:
          slaveSelect = slave0Pin;
           break;
  
          case 1:
          slaveSelect = slave1Pin;
            break;
  
          case 2:
          slaveSelect = slave2Pin;
          break;

          case 3:
          slaveSelect = slave3Pin;
          break;

          case 4:
          slaveSelect = slave4Pin;
          break;

          case 5:
          slaveSelect = slave5Pin;
          break;

          case 6:
          slaveSelect = slave6Pin;
          break;

          case 7:
          slaveSelect = slave7Pin;
          break;
      }
      return slaveSelect;
  }
  return -1;
  #endif
}

/***************************************************************************
  Function description : 
  Initializes the communication protocol between master and slave(s)

  returns:  - 1 upon succes 
            - 0 when failed
****************************************************************************/
int MTS::init(void)
{
  int rv;

  #ifdef SPI_f

  /*Configure chip select pins*/
  pinMode(slave0Pin,OUTPUT);
  pinMode(slave1Pin,OUTPUT);
  pinMode(slave2Pin,OUTPUT);
  pinMode(slave3Pin,OUTPUT);
  pinMode(slave4Pin,OUTPUT);
  pinMode(slave5Pin,OUTPUT);
  pinMode(slave6Pin,OUTPUT);
  pinMode(slave7Pin,OUTPUT);

  digitalWrite(slave0Pin, HIGH);
  digitalWrite(slave1Pin, HIGH);
  digitalWrite(slave2Pin, HIGH);
  digitalWrite(slave3Pin, HIGH);
  digitalWrite(slave4Pin, HIGH);
  digitalWrite(slave5Pin, HIGH);
  digitalWrite(slave6Pin, HIGH);
  digitalWrite(slave7Pin, HIGH);

  /*Configure SPI pins*/
  pinMode(MISO,INPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(SS,OUTPUT);
  pinMode(SCK,OUTPUT);

  /* Enable SPI, Master, set clock rate fck/16 */
  SPCR = 0;
  SPCR = (1<<SPE)|(1<<MSTR);
  
  rv = 1;

  #ifdef debug_f
  serial_db.print("SPI control register holds:");
  serial_db.println(SPCR,BIN);
  #endif 
  
  #endif

  return rv;
}

/***************************************************************************
  Function description : 
  Stores n amount of bytes in the given pointer location with the chosen 
  opcode
  returns:  - 0 when failed
            - 1 when succeeded
****************************************************************************/
int MTS::requestAndRead(uint8_t slaven, uint8_t opCode, int expectedBytes, uint8_t *InputBuffer_p)
{
  int rv;
  
  #ifdef SPI_f
  int slavePin;

  /* Get slave pin as defined in system_config.h */
  slavePin = identify(slaven);
  
  if(slavePin < 0)
  {
    #ifdef debug_f
    serial_db.print("Accesing an invalid slave at pin");
    serial_db.println(slavePin);
    #endif
    
    return 0;
  }
  request(slavePin,opCode);

  
  fillBuffer(InputBuffer_p, expectedBytes);
  digitalWrite(slavePin,HIGH);

  rv = 1;
  return rv;

  #endif
}

