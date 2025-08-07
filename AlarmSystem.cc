// 경고 장치(LED/Buzzer/Servo) 제어 함수 구현
#include "AlarmSystem.h"

AlarmSystem::AlarmSystem(int led, int buzzer, int servo)
    : ledPin(led), buzzerPin(buzzer), servoPin(servo) {
    pinMode(ledPin, OUTPUT);
    softToneCreate(buzzerPin);
    softPwmCreate(servoPin, 0, 200);
}

void AlarmSystem::activate() {
    std::vector<int> notes = { 300, 300, 300, 300, 300, 300, 300, 300 };
    std::cout << "Servo to 90 degrees...\n";
    softPwmWrite(servoPin, 15);
    delay(1200);

    std::cout << "LED and buzzer 10 sec...\n";
    long start = millis();
    while (millis() - start < 10000) {
        digitalWrite(ledPin, HIGH);
        softToneWrite(buzzerPin, notes[(millis() / 280) % notes.size()]);
        delay(500);
        digitalWrite(ledPin, LOW);
        softToneWrite(buzzerPin, 0);
        delay(500);
    }

    std::cout << "Servo to 0 degrees...\n";
    softPwmWrite(servoPin, 5);
    delay(1200);
}