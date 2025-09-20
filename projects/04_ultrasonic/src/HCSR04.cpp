#include "HCSR04.h"

void HCSR04::begin(int trig, int echo)
{
    _trig = trig;
    _echo = echo;
    pinMode(_trig, OUTPUT);
    pinMode(_echo, INPUT);
}

float HCSR04::distance(void)
{
    digitalWrite(_trig, LOW);
    delayMicroseconds(1);
    digitalWrite(_trig, HIGH);
    delayMicroseconds(11);
    digitalWrite(_trig, LOW);
    int duration = pulseIn(_echo, HIGH);
    if (duration <= 0)
    {
        return -1;
    }
    float distance = duration / 2;
    distance = distance * 340 * 100 / 1000000; // ultrasonic speed is 340m/s = 34000cm/s = 0.034cm/us
    return distance;
}
