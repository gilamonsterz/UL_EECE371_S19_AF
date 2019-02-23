//github Test
//VS Code test

#include <DallasTemperature.h>
#include <OneWire.h>
#define bus1 5
OneWire oneWire(bus1);
DallasTemperature sensors(&oneWire);
void setup() {
    Serial.begin(9600);
    Serial.println("Start");
    sensors.begin();
}
void loop() {
    sensors.requestTemperatures();
    Serial.println(sensors.getTempCByIndex(0));
    delay(200);
    Serial.println("nx");
    delay(100);
}