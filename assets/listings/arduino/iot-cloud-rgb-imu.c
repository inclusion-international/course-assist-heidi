// The following variables are automatically generated and updated when changes are made
//
//   float a_x;
//   float a_y;
//   float a_z;
//   CloudColoredLight rgb_light;

#include <Arduino_LSM6DSOX.h>

#include "thingProperties.h"

void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  // Obtain information related to the state of network and IoT Cloud connection.
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();

  // Init IMU Library
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1)
      ;
  }
}

void loop() {
  ArduinoCloud.update();
  // Read acceleration and store in a_x, a_y, a_z variables
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(a_x, a_y, a_z);
  }
}

// Since RgbLight is READ_WRITE variable, onRgbLightChange() is
// executed every time a new value is received from IoT Cloud.
void onRgbLightChange() {
  uint8_t r, g, b;

  // Retrieve values from the cloud
  rgb_light.getValue().getRGB(r, g, b);

  // Values on Nano RP2040 Connect are inverted
  // So let's remap them
  int red = map(r, 0, 255, 255, 0);
  int green = map(g, 0, 255, 255, 0);
  int blue = map(b, 0, 255, 255, 0);

  if (rgb_light.getSwitch()) {
    analogWrite(LEDR, red);
    analogWrite(LEDG, green);
    analogWrite(LEDB, blue);
  } else {
    analogWrite(LEDR, 255);
    analogWrite(LEDG, 255);
    analogWrite(LEDB, 255);
  }
}
