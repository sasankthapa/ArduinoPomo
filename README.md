
This is an arduino system I built to help me study. More specifically, help me do pomodoro without me having to use an online site.

The electronics I used for this are:
- IR radio module with remote
- active buzzer
- LCD display
- Temperature and Humidity sensor
- arduino UNO R3

Arduina Libraries:
- IRremote
- Adafruit_Sensor (https://github.com/adafruit/Adafruit_Sensor)
- DHT sensor Library
- LiquidCrystal
- elapsedMillis

I really like the way the code handles time: the millis of the arduino keeps running, everytime we want to start a timer, we store an offset value of millis() that is ahead of current millis().
Doing it this way instead of using delay(1000) allows us to do other stuff like check the temperature while a timer is running in the background.

<img alt="picture" src="https://github.com/sasankthapa/ArduinoPomo/blob/master/preview.jpg">