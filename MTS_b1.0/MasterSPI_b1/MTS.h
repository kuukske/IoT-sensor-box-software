/*****************************************************************************
  Baris Öztürk 2018 Fontys Hogescholen

  This code is used for The Mobile Sensor Box combined with the Arduino environ
  ment. USI focuses on communication between master and slaves

  Environment might change overtime to a different hardware solution

*****************************************************************************/
#include <stdint.h>
#include <Arduino.h>

#include "system_config.h"

#ifndef MTS_h
#define MTS_h

class MTS
{
   public:
      int init(void);
      int requestAndRead(uint8_t slaven, 
                         uint8_t opCode, 
                         int expectedBytes,
                         uint8_t *InputBuffer_p);

  private:
      void request(uint8_t location, 
                   uint8_t opCode);
      int fillBuffer(uint8_t *Input_p, 
                     int expectedBytes);
      int identify(int slave);
};



#endif

