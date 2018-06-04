/*****************************************************************************
  Baris Öztürk 2018,June Fontys Hogescholen

  This code is used for spi combined with the Arduino environment. Later to be
  used in The Mobile Sensor Box

*****************************************************************************/
#include "system_config.h"
#include "MTS.h"
#include "OpCodes.h"

struct slave_struct
{
  bool ignoreToken = false;

  uint8_t ID[ID_length];                   // type of sensor, should hold ID
  uint8_t sname[name_length];             // Name of the sensor
  uint8_t unit[unit_length];              // Unit type (eg: degrees,amps,volts,etc)
  uint8_t range[range_length];            // Range operation of units
  uint8_t com_prot[prot_length];          // Sensor to slave protocol (spi,I2C,A,D)

  uint8_t data[data_length];            //max buffer size to store data in
};
slave_struct slave[maxnSlaves];

MTS spi;
void setup() {
  #ifdef debug_f
  serial_db.begin(9600);;
  serial_db.println("Starting Initialization");
  #endif
  
  spi.init();
  InitializeSlaves();
  
}



void loop() {


  
}


/***************************************************************************
  Function description : 
  Requests fixed parameters from all available slaves if possible

  returns: nothing
****************************************************************************/
void InitializeSlaves(void)
{
  uint8_t buff[1];
  int i;
  
  for(i = 0; i < maxnSlaves; i++) /* Determine if slaves are present */
  {
    #ifdef debug_f
    serial_db.println("-----------------------------");
    serial_db.println("   Checking slave presence   ");
    #endif
    
    spi.requestAndRead(i,opCode[0],1,buff);
    
    if(buff[0] == confirmationToken)
    {
      #ifdef debug_f
      serial_db.print("Slave ");
      serial_db.print(i);
      serial_db.println(" found");
      #endif
    }
    else
    {
      #ifdef debug_f
      serial_db.print("Ignoring slave:");
      serial_db.println(i);
      #endif

      slave[i].ignoreToken = true;
    }
  }

  for(i = 0; slave[i].ignoreToken == false && i < maxnSlaves; i++)
  {
    spi.requestAndRead(i,opCode[1],ID_length,slave[i].ID);
  }
    
  for(i = 0; slave[i].ignoreToken == false && i < maxnSlaves; i++)
  {
    spi.requestAndRead(i,opCode[2],name_length,slave[i].sname);
  }

  for(i = 0; slave[i].ignoreToken == false && i < maxnSlaves; i++)
  {
    spi.requestAndRead(i,opCode[3],unit_length,slave[i].unit);
  }

  for(i = 0; slave[i].ignoreToken == false && i < maxnSlaves; i++)
  {
    spi.requestAndRead(i,opCode[4],range_length,slave[i].range);
  }

  for(i = 0; slave[i].ignoreToken == false && i < maxnSlaves; i++)
  {
    spi.requestAndRead(i,opCode[5],prot_length,slave[i].com_prot);
  }
}

