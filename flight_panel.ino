#include <Joystick.h>

#define PINS 16

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

CButton Buttons[PINS] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 16, 14, 15, 20, 21};

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
