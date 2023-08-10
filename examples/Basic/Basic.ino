/*!
   microfire.co for links to documentation, examples, and libraries
   github.com/u-fire for feature requests, bug reports, and  questions
   questions@microfire.co to get in touch with someone

   Mod-NTC hardware version 2, firmware 1
*/

#include <Microfire_Mod-NTC.h>
Microfire::Mod_NTC::i2c ntc;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  // start the sensor after Wire.begin()
  ntc.begin();
}

void loop()
{
  // take a measurement
  ntc.measureTemp();

  // display the results
  Serial.print(ntc.tempC, 2);
  Serial.println(" °C");
  Serial.print(ntc.tempF, 2);
  Serial.println(" °F");
  Serial.print(ntc.tempK, 2);
  Serial.println(" °K");
  Serial.print(ntc.resistance, 0);
  Serial.println(" Ohms");
  Serial.println();

  delay(1000);
} 