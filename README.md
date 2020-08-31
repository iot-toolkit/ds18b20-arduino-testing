# ds18b20-arduino-testing

* First we need to download and install the libraries OneWire and DallasTemperature from below links:
  - https://www.arduinolibraries.info/libraries/one-wire
  - https://www.arduinolibraries.info/libraries/dallas-temperature
* To install libraries go to: Arduino IDE -> Sketch -> Include Library -> Add .ZIP Library..., and select downloaded libraries.
* Open Serial Monitor window: Tools -> Serial Monitor. 
* Enter the ambient temperature in Serial Monitor.
* The program will display sensor/sensors number, address and current temperature reading in 'Readings' section.
* If the sensor detects temperature that is 2 degrees (or more) different than the ambient temperature, the sensor data and time since program start (in minutes) will be displayed in 'Errors' section.
