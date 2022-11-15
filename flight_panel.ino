#include <Joystick.h>

#define PINS 15

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, PINS, 0,
                   false, false, false, false, false, false, false, false, false, false, false);

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

CButton Buttons[PINS] = {9, 5, 10, 7, 8, A1, 15, 16, 6, 14, 0, 1, A3, 4, A2};

void setup()
{
  for (int i = 0; i < PINS; i++)
  {
    pinMode(Buttons[i].pin, INPUT_PULLUP);
  }

  Joystick.begin();
}

void JButtonStates()
{
  for (int i = 0; i < PINS; i++)
  {
    int currentState = !digitalRead(Buttons[i].pin);

    if (currentState != Buttons[i].lastState)
    {
      Joystick.setButton(i, currentState);
      Buttons[i].lastState = currentState;
    }
  }
}

void loop()
{
  JButtonStates();
  delay(50);
}
