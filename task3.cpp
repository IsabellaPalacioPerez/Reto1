#include <Arduino.h>
#include "task3.h"
#include "events.h"

String btnState(uint8_t btnState)
{
    if (btnState == HIGH)
    {
        return "OFF";
    }
    else if (btnState == LOW)
    {
        return "ON";
    }
    else
    return "ERROR";
}
String ledBlueS(uint8_t ledBlue)
{
    if (ledBlue == HIGH)
    {
        return "ON";
    }
    else if (ledBlue == LOW)
    {
        return "OFF";
    }
    else
    return "ERROR";
}
void task3()
{

    enum class TaskStates
    {
        INIT,
        WAIT_PRESS,
    };
    static TaskStates taskState = TaskStates::INIT;

    constexpr uint8_t PRESS_BTN_PIN = 13;
    static uint8_t ledGren = 25;
    static uint8_t ledBlue = 26;

    static uint32_t lasTime;
    static constexpr uint32_t STATIC_TIME = 500;

    static bool ledStatus = false;

    switch (taskState)
    {
        case TaskStates::INIT:
        {
            Serial.begin(115200);
            pinMode(PRESS_BTN_PIN, INPUT_PULLUP);
            ledStatus = LOW;
            pinMode(ledGren, OUTPUT);
            digitalWrite(ledGren, ledStatus);
            lasTime = 0;
            pinMode(ledBlue, OUTPUT);
            digitalWrite(ledBlue, ledStatus);
            taskState = TaskStates::WAIT_PRESS;
            break;
        }
        case TaskStates::WAIT_PRESS:
        {
            uint32_t currentTime = millis();
            if ((currentTime - lasTime) >= STATIC_TIME)
            {
                lasTime = currentTime;
                ledStatus = !ledStatus;
                digitalWrite(ledGren, ledStatus);
            }
            if (Serial.available() > 0)
            {
                String command = Serial.readStringUntil('\n');
                if (command == "ledON")
                {
                    digitalWrite(ledBlue, HIGH);
                }
                else if (command == "ledOFF")
                {
                    digitalWrite(ledBlue, LOW);
                }
                else if (command == "readBUTTONS")
                {
                    Serial.print("btn: ");
                    Serial.print(btnState(digitalRead(PRESS_BTN_PIN)).c_str());
                    Serial.print(' ');
                    Serial.print("led: ");
                    Serial.print(ledBlueS(digitalRead(ledBlue)).c_str());
                    Serial.print('\n');
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
}