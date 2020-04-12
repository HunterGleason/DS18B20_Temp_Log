//DS18B20_TEMP_LOG
/*
 * Author: Hunter Gleason
 * Date: Apr 12, 2020
 * Function: Uses the OneWire and DallasTemperature libraries to read DS18B20 temperature output in series.
 * Typically, the sensors are evenly spaced vertically along a post to determine depth of snow.
 * Temperature data is logged to a SD card, with timestamp from the PCF8523 real time clock.
 * The script was tested on a Adafruit Feather 32u4 board, equipped with the Adalogger FeatherWing shield.
 * To conserve power, the Adafruit_SleepyDog library is used to put the logger into low power mode
 * intermittently between readings. 
 */ 
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <Adafruit_SleepyDog.h>

RTC_PCF8523 rtc;

#define ONE_WIRE_BUS 6

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

DeviceAddress Thermometer1 = {0x28, 0x13, 0x98, 0xE2, 0x08, 0x00, 0x00, 0xFE};
DeviceAddress Thermometer2 = {0x28, 0x4E, 0x84, 0xE3, 0x08, 0x00, 0x00, 0x48};
DeviceAddress Thermometer3 = {0x28, 0xD2, 0x41, 0xE4, 0x08, 0x00, 0x00, 0xF2};
DeviceAddress Thermometer4 = {0x28, 0x8D, 0x1F, 0xE4, 0x08, 0x00, 0x00, 0xCF}; 


const int chipSelect = 10;

const String filename = "LogFile.txt";

const int n = 3;

const int log_interval=15;

long log_time=0;

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH); 
  // see if the card is present and can be initialized:
  while(!SD.begin(chipSelect)) {
    //don't do anything more:
    delay(5);
  }
  
  File dataFile = SD.open(filename, FILE_WRITE);
  dataFile.println("DateTime,T1,T2,T3,T4");
  dataFile.close();

  if (!rtc.begin()) {
    delay(5);
  }

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  DateTime now = rtc.now();

  log_time=now.unixtime()+log_interval;

  sensors.begin();
  sensors.setResolution(Thermometer1,12);
  sensors.setResolution(Thermometer2,12);
  sensors.setResolution(Thermometer3,12);
  sensors.setResolution(Thermometer4,12);

}

void loop() {
  
  DateTime now = rtc.now();
  long current_time=now.unixtime();

  if(current_time>=log_time)
  {
  log_time=current_time+log_interval;
  logtemp();
  }else{
    digitalWrite(LED_BUILTIN, LOW);
    Watchdog.sleep();
  }

}

float avTemp(int n, DeviceAddress therm){

  float t = sensors.getTempC(therm);
  
  for(int i = 1; i<n ; i++)
  {
    t = t + sensors.getTempC(therm);
    delay(2);
  }

  t = t/float(n);

  return t;
}


void logtemp()
{

  File dataFile = SD.open(filename, FILE_WRITE);

  DateTime now = rtc.now();

  sensors.requestTemperatures();


  float t1 = avTemp(n, Thermometer1);
  float t2 = avTemp(n, Thermometer2);
  float t3 = avTemp(n, Thermometer3);
  float t4 = avTemp(n, Thermometer4);

  String datastring = String(String(now.unixtime())+","+String(t1) + "," + String(t2) + "," + String(t3) + "," + String(t4));

  if (dataFile) {
    dataFile.println(datastring);
    dataFile.close();
  }

}
