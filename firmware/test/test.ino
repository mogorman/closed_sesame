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

uint32_t time;
uint8_t sequence, sequence_check_code;
uint8_t door_state, check_state;

uint8_t lock[] =        {0xbd, 0x04, 0x0c, 0xe7, 0x03, 0x13};
uint8_t unlock[] =      {0xbd, 0x04, 0x09, 0xe7, 0x05, 0x10};
uint8_t status[] =      {0xbd, 0x04, 0x07, 0xe7, 0x07, 0x1c};
uint8_t bat_status[] =  {0xbd, 0x04, 0x07, 0xe7, 0x09, 0x1c};
uint8_t unpair[] =      {0xbd, 0x04, 0x03, 0xe7, 0x0e, 0x11};
uint8_t add_code[] =    {0xbd, 0x0a, 0x0f, 0xe7, 0x0b, 0x00, 0x01, 0x12, 0x34, 0x56, 0x78, 0x1f};
uint8_t delete_code[] = {0xbd, 0x06, 0x1c, 0xe7, 0x0d, 0x00, 0x01, 0x0c};
uint8_t check_code[]=   {0xbd, 0x06, 0x10, 0xe7, 0x0c, 0x00, 0x01, 0x03};

uint8_t pair_0[] =    {0xbd, 0x04, 0x01, 0xe7, 0x18, 0x05};
uint8_t pair_1[] =   {0xbd, 0x04, 0x08, 0xe7, 0x09, 0x1d};
uint8_t pair_2[] =   {0xbd, 0x0b, 0x09, 0xe7, 0x0f, 0x14, 0x06, 0x21, 0x03, 0x33, 0x52, 0x06, 0x42};
uint8_t pair_3[] =   {0xbd, 0x04, 0x0a, 0xe7, 0x10, 0x06};
uint8_t pair_4[] =   {0xbd, 0x0b, 0x0b, 0xe7, 0x0f, 0x14, 0x06, 0x20, 0x20, 0x33, 0x54, 0x05, 0x67}; 


void check_code_message(uint8_t slot)
{
  check_code[6] = slot;
  send_message(check_code);
}

void delete_code_message(uint8_t slot)
{
  check_code[6] = slot;
  send_message(delete_code);
}

void set_code_message(uint8_t *code,uint8_t slot)
{
  add_code[6b] = slot;
  send_message(add_code);
}

void send_message(uint8_t *message)
{
  uint8_t i;
  uint8_t length = message[1] + 2;
      message[2] = sequence;
      message[length - 1] = 0xff;
      for(i = 1; i < length - 1; i++) {
  	message[length -1] ^= message[i];
      }
      Serial.print(" sending ");
      for(i = 0 ; i < length; i++) {
	Serial.print("0x");
	if(message[i] < 16) Serial.print("0");
	Serial.print(message[i], HEX);
	Serial.print(" ");
  	Serial1.write(message[i]);
      }
      Serial.println("");
      sequence++;
}

void setup()
{
 Serial.begin(9600);
 Serial1.begin(9600);
 time = 0;
 door_state = 0;
 check_state = 0;
 sequence = 0;
 sequence_check_code = 0;
}

void loop()
{
  uint8_t input, loop, i;
  loop = 0;

  while (Serial.available()) {
    input = Serial.read();
    switch(input) {
    case 'u':
      Serial.print("unlock");
      send_message(unlock);
      break;
    case 'l':
      Serial.print("lock");
      send_message(lock);
      break;
    case 's':
      Serial.print("status");
      send_message(status);
      break;
    case 'S':
      Serial.print("battery status");
      send_message(bat_status);
      break;
    case 'p':
      Serial.print("unpair");
      send_message(unpair);
      break;
    case 'c':
      Serial.print("check code");
      check_code_message(sequence_check_code);
      sequence_check_code++;
      break;
    case '0':
      Serial.print("pair_0");
      send_message(pair_0);
      break;
    case '1':
      Serial.print("pair_1");
      send_message(pair_1);
      break;
    case '2':
      Serial.print("pair_2");
      send_message(pair_2);
      break;
    case '3':
      Serial.print("pair_3");
      send_message(pair_3);
      break;
    case '4':
      Serial.print("pair_4");
      send_message(pair_4);
      break;
    default:
      break;
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
