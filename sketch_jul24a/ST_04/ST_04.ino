
#include <RH_RF69.h>
#include "uFire_SHT20.h"


//#define SCREEN_WIDTH 128 // OLED display width, in pixels
//#define SCREEN_HEIGHT 32 // OLED display height, in pixels

//#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
//#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Adafruit_ADS1015 ads;

#define RF69_FREQ 433.0
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
  //#define LED           13
#endif

#if defined (__AVR_ATmega328P__)  // Feather 328P w/wing
  #define RFM69_INT     3  // 
  #define RFM69_CS      4  //
  #define RFM69_RST     2  // "A"
 // #define LED           13
#endif

#if defined(ESP8266)    // ESP8266 feather w/wing
  #define RFM69_CS      2    // "E"
  #define RFM69_IRQ     15   // "B"
  #define RFM69_RST     16   // "D"
 // #define LED           0
#endif

#if defined(ESP32)    // ESP32 feather w/wing
  #define RFM69_RST     13   // svb ame as LED
  #define RFM69_CS      33   // "B"
  #define RFM69_INT     27   // "A"
  //#define LED           13
#endif

#define SHT20_I2C  0x40


#define HOUSE_CDE    1
#define NODE_ID    1
#define SENSOR_CNT    3

#define STATUSPIN 13

#define READ_INTERVAL 1000

float f_temp;
float f_hum;
float f_dewpoint;
float f_humidity37oC;
int temp;
int hum;
int dewpoint;
int humidity37oC;

byte high;
byte low;

unsigned long nowShow = 0;
unsigned long pastShow = 0;


byte sendData[(SENSOR_CNT*3) + 1 + 1 + 1 + 3];
RH_RF69 rf69(RFM69_CS, RFM69_INT);

uFire_SHT20 sht20;

void setup() {
  Serial.begin(19200);
  delay(2000);

  Serial.println("start");


  
  if (!rf69.init()) {
    Serial.println("RFM69 radio init failed");
    //while (1);
  }
  
  if (!rf69.setFrequency(RF69_FREQ)) {
    Serial.println("setFrequency failed");
  }
  
  rf69.setTxPower(20, true);  // range from 14-20 for power, 2nd arg must be true for 69HCW

  uint8_t key[] = { 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                    0x01, 0x05, 0x04, 0x03, 0x02, 0x01, 1, 0x08}; 

  rf69.setEncryptionKey(key);
Serial.println("start11");
  //Wire.begin();
  Serial.println("start12");
  sht20.begin();
Serial.println("start13");
  
  sendData[0] = HOUSE_CDE;
  sendData[1] = NODE_ID;
  //sendData[2] = SENSOR_CNT+1;
  sendData[2] = SENSOR_CNT;

  
  
  
}



void loop() {  

 

  sht20.measure_all();
  uint8_t pos = 3;
  uint8_t sid = 1;

  temp = (sht20.tempC*10);
  hum = (sht20.RH*10);
  dewpoint = (sht20.dew_pointC*10);
  humidity37oC = (sht20.vpd()*10);

  

  Serial.println();
  Serial.print("Temperature = ");   
  Serial.print(temp);
  Serial.print(" oC, Humidity = ");  
  Serial.print(hum);
  Serial.print(" %, Dewpoint = ");  
  Serial.print(dewpoint);
  Serial.print(" oC, Humidity @37oC = ");
  Serial.print(humidity37oC);
  Serial.print(" % "); 
  Serial.println();
  
  high = highByte(temp);
  low = lowByte(temp);
  sendData[pos++]= sid++;     
  sendData[pos++]= high; 
  sendData[pos++]= low;  

  high = highByte(hum);
  low = lowByte(hum);
  sendData[pos++]= sid++; 
  sendData[pos++]= high; 
  sendData[pos++]= low;  


  high = highByte(dewpoint);
  low = lowByte(dewpoint);
  sendData[pos++]= sid++; 
  sendData[pos++]= high; 
  sendData[pos++]= low;  



  rf69.send(sendData,(SENSOR_CNT*3) + 1 + 1 + 1 + 3);
  Blink(STATUSPIN,50,1);
  delay(1000);
  rf69.sleep(); 

  delay(READ_INTERVAL*2);
  
  //delay(READ_INTERVAL/2);
}






void Blink(byte PIN, byte DELAY_MS, uint8_t loops) {
  for (byte i=0; i<loops; i++)  {
    digitalWrite(PIN,HIGH);
    delay(DELAY_MS);
    digitalWrite(PIN,LOW);
    delay(DELAY_MS);
  }
} 

void BlinkAnalog(byte PIN, byte DELAY_MS, byte loops, uint8_t bright) {
  for (byte i=0; i<loops; i++)  {
    analogWrite(PIN,bright);
    delay(DELAY_MS);
    analogWrite(PIN,0);
    delay(DELAY_MS);
  }
} 
