#include <DHT.h>
#define DHTTYPE DHT11
#define DHTPIN 8

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("\n");
  int h = (float)dht.readHumidity();
  int t = dht.readTemperature();
  Serial.print("Humidity (%): ");
  Serial.println(h);
  Serial.print("Temperature (℃): ");
  Serial.print(t);
  

  delay(5000);
}