/*
 *
 * Battery discharge formula
 * equivalent_current_mA = (percent_monthly_discharge_rate / 100) * capacity_maH / (24 * 30) 
 */
#include <Wire.h>
#include <SPI.h>
#include <EEPROM.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>
#include <util/atomic.>

uint32_t time;
uint8_t sequence;
uint8_t door_state;

uint8_t lock_door[] = {0xbd, 0x04, 0x0c, 0xe7, 0x03, 0x13};
uint8_t unlock_door[] = {0xbd, 0x04, 0x09, 0xe7, 0x05, 0x10};


void setup()
{
 Serial.begin(9600);
 Serial1.begin(9600);
 time = 0;
 door_state = 0;
}

void loop()
{
  uint8_t input, loop;
  loop = 0;
//  Serial.println("hello there");
//  Serial1.println("you there");
    // read from port 0, send to port 1:
  //    Serial1.println("hello world");
  // read from port 1, send to port 0:
  if(!door_state) {
    door_state = 1;
    for(int i = 0 ; i < 6; i++) {
      if(i == 2) {
	Serial1.write(sequence);
      } else {
      Serial1.write(lock_door[i]);
      }
    }
    sequence++;
  } else {
    door_state = 0;
    for(int i = 0 ; i < 6; i++) {
      if(i == 2) {
	Serial1.write(sequence);
      } else {
      Serial1.write(unlock_door[i]);
      }
    }
  }
    while (Serial1.available()) {
      if (!loop) {
	Serial.print(time);
	Serial.print(":");
        Serial.print(sequence, HEX);
	Serial.print(":\t");
      }
      input = Serial1.read();
      if(input < 16) Serial.print("0");
      Serial.print(input, HEX);
      Serial.print(":");
      loop = 1;
  }
    if (loop) {
      Serial.println("");
    }
    time++;
  delay(1000);
}


