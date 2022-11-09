#include <Joystick.h>
#include <PCF8575.h>

#define PINS 16
#define PCF_PINS 1

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, PINS + PCF_PINS, 0,
                   false, false, false, false, false, false, false, false, false, false, false);
PCF8575 PCF(0x20);

class CButton
{
public:
  int pin = NULL;
  int lastState = 0;

  CButton(int p)
  {
    pin = p;
  }
};

CButton Buttons[PINS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15, 20, 21};
CButton PCFButtons[PCF_PINS] = {P0};

void setup()
{
  PCF.begin();
  for (int i = 0; i < PINS; i++)
  {
    pinMode(Buttons[i].pin, INPUT_PULLUP);
  }
  for (int j = 0; j < PCF_PINS; j++)
  {
    PCF.pinMode(PCFButtons[j].pin, INPUT);
  }

  Joystick.begin();
}

void JButtonStates()
{
  for (int i = 0; i < PINS; i++)
  {
    uint8_t currentState = !digitalRead(Buttons[i].pin);

    if (currentState != Buttons[i].lastState)
    {
      Joystick.setButton(i, currentState);
      Buttons[i].lastState = currentState;
    }
  }
  for (int j = 0; j < PCF_PINS; j++)
  {
    uint8_t currentState = !PCF.digitalRead(PCFButtons[j].pin);
    if (currentState != PCFButtons[j].lastState)
    {
      Joystick.setButton(PINS + j, currentState);
      PCFButtons[j].lastState = currentState;
    }
  }
}

void loop()
{
  JButtonStates();
  delay(50);
}
