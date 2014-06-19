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
uint8_t door_state, check_state;

uint8_t lock_door[] = {0xbd, 0x04, 0x0c, 0xe7, 0x03, 0x13};
uint8_t unlock_door[] = {0xbd, 0x04, 0x09, 0xe7, 0x05, 0x10};
uint8_t status_door[] = {0xbd, 0x04, 0x07, 0xe7, 0x07, 0x1c};
uint8_t message[6];

void setup()
{
 Serial.begin(9600);
 Serial1.begin(9600);
 time = 0;
 door_state = 0;
 check_state = 0;
}

void loop()
{
  uint8_t input, loop, i;
  loop = 0;
  //  sequence = 0x0c;
  if(check_state) {
    check_state = 0;
      status_door[2] = sequence;
      status_door[5] = 0xff;
      for(i = 1; i < 5; i++) {
	status_door[5] ^= status_door[i];
      }
      for(i = 0 ; i < 6; i++) {
	//Serial.print(lock_door[i], HEX);
	//      Serial.print(":");
	Serial1.write(status_door[i]);
      }
  } else {
    check_state = 1;
    /* if(!door_state) { */
    /*   door_state = 1;//fix */
    /*   lock_door[2] = sequence; */
    /*   lock_door[5] = 0xff; */
    /*   for(i = 1; i < 5; i++) { */
    /* 	lock_door[5] ^= lock_door[i]; */
    /*   } */
    /*   for(i = 0 ; i < 6; i++) { */
    /* 	//Serial.print(lock_door[i], HEX); */
    /* 	//      Serial.print(":"); */
    /* 	Serial1.write(lock_door[i]); */
    /*   } */
    /*   Serial.print("\r\n"); */
    /* } else { */
    /*   door_state = 0; */
    /*   unlock_door[2] = sequence; */
    /*   unlock_door[5] = 0xff; */
    /*   for(i = 1; i < 5; i++) { */
    /* 	unlock_door[5] ^= unlock_door[i]; */
    /*   } */
    /*   for(i = 0 ; i < 6; i++) { */
    /* 	Serial1.write(unlock_door[i]); */
    /*   } */
    /* } */
  }
  sequence++;
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
  delay(100);
}


