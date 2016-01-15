#include <tsl2561.h>
#include <Arduino.h>
#include <software_i2c.h>

#define SAMPLE_DELAY 8 /* ms */

/* Test macro uncomment the 'x' to run the code in test mode. It will use the 
   default values from the datasheet instead of the ones from the device. This
   can also be used to see how this code works if you do not have an BMP180 or
   compatible device.
 */
#define TEST(x) x


TSL2561::TSL2561(Software_I2C* i2c_bus)
{
  _i2c_bus = i2c_bus;
  
  whiteLight = 0;
  irLight = 0;
}

char TSL2561::begin()
{
  char value = 0;
  char error;
  
  Serial.println("I2C ID");
  Serial.println(  _i2c_bus->status(),HEX);
  Serial.println(  _i2c_bus->start_i2c(),HEX);
  Serial.println(  _i2c_bus->write(TSL2561_ADDRESS | I2C_WRITE),HEX);
  Serial.println(  _i2c_bus->write(TSL2561_COMMAND | TSL2561_CONTROL),HEX);
  Serial.println(  _i2c_bus->write(0x02),HEX);
  Serial.println(  _i2c_bus->start_i2c(),HEX);
  Serial.println(  _i2c_bus->write(TSL2561_ADDRESS | I2C_READ),HEX);
  value =          _i2c_bus->read(&error,1);
  
  Serial.println(error,HEX);
  
  Serial.println(  _i2c_bus->stop_i2c(),HEX);  
  
  
  return value;
}

char TSL2561::run()
{
	return 0;
}

unsigned int TSL2561::getWhiteLight()
{
  return whiteLight;
}

unsigned int TSL2561::getIRLight()
{
  return irLight;
}
