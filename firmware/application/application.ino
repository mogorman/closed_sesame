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

uint8_t lock_door[] =     {0xbd, 0x04, 0x0c, 0xe7, 0x03, 0x13};
uint8_t unlock_door[] =   {0xbd, 0x04, 0x09, 0xe7, 0x05, 0x10};
uint8_t status_door[] =   {0xbd, 0x04, 0x07, 0xe7, 0x07, 0x1c};
uint8_t first_message[] = {0xbd,0x05,0x02,0xaa,0x18,0x01,0x4b};


void send_message(uint8_t *message, int length)
{
  int i;
      message[2] = sequence;
      message[length - 1] = 0xff;
      for(i = 1; i < length - 1; i++) {
  	message[length -1] ^= message[i];
      }
      for(i = 0 ; i < length; i++) {
  	Serial1.write(message[i]);
      }
}

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
  /* if(check_state) { */
  /*   check_state = 0; */
  /*   send_message(status_door,6); */
  /* } else { */
  /*   check_state = 1; */
  /*   if(!door_state) { */
  /*     door_state = 1;//fix */
  /*     send_message(lock_door,6); */
  /*     Serial.print("\r\n"); */
  /*   } else { */
  /*     door_state = 0; */
  /*     send_message(unlock_door,6); */
  /*   } */
  /* } */
  /* sequence++; */
  while (Serial1.available()) {
    if (!loop) {
      Serial.print(time);
      Serial.print(":");
      Serial.print(sequence, HEX);
      Serial.print(":\t");
    }
    input = Serial1.read();
    Serial.print("0x");
    if(input < 16) Serial.print("0");
    Serial.print(input, HEX);
    Serial.print(" ");
    loop = 1;
  }
  if (loop) {
    Serial.println("");
  }
  time++;
  delay(1000);
}


