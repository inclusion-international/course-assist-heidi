#include "PluggableUSBHID.h"
#include "USBMouse.h"

#define DEBOUNCE_DELAY 200

int led = 2;
int btn = 3;

int buttonState = HIGH;
int lastButtonState = LOW;
unsigned long lastDebounceTime = 0;

USBMouse mouse;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
}

void loop() {
  int reading = digitalRead(btn);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        digitalWrite(led, HIGH);
        Serial.println("Clicking left mouse button...");
        mouse.click(MOUSE_LEFT);
      } else {
        digitalWrite(led, LOW);
      }
    }
  }

  lastButtonState = reading;
}