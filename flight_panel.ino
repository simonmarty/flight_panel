#include "Joystick.h"
#include "PCF8575.h"
#include "RotaryEncoder.h"

#define PINS 14
#define PCF_PINS 16
#define ROTARY_ENCODER_PINS 2

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD, PINS + PCF_PINS + ROTARY_ENCODER_PINS, 0,
                   false, false, false, false, false, false, false, false, false, false, false);
PCF8575 PCF(0x20);
RotaryEncoder* rotaryEncoder = nullptr;

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

class DirectionButton
{
public:
  RotaryEncoder::Direction lastDirection;

  DirectionButton()
  {
    lastDirection = RotaryEncoder::Direction::NOROTATION;
  }
};

CButton Buttons[PINS] = {4, 5, 6, 7, 8, 9, 10, 14, 15, 16, 20, 21, A0, A1};
CButton PCFButtons[PCF_PINS] = {P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12, P13, P14, P15};
DirectionButton rotaryEncoder1Button;

void checkPosition()
{
  rotaryEncoder->tick();
}

void setup()
{
  PCF.begin();
  rotaryEncoder = new RotaryEncoder(0, 1, RotaryEncoder::LatchMode::TWO03);
  for (int i = 0; i < PINS; i++)
  {
    pinMode(Buttons[i].pin, INPUT_PULLUP);
  }
  for (int j = 0; j < PCF_PINS; j++)
  {
    PCF.pinMode(PCFButtons[j].pin, INPUT);
  }

  Joystick.begin(false);
  attachInterrupt(digitalPinToInterrupt(0), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(1), checkPosition, CHANGE);
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

void EncoderStates()
{
  RotaryEncoder::Direction dir = rotaryEncoder->getDirection();
  long pos = rotaryEncoder->getPosition();

  if (dir != rotaryEncoder1Button.lastDirection)
  {
    int ENCODER_PIN_CCW = PINS + PCF_PINS;
    int ENCODER_PIN_CW = PINS + PCF_PINS + 1;
    switch (dir)
    {
    case RotaryEncoder::Direction::COUNTERCLOCKWISE:
      Joystick.setButton(ENCODER_PIN_CCW, HIGH);
      Joystick.setButton(ENCODER_PIN_CW, LOW);
      break;
    case RotaryEncoder::Direction::CLOCKWISE:
      Joystick.setButton(ENCODER_PIN_CCW, LOW);
      Joystick.setButton(ENCODER_PIN_CW, HIGH);
      break;
    default: // NOROTATION
      Joystick.setButton(ENCODER_PIN_CCW, LOW);
      Joystick.setButton(ENCODER_PIN_CW, LOW);
      break;
    }

    rotaryEncoder1Button.lastDirection = dir;
  }
}

void loop()
{
  EncoderStates();
  JButtonStates();
  Joystick.sendState();
  delay(50);
}
