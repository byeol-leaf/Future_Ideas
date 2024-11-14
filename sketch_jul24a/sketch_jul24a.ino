#include <Adafruit_NeoPixel.h>
#include <uFire_SHT20.h>
uFire_SHT20 sht20;
#ifdef __AVR__
 #include <avr/power.h>
#endif
#define PIN       6
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Wire.begin();
  sht20.begin();
  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  sht20.measure_all();
  float tempC = sht20.tempC;
  Serial.println((String)sht20.tempC + "°C");
  if ((tempC > 18) && (tempC < 22)) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 255));
    pixels.show();
  }
  else {
    pixels.setPixelColor(0, pixels.Color(255, 0, 0));
    pixels.show();
    for (uint8_t k=0; k<10; k++){
      pixels.setPixelColor(k, pixels.Color(255, 0, 0));
      pixels.show();
      delay(500);
      pixels.setPixelColor(k, pixels.Color(0, 0, 0));
      }
  delay(1000);
    
}
}
