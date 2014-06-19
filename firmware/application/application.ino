/*
 *
 * Battery discharge formula
 * equivalent_current_mA = (percent_monthly_discharge_rate / 100) * capacity_maH / (24 * 30) 
 */
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>

void setup()
{
 Serial.begin(9600);
 Serial1.begin(9600);

}

void loop()
{
  Serial.println("hello there");
  Serial1.println("you there");

  delay(1000);
}


