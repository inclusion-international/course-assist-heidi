from machine import Pin
from time import sleep_ms

led = Pin(25, Pin.OUT)

while True:
    led.toggle()
    if (led()):
        print("On")
    else:
        print("Off")
    sleep_ms(1000)
