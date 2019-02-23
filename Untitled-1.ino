//github Test
//VS Code test

//branch test

#include <DallasTemperature.h>
#include <OneWire.h>
#define bus2 5
OneWire oneWire(bus2);
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