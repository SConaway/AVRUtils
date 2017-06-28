#include <AVRUtils.h>

AVRUtils avr;

void setup() {
    Serial.begin(9600);
    while (!Serial);
    avr.begin();
}

void loop() {
    digitalWrite(13, HIGH);
    delay(500);
    digitalWrite(13, LOW);
    delay(500);

    Serial.print(F("AVR temp is "));
    Serial.print(avr.getAVRTemp());
    Serial.println(F(" *F"));
    Serial.print(F("AVR Vcc is "));
    Serial.print(avr.getAVRVcc());
    Serial.println(F(" mV"));
}
