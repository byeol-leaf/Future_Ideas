
#include <EEPROM.h>
#include <RH_RF69.h>
#include <Wire.h> 



enum NMEA_BLOCK_TYPE
{
  NMEA_FIRST_BLOCK,
  NMEA_INTERMEDIATE_BLOCK,
  NMEA_LAST_BLOCK
};

// and define the transmitted block as:
struct NMEA_BLOCK
{
   uint8_t
     len,
     type,
     data[58];    // note that you don't have to fill or send ALL of these if your block is shorter than 58 bytes, 'len' tells the receiver how many bytes to read.
} block;

//#define RH_RF69_MAX_ENCRYPTABLE_PAYLOAD_LEN 80

//특용
#define RF69_FREQ 433.0


/************ Radio Setup  딸기
#define RF69_FREQ 434.5
***************/

#if defined (__AVR_ATmega32U4__) // Feather 32u4 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     7
  #define RFM69_RST     4
  #define LED           13
#endif

#if defined(ARDUINO_SAMD_FEATHER_M0) // Feather M0 w/Radio
  #define RFM69_CS      8
  #define RFM69_INT     3
  #define RFM69_RST     4
  #define LED           13
#endif

#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM69_INT     3  // 
  #define RFM69_CS      4  //
  #define RFM69_RST     2  // "A"
  #define LED           13
#endif

#if defined(ESP8266)    // ESP8266 feather w/wing
  #define RFM69_CS      2    // "E"
  #define RFM69_IRQ     15   // "B"
  #define RFM69_RST     16   // "D"
  #define LED           0
#endif

#if defined(ESP32)    // ESP32 feather w/wing
  #define RFM69_RST     13   // same as LED
  #define RFM69_CS      33   // "B"
  #define RFM69_INT     27   // "A"
 // #define LED           13
#endif

// Singleton instance of the radio driver
RH_RF69 rf69(RFM69_CS, RFM69_INT);

//software reset
void(* resetFunc)(void) = 0;
union ArrayToInteger {
  byte array[2];
  uint16_t integer;
};

ArrayToInteger converter;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif

#define STATUSPIN A5
#define PIN_POWER_CHECK_INTERRUPT 0
#define PIN_OPP_FAN A1

byte rfRevData[30] = {0}; // each SensorNode
String sNames[5] = {"온도","습도","이슬점","VPD","BAT"};


void setup() {
  Serial.begin(19200);
  delay(1000);
  pinMode(PIN_POWER_CHECK_INTERRUPT,INPUT_PULLUP);
  pinMode(PIN_OPP_FAN, OUTPUT);     
  if (!rf69.init()) {
    Serial.println("init failed");
  }
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }

  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW
  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x01};
  rf69.setEncryptionKey(key);  
  pinMode(LED, OUTPUT);
  

  Serial.println("start");
}




void loop() {
  byte sid = 0;
  if (rf69.available() ) { 
    uint8_t len = sizeof(rfRevData);
    if (rf69.recv(rfRevData, &len)) {  
      if (!len) return;
      
      byte house_cde = rfRevData[0];
      byte node_id = rfRevData[1];
      byte sensor_cnt = rfRevData[2];

      byte pos = 3; 

      Blink(13,500,10);

      for (byte k=0; k<sensor_cnt; k++) {
        sid = rfRevData[pos++];
        byte high = rfRevData[pos++];
        byte low = rfRevData[pos++];

        byte idxName = sid -1;
        if (sid == 0x09) {
          idxName = 4;
        }
        converter.array[0] = low;
        converter.array[1] = high;
        Serial.print(sNames[idxName]); Serial.print(" : " );
        Serial.println(converter.integer / 10.0); 
      }
      Serial.println("");
    }
  } 

}


void Blink(byte PIN, byte DELAY_MS, byte loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
} 


