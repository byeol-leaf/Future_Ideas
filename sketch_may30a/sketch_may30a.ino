#include <LiquidCrystal.h>
#include <Grove_Temperature_And_Humidity_Sensor.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN 2

LiquidCrystal lcd(4, 5, 6, 8, 9, 10, 11);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("\n");
  int h = (float)dht.readHumidity();
  int t = dht.readTemperature();
  lcd.setCursor(0, 0);
  lcd.print("Temp (C): ");
  lcd.print(t); 
  lcd.setCursor(0, 1);
  lcd.print("Moist(%): ");
  lcd.print(h);
  delay(1000);
  lcd.clear();
}
