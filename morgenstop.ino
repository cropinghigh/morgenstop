#include "RF24.h"
/*
rf24 pin ESP32 pin
1 ground
2 3.3V Note (10-50uF cap across ground and 3.3V)
3 (CE) 22
4 (CSN) 21
5 (SCK) 18
6 (MOSI) 23
7 (MISO) 19
*/
//BTF ~= 2403-2480M

RF24 radio(22, 21, 16000000);

uint8_t ch = 50; //NRF has weird behaviour when frequency is being set during transmission; looks like it varies only about +-20-30 channels from the center one, set before transmission. 40-50 appears to be the most effective values experimentally found

void setup(void) {
  Serial.begin(115200);
  Serial.println("RAP IS GOVNO, PUNKS HOI!");
  radio.begin();
  radio.setAutoAck(false);
  radio.startListening();
  radio.stopListening();
  radio.setPALevel(RF24_PA_MAX, true);
  radio.setDataRate(RF24_2MBPS);
  radio.setCRCLength(RF24_CRC_DISABLED);
  radio.printDetails();
  radio.startConstCarrier(RF24_PA_MAX, ch); //Using test mode to get uninterruptible output, and changing frequency fast enough to cover the entire band
}


void loop(void) {
  uint32_t randVal = esp_random(); //Randomize time and frequency shift to get better spectrum spread
  uint32_t del = 10;
  switch(randVal & 0b11100) {
    case 0b00000:
      del = 3;
      break;
    case 0b00100:
      del = 4;
      break;
    case 0b01000:
      del = 5;
      break;
    case 0b01100:
      del = 6;
      break;
    case 0b10000:
      del = 10;
      break;
    case 0b10100:
      del = 15;
      break;
    case 0b11000:
      del = 1;
      break;
    case 0b11100:
      del = 20;
      break;
  }
  delayMicroseconds(del);
  radio.setChannel(ch);
  ch += (randVal & 0b11);
  if(ch > 80) {
    ch = 1;
  }
//  ch = map(randVal & 0b11111111, 0, 255, 1, 70); //Fully random frequency works worse. I guess that BT's FHSS adapts to the pure white noise quite good, but worse to the impulse chaotic one
}
