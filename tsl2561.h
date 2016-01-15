
#ifndef TSL2561_H
#define TSL2561_H

#include <software_i2c.h>

#define TSL2561_ADDRESS 0x72 /* pre shifted */

#define TSL2561_COMMAND           0x80
#define TSL2561_INTERRUPT_CLEAR   0x40
#define TSL2561_WORD_TRANSACTION  0x20
#define TSL2561_BLOCK_TRANSACTION 0x10

#define TSL2561_CONTROL                  0x00
#define TSL2561_TIMING                   0x01
#define TSL2561_THRESHOLD_LOW_LOW_BYTE   0x02
#define TSL2561_THRESHOLD_LOW_HIGH_BYTE  0x03
#define TSL2561_THRESHOLD_HIGH_LOW_BYTE  0x04
#define TSL2561_THRESHOLD_HIGH_HIGH_BYTE 0x05
#define TSL2561_INTERRUPT                0x06
#define TSL2561_ID                       0x0A
#define TSL2561_DATA_0_LOW               0x0C
#define TSL2561_DATA_0_HIGH              0x0D
#define TSL2561_DATA_1_LOW               0x0E
#define TSL2561_DATA_1_HIGH              0x0F

#define TSL2561_STARTUP_OK 0x00
#define TSL2561_BEGIN_FAIL 0x70
#define TSL2561_POWER_ON_FAIL 0x71

#define TSL2561_POWER_ON  0x03
#define TSL2561_POWER_OFF 0x00

#define TSL2561_INTERRUPT_OFF 0x00
#define TSL2561_INTERRUPT_ON  0x10


class TSL2561
{
public:
  TSL2561(Software_I2C* i2c_bus);
  char begin();
  char run();
  
  unsigned int getWhiteLight();
  unsigned int getIRLight();

private:

  Software_I2C* _i2c_bus;
  
  unsigned int whiteLight;
  unsigned int irLight;

};


#endif