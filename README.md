# DS18B20 Snow Depth Logger

A script (tested using the Adafruit Feather 32u4 and Adalogger shield) for logging DS18B20 temperature loggers in series. Developed with the intention of monitoring snow depth, by mounting DS18B20 sensors to snow post at known spacing interval. Snow depth can be approximated from the temperature signals. Requires user to specify the unique serial number for each DS18B20 sensor. The DallasTemperature  oneWireSearch script can be used for this. Relies on the PCF8523 real time clock built into the feather wing and  RTClib for keeping track of logging intervals, and time stamps. Implements the Adafruit_SleepyDog library for putting the logger into low power sleep mode via a watchdog timer between temperature readings. Writing data to SD card relies on the SD and SPI libraries. 

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

Must have the following installed:

```
Arduino IDE
OneWire Library 
DallasTemperature Library
RTClib Library 
Adafruit_SleepyDog Library
SPI Library 
SD Library 
```

### Installing

Provided the prerequisite libraries are in place, the script can simply be downloaded or copyied to the users local Arduino sketch book directory. 

```
1) Download ds18b20_temp_log.ino script.
2) Put in folder with same name.
3) Make sure named folder is in Arduino sketch book directory.
```

## Running

Once the script has been put into the users local sketch book, the sketch can be uploaded using the Arduino IDE. 

```
1) Open Arduino IDE
2) File>Sketchbook>ds18b20_temp_log.ino
3) Make sure DS18B20 serial numbers are correct for your sensor configuration (see oneWireSearch), change logging interval and logfile name to suit logging needs. Make sure OneWire bus is correct.   
4) Sketch>Upload
!! Because the Adafruit_Sleepydog puts the logger to sleep, when uploading a new sketch the reset button must be pressed during the upload as the logger will likely be asleep!!
```

## Deployment

This set up was tested on a [Adafruit 32u4](https://learn.adafruit.com/adafruit-feather-32u4-basic-proto) with the [Adalogger Featherwing](https://learn.adafruit.com/adafruit-adalogger-featherwing/overview) shield. Temperature readings are made using the [DS18B20](https://www.adafruit.com/product/381) sensors in series. See attached Fritzing diagram for hardware configuration.

## Built With

* [Arduino](https://www.arduino.cc/)

## Authors

* Hunter Gleason

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

* Thanks to all open source contributes who created the libraries used in this project. 
