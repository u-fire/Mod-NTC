/*
   This allows you to run various functions in a command-line like interface.
   Enter:
   `config` to see the configuration of the device.
   Measure Temperature:
    temp
   Change beta of probe
    beta 3950
   Change the I2C address:
    i2c 0x0F
*/

#include <Arduino.h>
#include <Wire.h>
#include <Microfire_Mod-NTC.h>

Microfire::Mod_NTC::i2c ntc;

String buffer, cmd, p1, p2;
const int fw_compatible = 1;
const int hw_compatible = 2;

void config()
{
  ntc.update();
  Serial.println((String) "Microfire Mod-NTC Sensor: " + (ntc.connected() ? "connected" : "*disconnected*"));
  if (!ntc.connected())
    return;
  if ((ntc.fwVersion != fw_compatible) || (ntc.hwVersion != hw_compatible))
  {
    Serial.println("*This version of shell was designed for a different hardware revision or firmware version*");
  }
  Serial.print("β: "); Serial.println(ntc.beta);
  Serial.print("hardware:firmware version: ");
  Serial.print(ntc.hwVersion);
  Serial.print(":");
  Serial.println(ntc.fwVersion);
}

void beta()
{
  ntc.setBeta(p1.toFloat());
  Serial.print("β: "); Serial.println(ntc.beta);
}

void temperature()
{
  while (Serial.available() == 0)
  {
    ntc.measureTemp();
    switch (ntc.status)
    {
    case ntc.STATUS_NO_PROBE:
      Serial.println("Error: No probe connected");
      break;
    case ntc.STATUS_NO_ERROR:
      Serial.print(ntc.tempC, 2);
      Serial.println(" °C");
      Serial.print(ntc.tempF, 2);
      Serial.println(" °F");
      Serial.print(ntc.tempK, 2);
      Serial.println(" °K");
      Serial.print(ntc.resistance, 0);
      Serial.println(" Ohms");
      Serial.println();
      break;
    }
    delay(1000);
  }
}

void i2c()
{
  uint8_t i2c_address;
  char *p;

  if (p1.length())
  {
    i2c_address = strtoul(p1.c_str(), &p, 16);
    Serial.println(i2c_address);
    if (i2c_address == 0)
    {
      Serial.println("Error: malformed I2C address");
    }
    else if ((i2c_address <= 0x07) || (i2c_address > 0x7f))
    {
      Serial.println("Error: I2C address not in valid range");
    }
    else
    {
      ntc.setI2CAddress(i2c_address);
    }
  }
}

void help()
{
  Serial.println(F("List of available commands"));
  Serial.println(F("config -or- c : no parameters : Displays all configuration and system information."));
  Serial.println(F("i2c           : I2C_address   : Changes the module's I2C address."));
  Serial.println(F("temp -or- t   : no parameters : Starts a temperature measurement"));
  Serial.println(F("beta -or- b   : no parameters : Saves a new probe beta"));
}

void cmd_run()
{
  if ((cmd == "conf") || (cmd == "config") || (cmd == "c"))
    config();
  if ((cmd == "temp") || (cmd == "t"))
    temperature();
  if (cmd == "i2c")
    i2c();
  if ((cmd == "beta") || (cmd == "b"))
    beta();
  if ((cmd == "help") || (cmd == "h"))
    help();
}

void cli_process()
{
  while (Serial.available())
  {
    char c = Serial.read();

    switch (c)
    {
    case '\n':
      Serial.println();
      cmd = buffer.substring(0, buffer.indexOf(" ", 0));
      cmd.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p1 = buffer.substring(0, buffer.indexOf(" ", 0));
      p1.trim();
      buffer.remove(0, buffer.indexOf(" ", 0));
      buffer.trim();
      p2 = buffer.substring(0, buffer.indexOf(" ", 0));
      p2.trim();
      cmd_run();

      Serial.print("> ");
      buffer = "";
      break;

    case '\b': // backspace
      buffer.remove(buffer.length() - 1);
      Serial.print("\b \b");
      break;

    default: // everything else
      buffer += c;
      Serial.print(c);
    }
  }
}

void setup()
{
  Wire.begin();
  ntc.begin();
  Serial.begin(9600);
  Serial.println("Type `help` for a list of commands");
  Serial.println("Type `t` to take a temperature measurement");
  config();
  Serial.print("> ");
}

void loop()
{
  cli_process();
}
