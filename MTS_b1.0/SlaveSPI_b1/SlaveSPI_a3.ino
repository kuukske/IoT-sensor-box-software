#include "slave_config.h"

uint8_t ID[ID_length] = "ABC";                  // type of sensor, should hold ID
uint8_t sname[name_length] = "KY-002";             // Name of the sensor
uint8_t unit[unit_length] = "Volt";              // Unit type (eg: degrees,amps,volts,etc)
uint8_t range[range_length] = {0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF};            // Range operation of units
uint8_t com_prot[prot_length] = {5,0,5};          // Sensor to slave protocol (SPI,I2C,A,D)

//uint8_t data[data_length];            //max buffer size to store data in
  
uint8_t data[50]
{
  54, 23,  61,  16,  47,
  93, 71,  11,  31,  78,
  33, 64,  17,  8,   43,
  54, 28,  67,  76,  33,
  81, 98,  77,  38,  61,
  4,  24,  17,  67,  28,
  4,  87,  22,  60,  55,
  20, 43,  2,   75,  36,
  5,  18,  38,  2,   17,
  90, 86,  96,  4,   55,
};

void setup (void)
{
  Serial.begin (9600);   // debugging
  Serial.println("Starting Initialization");
  
  // have to send on master in, *slave out*
  pinMode(MISO,OUTPUT);
  pinMode(MOSI,INPUT);
  pinMode(SS,INPUT);
  pinMode(SCK,INPUT);

  /* Enable SPI, Slave,set clock rate fck/16 */
  SPCR = 0;
  SPCR = (1<<SPE);
}  // end of setup


uint8_t buff[100];
void loop (void)
{
  int i = 0;
  LoadFromBuffer(1);
  SPDR = 0;
  switch(buff[0]) {
    
    case 0b00000000:
    /*RESERVED AND CRITICAL*/
    break;
    
    case 0b00000001:
    Serial.println("Sending recognition token"); 
    LoadIntoBuffer(0b10101010);
    break;/* Send Recogognition */

    case 0b00000010:
    Serial.println("Sending ID");
    for(int i = 0; i < sizeof(ID); i++)
    {
      LoadIntoBuffer(ID[i]);
      LoadFromBuffer(1);
    }
    break;/* Send ID */

    case 0b00000011:
    Serial.println("Sending sensor name");
    for(int i = 0; i < sizeof(sname); i++)
    {
      LoadIntoBuffer(sname[i]);
      LoadFromBuffer(1);
    }
    break;/* Send sensor name */
    
    case 0b00000100:
    Serial.println("Sending sensor unit");
    for(int i = 0; i < sizeof(unit); i++)
    {
      LoadIntoBuffer(unit[i]);
      LoadFromBuffer(1);
    }
    break;/* Send sensor unit */

    case 0b00000101:
    Serial.println("Sending sensor range");
    for(int i = 0; i < sizeof(range); i++)
    {
      LoadIntoBuffer(range[i]);
      LoadFromBuffer(1);
    }
    break;/* Send sensor range */

    case 0b00000110:
    Serial.println("Sending sensor protocol");
    for(int i = 0; i < sizeof(com_prot); i++)
    {
      LoadIntoBuffer(com_prot[i]);
      LoadFromBuffer(1);
    }
    break;/* Send sensor protocol */

    case 0b00000111:
    Serial.println("Acquiring data");
    break;/* Acquire sensor data */

    case 0b00001000:
    Serial.println("Sending data");
     for(int i = 0; i < sizeof(data); i++)
    {
      LoadIntoBuffer(data[i]);
      LoadFromBuffer(1);
    }
    break;/* Send sensor data */

    case 0b00100111:

    break; /* Send data */
 
    default:/* Unsupported opCode */
    Serial.print("Unrecognized opCode :");
    Serial.println(buff[0],BIN);
  }
} 

void LoadFromBuffer(int expectedBytes)
{
  uint8_t rv;
  int i = 0;
  do
  {
    rv = SPI_SlaveReceive();
    buff[i++] = rv;
  }while(i != expectedBytes);
}

uint8_t SPI_SlaveReceive(void)
{
/* Wait for reception complete */
while(!(SPSR & (1<<SPIF)));
/* Return Data Register */
return SPDR;
}

void LoadIntoBuffer(char cData)
{
/* Start transmission */
SPDR = cData;
/* Wait for transmission complete */
while(!(SPSR & (1<<SPIF)));
}
