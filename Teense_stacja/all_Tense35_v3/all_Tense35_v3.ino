#include <TinyGPS++.h>
#include <SD.h>
#include <DFRobot_BME280.h>

TinyGPSPlus gps;
#include "DGS.h"
const int chipSelect = BUILTIN_SDCARD;

struct all
{
  File myFile;
  String serial_n1, serial_n2, serial_n3;
  int conc1, conc2, conc3;
  int temp1, temp2, temp3;
  int hum1, hum2, hum3;
  int time1, time2, time3;
  float temp, pa, hum, alt;
} dane;

#define SEA_LEVEL_PRESSURE  1013.25f
#define BME_CS 10
DFRobot_BME280 bme; //I2C

DGS mySensor1(&Serial2);
DGS mySensor2(&Serial3);
DGS mySensor3(&Serial5);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(57600);
  Serial5.begin(9600);
  Serial4.print("#Initializing SD card...");
  Serial.print("#Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    Serial4.print("#initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  Serial4.print("#initialization done.");
  if (!bme.begin(0x77)) {
    Serial4.println("No sensor device found, check line or address!");
    while (1);
  }
  Serial4.print("#Initializing DGS devices...");
  Serial.print("#Initializing DGS devices...");
  mySensor1.DEBUG = false;
  mySensor2.DEBUG = false;
  mySensor3.DEBUG = false;
  mySensor1.getData('\r');
  mySensor2.getData('\r');
  mySensor3.getData('\r');
  mySensor1.getEEPROM();
  mySensor2.getEEPROM();
  mySensor3.getEEPROM();
}

void loop()
{
  
  mySensor1.getData('\r');
  mySensor2.getData('\r');
  mySensor3.getData('\r');
  dane.serial_n1 = mySensor1.getId('i');
  dane.serial_n2 = mySensor2.getId('i');
  dane.serial_n3 = mySensor3.getId('i');
  dane.conc1 = (mySensor1.getConc('c'));
  dane.conc2 = (mySensor2.getConc('c'));
  dane.conc3 = (mySensor3.getConc('c'));
  dane.temp1 = (mySensor1.getTemp('c'));
  dane.temp2 = (mySensor2.getTemp('c'));
  dane.temp3 = (mySensor3.getTemp('c'));
  dane.hum1 = (mySensor1.getRh('c'));
  dane.hum2 = (mySensor2.getRh('c'));
  dane.hum3 = (mySensor3.getRh('c'));
  dane.time1 = (mySensor1.getTime('s'));
  dane.time2 = (mySensor2.getTime('s'));
  dane.time3 = (mySensor3.getTime('s'));
  
  dane.temp = bme.temperatureValue();
  dane.pa = bme.pressureValue();
  dane.hum = bme.humidityValue();
  dane.alt = bme.altitudeValue(SEA_LEVEL_PRESSURE);
    
  printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
  printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
  printDateTime(gps.date, gps.time);
  printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
  Serial.println();
  Serial4.println();
  wypisz_pc(dane);
  wypisz_433(dane);
  zapisz_SD(dane);
  
  smartDelay(1000);

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring"));
}

// This custom version of delay() ensures that the gps object
// is being "fed".
static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1){
      Serial.print('*');
      Serial4.print('*');
    }
    Serial.print(' ');
    Serial4.print('*');
  }
  else
  {
    Serial.print(val, prec);
    Serial4.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i){
      Serial.print(' ');
      Serial4.print(' ');
    }
  }
  smartDelay(0);
}
static void printInt(unsigned long val, bool valid, int len)
{
  char sz[32] = "*****************";
  if (valid)
    sprintf(sz, "%ld", val);
  sz[len] = 0;
  for (int i=strlen(sz); i<len; ++i)
    sz[i] = ' ';
  if (len > 0) 
    sz[len-1] = ' ';
  Serial.print(sz);
  Serial4.print(sz);
  smartDelay(0);
}
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
    Serial4.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.day(), d.month(), d.year());
    Serial.print(sz);
    Serial4.print(sz);
  }
  if (!t.isValid())
  {
    Serial.print(F("******** "));
    Serial4.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
    Serial4.print(sz);
  }
  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}
