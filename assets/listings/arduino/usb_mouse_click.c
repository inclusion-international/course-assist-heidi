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
  if (digitalRead(btn) == LOW) {
    digitalWrite(led, HIGH);
    Serial.println("Clicking left mouse button...");
    mouse.click(MOUSE_LEFT);
  } else {
    digitalWrite(led, LOW);
  }
}
