/*
 *
 * Battery discharge formula
 * equivalent_current_mA = (percent_monthly_discharge_rate / 100) * capacity_maH / (24 * 30) 
 */
//#include <Wire.h>
#include <RFduinoBLE.h>

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
  add_code[6] = slot;
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
      for(i = 0 ; i < length; i++) {
  	Serial.write(message[i]);
      }
      sequence++;
}


/*
This RFduino sketch demonstrates a full bi-directional Bluetooth Low
Energy 4 connection between an iPhone application and an RFduino.

This sketch works with the rfduinoLedButton iPhone application.

The button on the iPhone can be used to turn the green led on or off.
The button state of button 1 is transmitted to the iPhone and shown in
the application.
*/

/*
 Copyright (c) 2014 OpenSourceRF.com.  All right reserved.

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 See the GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/



// pin 3 on the RGB shield is the red led
// (can be turned on/off from the iPhone app)
int led = 2;

// pin 5 on the RGB shield is button 1
// (button press will be shown on the iPhone app)
int button = 3;

// debounce time (in ms)
int debounce_time = 10;

// maximum debounce timeout (in ms)
int debounce_timeout = 100;

void setup() {
  // led turned on/off from the iPhone app
  //  pinMode(led, OUTPUT);

  // button press will be shown on the iPhone app)
  pinMode(button, INPUT);
  Serial.begin(9600);
  // this is the data we want to appear in the advertisement
  // (if the deviceName and advertisementData are too long to fix into the 31 byte
  // ble advertisement packet, then the advertisementData is truncated first down to
  // a single byte, then it will truncate the deviceName)
  RFduinoBLE.advertisementData = "csesame";
  
  // start the BLE stack
  RFduinoBLE.begin();
}

int debounce(int state)
{
  int start = millis();
  int debounce_start = start;
  
  while (millis() - start < debounce_timeout)
    if (digitalRead(button) == state)
    {
      if (millis() - debounce_start >= debounce_time)
        return 1;
    }
    else
      debounce_start = millis();

  return 0;
}

int delay_until_button(int state)
{
  // set button edge to wake up on
  if (state)
    RFduino_pinWake(button, HIGH);
  else
    RFduino_pinWake(button, LOW);
    
  do
    // switch to lower power mode until a button edge wakes us up
    RFduino_ULPDelay(INFINITE);
  while (! debounce(state));
  
  // if multiple buttons were configured, this is how you would determine what woke you up
  if (RFduino_pinWoke(button))
  {
    // execute code here
    RFduino_resetPinWake(button);
  }
}

void loop() {
  uint8_t input;
  //  delay(100);
  while (Serial.available()) {
    input = Serial.read();
  }
  //  delay_until_button(HIGH);
  //  RFduinoBLE.send(1);
  
  //  delay_until_button(LOW);
  //  RFduinoBLE.send(0);
}

void RFduinoBLE_onDisconnect()
{
  // don't leave the led on if they disconnect
  //  digitalWrite(led, LOW);
}

void RFduinoBLE_onReceive(char *data, int len)
{
  // if the first byte is 0x01 / on / true
  RFduinoBLE.send(data[0]);
  switch(data[0]) {
    case 0:
      RFduinoBLE.send('U');
      delay(100);
      send_message(unpair);
      break;
    case 2:
      RFduinoBLE.send('l');
      delay(100);
      send_message(lock);
      break;
    case 3:
      RFduinoBLE.send('u');
      delay(100);
      send_message(unlock);
      break;
    case 4:
      RFduinoBLE.send('s');
      delay(100);
      send_message(status);
      break;
    /* case 'S': */
    /*   Serial.print("battery status"); */
    /*   send_message(bat_status); */
    /*   break; */
    case 1:
      RFduinoBLE.send('r');
      send_message(pair_2);
      break;
    default:
      break;
  }
}
