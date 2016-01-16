#include <tsl2561.h>
#include <software_i2c.h>

/* Test macro uncomment the 'x' to run the code in test mode. */
#define TEST(x) x

/* This chip seems to be pretty picky about the I2C bus behavior. Most I2C
 * devices that I have worked with are pretty fast and loose with the spec.
 * This device really made me pay attention to bus behaviors. This device
 * does not seem to respect the absoluteness of the i2c bus stop condition.
 * I observed that on a partial bus cycle ending with a stop condition the
 * device takes several tries to kick it back into gear.
 */
unsigned char TSL2561::writeData(unsigned char command, char* data, unsigned char elements)
{
  int i = 0;
  
  if (elements == 0)
	return TSL2561_COMM_FAIL;
  
  unsigned char tsl2561_command = TSL2561_COMMAND | command;
  
  if (elements > 1)
  {
	  tsl2561_command |= TSL2561_BLOCK_TRANSACTION;
  }
  
  _i2c_bus->start_i2c();
  _i2c_bus->write(_address | I2C_WRITE);
  
  _i2c_bus->write(tsl2561_command);
  
  for (i = 0; i < elements; i++)
  {
	  _i2c_bus->write(data[i]);
  }

  _i2c_bus->stop_i2c();

  return _i2c_bus->status();
}

unsigned char TSL2561::readData(unsigned char command, char* data, unsigned char elements)
{
  int i = 0;
  char error;
  if (elements == 0)
  {
	return TSL2561_COMM_FAIL;
  }
  
  unsigned char tsl2561_command = TSL2561_COMMAND | command;
  
  _i2c_bus->start_i2c();
  _i2c_bus->write(_address | I2C_WRITE);
  _i2c_bus->write(tsl2561_command);
  _i2c_bus->start_i2c();
  _i2c_bus->write(_address | I2C_READ);
  
  for (i = 0; i < elements - 1; i++)
  {
	data[i] = _i2c_bus->read(&error);
  }

  data[i] = _i2c_bus->read(&error, I2C_NACK);

  _i2c_bus->stop_i2c();

  return _i2c_bus->status();
}

TSL2561::TSL2561(Software_I2C* i2c_bus, unsigned char address)
{
  _i2c_bus = i2c_bus;
  _address = address << 1;
  whiteLight = 0;
  irLight = 0;
}

char TSL2561::begin()
{
  char value = 0;
  char data = 0;
  
  if (_i2c_bus->status() != I2C_AVAILABLE)
  {
	  return TSL2561_BEGIN_FAIL;
  }
  
  /* Turn the device on and read back the register to make sure
   * that the device is there.
   */
  data = TSL2561_POWER_ON;
  writeData(TSL2561_CONTROL, &data, 1);
  readData(TSL2561_CONTROL, &value, 1);
  
  if (value != TSL2561_POWER_ON)
  {
	  return TSL2561_POWER_ON_FAIL;
  }
  
  if (_i2c_bus->status() != I2C_AVAILABLE)
  {
	  return TSL2561_BEGIN_FAIL;
  }
  
  /* Since this is intended to be used in bright sunlight turn
   * off interrupts (not hooked up anyway) and set the shortest
   * integration parameters.
   */
  
  data = TSL2561_INTERRUPT_OFF;
  writeData(TSL2561_INTERRUPT, &data, 1);

  data = TSL2561_INTEGRATION_13P7MS | TSL2561_INTEGRATION_LOW_GAIN;
  writeData(TSL2561_TIMING, &data, 1);  
  
  return TSL2561_STARTUP_OK;
}

char TSL2561::run()
{
  unsigned int reportedValue;
  char value[4];
  
  readData(TSL2561_DATA_0_LOW, value, 4);

  reportedValue = value[0];
  reportedValue += (value[1] << 8);
  
  whiteLight = reportedValue;
  
  irLight = value[2];
  irLight += (value[3] << 8);
  
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
