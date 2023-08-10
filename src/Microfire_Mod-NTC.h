#ifndef MOD_NTC_H
#define MOD_NTC_H

#include <Arduino.h>
#include <Wire.h>

#define UFIRE_MOD_NTC 0x0c

#define MEASURE_TEMP_TASK 40   /*!< Command to measure temperature */
#define BETA_TASK 20           /*!< Command to calibrate the probe */
#define I2C_TASK 2             /*!< Command to change the i2c address */

#define HW_VERSION_REGISTER 0  /*!< hardware version register */
#define FW_VERSION_REGISTER 1  /*!< firmware version  register */
#define TASK_REGISTER 2        /*!< task register */
#define STATUS_REGISTER 3      /*!< status register */
#define BETA_REGISTER 4        /*!< Beta register */
#define TEMP_C_REGISTER 8      /*!< Temperature in C register */
#define TEMP_K_REGISTER 12     /*!< Temperature in K register */
#define TEMP_F_REGISTER 16     /*!< Temperature in F register */
#define RESISTANCE_REGISTER 20 /*!< resistance in Ohms register */
#define BUFFER_REGISTER 24     /*!< buffer register */

namespace Microfire
{
  namespace Mod_NTC
  {
    class i2c
    {
    public:
      const float &tempC = _tempC;
      const float &tempF = _tempF;
      const float &tempK = _tempK;
      const float &resistance = _resistance;
      const float &beta = _beta;

      const int &hwVersion = _hwVersion;
      const int &fwVersion = _fwVersion;
      const int &status = _status;

      bool begin(TwoWire &wirePort = Wire, uint8_t address = UFIRE_MOD_NTC);
      bool connected();
      float measureTemp(bool blocking = true);
      void reset();
      void setI2CAddress(uint8_t i2cAddress);
      float setBeta(float beta);
      void update();

      typedef enum Temp_Error_e
      {
        STATUS_NO_ERROR,
        STATUS_NO_PROBE,
        STATUS_SYSTEM_ERROR
      } Temp_Error_t;

    private:
      uint8_t _address;
      TwoWire *_i2cPort;
      static int _hwVersion;
      static int _fwVersion;
      static int _status;

      static float _tempC;
      static float _tempF;
      static float _tempK;
      static float _resistance;
      static float _beta;

      void _beta_calculation();
      void _SH_calculation();
      void _updateRegisters();
      void _send_command(uint8_t command);
      void _write_4_bytes(uint8_t reg, float f);
      void _write_byte(uint8_t reg, uint8_t val);
      float _read_4_bytes(uint8_t reg);
      uint8_t _read_byte(uint8_t reg);
    };
  } // namespace Mod_NTC
} // namespace Microfire
#endif // ifndef Mod_NTC_H