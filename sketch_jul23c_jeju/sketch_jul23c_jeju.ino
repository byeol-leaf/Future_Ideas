#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h>
#endif
#define PIN       6
#define NUMPIXELS 8
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define DO 0
#define RE 1
#define MI 2
#define FA 3
#define SOL 4
#define LA 5
#define SI 6
#define SILENT 7
uint8_t ssb[48]={0};

void setup() {
  // put your setup code here, to run once:
  pixels.clear();

  ssb[0]= SOL;
  ssb[1]= SOL;
  ssb[2]= LA;
  ssb[3]= LA;
  ssb[4]= SOL;
  ssb[5]= SOL;
  ssb[6]= MI;
  ssb[7]= MI;

  ssb[8]= SOL;
  ssb[9]= SOL;
  ssb[10]= MI;
  ssb[11]= MI;
  ssb[12]= RE;
  ssb[13]= RE;
  ssb[14]= RE;
  ssb[15]= SILENT;

  ssb[16]= SOL;
  ssb[17]= SOL;
  ssb[18]= LA;
  ssb[19]= LA;
  ssb[20]= SOL;
  ssb[21]= SOL;
  ssb[22]= MI;
  ssb[23]= MI;

  ssb[24]= SOL;
  ssb[25]= MI;
  ssb[26]= RE;
  ssb[27]= MI;
  ssb[28]= DO;
  ssb[29]= DO;
  ssb[30]= DO;
  ssb[31]= SILENT;

  pixels.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0; i<32; i++) {
    if((i%8)==7) pixels.clear();
      pixels.setPixelColor(ssb[i], 128, 128, 0);
    	pixels.show();
   		delay(1000);
  		pixels.clear();}
    	}
