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

//Import libraries
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include <Adafruit_SleepyDog.h>

//Initialize  PCF8523 RTC with RTClib
RTC_PCF8523 rtc;

//Define OneWire Bus
#define ONE_WIRE_BUS 6

//Initialize OneWire
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);


//USER VARS//

//Set DS18B20 Serial Address, these must be updated for each hardware configuration, see oneWireSearch. (add or delete for the number of Thermometers present)
DeviceAddress Thermometer1 = {0x28, 0x13, 0x98, 0xE2, 0x08, 0x00, 0x00, 0xFE};
DeviceAddress Thermometer2 = {0x28, 0x4E, 0x84, 0xE3, 0x08, 0x00, 0x00, 0x48};
DeviceAddress Thermometer3 = {0x28, 0xD2, 0x41, 0xE4, 0x08, 0x00, 0x00, 0xF2};
DeviceAddress Thermometer4 = {0x28, 0x8D, 0x1F, 0xE4, 0x08, 0x00, 0x00, 0xCF}; 

//Child select for Adalogger Featherwing SD
const int chipSelect = 10;

//Name for output logfile
const String filename = "LogFile.txt";

//Number of samples to average per reading, change as needed.
const int n = 3;

//Logging interval in seconds, change as needed. 
const int log_interval=15;

//Define logging time var
long log_time;

//Runs Once
void setup() {

  /*
  //For Debugging only
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  */ 
  
  // See if SD the card is present and can be initialized:
  while(!SD.begin(chipSelect)) {
    //don't do anything more:
    delay(5);
  }

  //Open the logfile as 'filename'
  File dataFile = SD.open(filename, FILE_WRITE);
  
  //Print header and close file 
  dataFile.println("DateTime,T1,T2,T3,T4");
  dataFile.close();

  //Initialize RTC 
  if (!rtc.begin()) {
    delay(5);
  }

  //Set RTC to system time 
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //Get current time 
  DateTime now = rtc.now();

  //Set log time to current time + log interval 
  log_time=now.unixtime()+log_interval;

  //Initialize sensors and set resolution 
  sensors.begin();
  sensors.setResolution(Thermometer1,12);
  sensors.setResolution(Thermometer2,12);
  sensors.setResolution(Thermometer3,12);
  sensors.setResolution(Thermometer4,12);

}

//Runs infinitely 
void loop() {

  //Get current time from RTC
  DateTime now = rtc.now();
  long current_time=now.unixtime();

  //If the current time is greater than or equal to logging time 
  if(current_time>=log_time)
  {
    //Update logging time to current time + log_interval 
    log_time=current_time+log_interval;
    
    //Log DS18B20 temperature data 
    logtemp();
  }else{
    //Trouble shooting only 
    //digitalWrite(LED_BUILTIN, LOW);
    
    //Else put MC back to sleep for as long as possible (8 sec. max) 
    Watchdog.sleep();
  }

}

//Function for averaging temperature readings, provided n and a thermometer address 
float avTemp(int n, DeviceAddress therm){

  //Get a temp. reading from thermometer at therm address 
  float t = sensors.getTempC(therm);

  //Repeat n times, add sum to var t 
  for(int i = 1; i<n ; i++)
  {
    t = t + sensors.getTempC(therm);
    delay(2);
  }

  //Divide by n to get average temp
  t = t/float(n);

  //Return average 
  return t;
}

//Function for logging DS18B20 readings 
void logtemp()
{

  //Open the log file 
  File dataFile = SD.open(filename, FILE_WRITE);

  //Get current time from RTC
  DateTime now = rtc.now();

  //Request DS18B20 temperatures 
  sensors.requestTemperatures();

  //Get average temp for each Thermometer (add or delete for the number of Thermometers present)
  float t1 = avTemp(n, Thermometer1);
  float t2 = avTemp(n, Thermometer2);
  float t3 = avTemp(n, Thermometer3);
  float t4 = avTemp(n, Thermometer4);

  //Create a data string (add or delete for the number of Thermometers present)
  String datastring = String(String(now.unixtime())+","+String(t1) + "," + String(t2) + "," + String(t3) + "," + String(t4));

  //Print datastring to SD card and close file
  if (dataFile) {
    dataFile.println(datastring);
    dataFile.close();
  }

}
