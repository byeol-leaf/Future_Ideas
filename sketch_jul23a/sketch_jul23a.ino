#include <Adafruit_NeoPixel.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#define PIN        6
#define NUMPIXELS 16
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500
#include <uFire_SHT20.h>
uFire_SHT20 sht20;

 #include "I2CScanner.h"
I2CScanner scanner;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  Wire.begin();
  sht20.begin();

  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif

  pixels.begin();

  	while (!Serial) {};

	scanner.Init();
}

void loop() {
  // put your main code here, to run repeatedly:
  sht20.measure_all();
  Serial.println((String)sht20.tempC + "°C");
  Serial.println((String)sht20.tempF + "°F");
  Serial.println((String)sht20.dew_pointC + "°C dew point");
  Serial.println((String)sht20.dew_pointF + "°F dew point");
  Serial.println((String)sht20.RH + " %RH");
  Serial.println((String)sht20.vpd() + " kPa VPD");
  Serial.println();

  pixels.clear();
  delay(5000);
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
    pixels.show();
    delay(DELAYVAL);

	scanner.Scan();
	delay(5000);}
}
