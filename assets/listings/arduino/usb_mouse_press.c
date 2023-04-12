#include "PluggableUSBHID.h"
#include "USBMouse.h"

int led = 2;
int btn = 3;
USBMouse mouse;

void setup() {
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(led, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
}

void loop() {
  static int released = 1;

  if (digitalRead(btn) == LOW) {
    if (released) {
      digitalWrite(led, HIGH);
      Serial.println("Pressing left mouse button...");
      mouse.press(MOUSE_LEFT);
      released = 0;
    }
  } else {
    if (!released) {
      digitalWrite(led, LOW);
      mouse.release(MOUSE_LEFT);
      Serial.println("Releasing left mouse button...");
      released = 1;
    }
  }
}