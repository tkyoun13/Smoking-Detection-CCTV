// 하드웨어 제어를 위한 클래스 (LED, Buzzer, Servo)
#pragma once
#include <wiringPi.h>
#include <softTone.h>
#include <softPwm.h>
#include <vector>
#include <iostream>

class AlarmSystem {
public:
    AlarmSystem(int led, int buzzer, int servo);
    void activate(); // 경고 시스템 동작

private:
    int ledPin, buzzerPin, servoPin;
};

