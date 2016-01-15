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
  
  if (_i2c_bus->status() != I2C_AVAILABLE)
  {
	  return TSL2561_BEGIN_FAIL;
  }
  
  /* Turn the device on and readback the register to make sure
   * that the device is there.
   */
  _i2c_bus->start_i2c();
  _i2c_bus->write(TSL2561_ADDRESS | I2C_WRITE);
  _i2c_bus->write(TSL2561_COMMAND | TSL2561_CONTROL);
  _i2c_bus->write(TSL2561_POWER_ON);
  _i2c_bus->start_i2c();
  _i2c_bus->write(TSL2561_ADDRESS | I2C_READ);

  value = _i2c_bus->read(&error,I2C_NACK);

  _i2c_bus->stop_i2c();  

  if (value != TSL2561_POWER_ON)
  {
	  return TSL2561_POWER_ON_FAIL;
  }
  
  if (_i2c_bus->status() != I2C_AVAILABLE)
  {
	  return TSL2561_BEGIN_FAIL;
  }
  
  _i2c_bus->start_i2c();
  _i2c_bus->write(TSL2561_ADDRESS | I2C_WRITE);
  _i2c_bus->write(TSL2561_COMMAND | TSL2561_INTERRUPT);
  _i2c_bus->write(TSL2561_INTERRUPT_OFF);
  _i2c_bus->stop_i2c();    
  
  _i2c_bus->start_i2c();
  _i2c_bus->write(TSL2561_ADDRESS | I2C_WRITE);
  _i2c_bus->write(TSL2561_COMMAND | TSL2561_TIMING);
  _i2c_bus->write(0x00);
  _i2c_bus->stop_i2c();    
  
  return TSL2561_STARTUP_OK;
}

char TSL2561::run()
{
  char value = 0;
  char error;	
	
  _i2c_bus->start_i2c();
  _i2c_bus->write(TSL2561_ADDRESS | I2C_WRITE);
  _i2c_bus->write(TSL2561_COMMAND | TSL2561_DATA_0_LOW);
  _i2c_bus->start_i2c();
  _i2c_bus->write(TSL2561_ADDRESS | I2C_READ);

  value = _i2c_bus->read(&error,I2C_ACK);
  whiteLight = value;
  value = _i2c_bus->read(&error,I2C_ACK);
  whiteLight += (value << 8);
  
  value = _i2c_bus->read(&error,I2C_ACK);
  irLight = value;
  value = _i2c_bus->read(&error,I2C_NACK);
  irLight += (value << 8);
  
  return _i2c_bus->stop_i2c(); 
}

unsigned int TSL2561::getWhiteLight()
{
  return whiteLight;
}

unsigned int TSL2561::getIRLight()
{
  return irLight;
}
