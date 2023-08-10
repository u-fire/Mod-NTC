#include "Microfire_Mod-NTC.h"

int Microfire::Mod_NTC::i2c::_hwVersion = -1;
int Microfire::Mod_NTC::i2c::_fwVersion = -1;
int Microfire::Mod_NTC::i2c::_status = -1;

float Microfire::Mod_NTC::i2c::_tempC = -1;
float Microfire::Mod_NTC::i2c::_tempF = -1;
float Microfire::Mod_NTC::i2c::_tempK = -1;
float Microfire::Mod_NTC::i2c::_resistance = -1;
float Microfire::Mod_NTC::i2c::_beta = -1;

namespace Microfire
{
  namespace Mod_NTC
  {
    // Initializes sensor
    bool i2c::begin(TwoWire &wirePort, uint8_t address)
    {
      _address = address;
      _i2cPort = &wirePort;

      return connected();
    }

    // Returns true or false if the sensor is connected.
    bool i2c::connected()
    {
      _i2cPort->beginTransmission(_address);
      uint8_t retval = _i2cPort->endTransmission();

      if (retval)
      {
        return false;
      }
      else
      {
        return true;
      }
    }

    // Takes a temperature measurement
    float i2c::measureTemp(bool blocking)
    {
      _send_command(MEASURE_TEMP_TASK);
      if (blocking)
      {
        delay(150);
      }

      _updateRegisters();

      return tempC;
    }

    // Change the I2C address.
    void i2c::reset()
    {
      setBeta(3977.0);
    }

    // Change the I2C address.
    void i2c::setI2CAddress(uint8_t i2cAddress)
    {
      _write_4_bytes(BUFFER_REGISTER, i2cAddress);
      _send_command(I2C_TASK);
      _address = i2cAddress;
    }

    // Change the beta.
    float i2c::setBeta(float beta)
    {
      _write_4_bytes(BETA_REGISTER, beta);
      _send_command(BETA_TASK);
      _updateRegisters();
      return _beta;
    }

    void i2c::update()
    {
      _updateRegisters();
    }

    void i2c::_updateRegisters()
    {
      _status = _read_byte(STATUS_REGISTER);
      _hwVersion = _read_byte(HW_VERSION_REGISTER);
      _fwVersion = _read_byte(FW_VERSION_REGISTER);
      _resistance = _read_4_bytes(RESISTANCE_REGISTER);
      _beta = _read_4_bytes(BETA_REGISTER);
      _tempC = _read_4_bytes(TEMP_C_REGISTER);
      _tempK = _read_4_bytes(TEMP_K_REGISTER);
      _tempF = _read_4_bytes(TEMP_F_REGISTER);
    }

    void i2c::_send_command(uint8_t command)
    {
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(TASK_REGISTER);
      _i2cPort->write(command);
      _i2cPort->endTransmission();
    }

    void i2c::_write_4_bytes(uint8_t reg, float f)
    {
      uint8_t b[5];
      float f_val = f;

      b[0] = reg;
      b[1] = *((uint8_t *)&f_val);
      b[2] = *((uint8_t *)&f_val + 1);
      b[3] = *((uint8_t *)&f_val + 2);
      b[4] = *((uint8_t *)&f_val + 3);
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(b, 5);
      _i2cPort->endTransmission();
    }

    float i2c::_read_4_bytes(uint8_t reg)
    {
      float retval;

      _i2cPort->beginTransmission(_address);
      _i2cPort->write(reg);
      _i2cPort->endTransmission();
      _i2cPort->requestFrom(_address, (uint8_t)4);
      *((uint8_t *)&retval) = _i2cPort->read();
      *((uint8_t *)&retval + 1) = _i2cPort->read();
      *((uint8_t *)&retval + 2) = _i2cPort->read();
      *((uint8_t *)&retval + 3) = _i2cPort->read();
      return retval;
    }

    void i2c::_write_byte(uint8_t reg, uint8_t val)
    {
      uint8_t b[5];

      b[0] = reg;
      b[1] = val;
      _i2cPort->beginTransmission(_address);
      _i2cPort->write(b, 2);
      _i2cPort->endTransmission();
    }

    uint8_t i2c::_read_byte(uint8_t reg)
    {
      uint8_t retval;

      _i2cPort->beginTransmission(_address);
      _i2cPort->write(reg);
      _i2cPort->endTransmission();
      _i2cPort->requestFrom(_address, (uint8_t)1);
      retval = _i2cPort->read();

      return retval;
    }
  } // namespace Mod_NTC
} // namespace Microfire