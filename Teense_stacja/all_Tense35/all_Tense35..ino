//Created by SebekO https://github.com/SebekO used edited lib DGS from https://github.com/SPEC-Sensors/DGS#include
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LSM6.h>
#include <LPS.h>
#include <LIS3MDL.h>
#include <DFRobot_BME280.h>
#include "DGS.h"
#include <TinyGPS++.h>
struct all
{
  File myFile;
  float temp, pa, hum, alt;
  char report_mag[80];
  char report[80];
  String serial_n1, serial_n2, serial_n3;
  long conc1, conc2, conc3;
  long temp1, temp2, temp3;
  long hum1, hum2, hum3;
  long time1, time2, time3;
  float pressure, altitude, temperature;
} dane;
const int chipSelect = BUILTIN_SDCARD;
static const uint32_t Baud9 = 9600;
TinyGPSPlus gps;
DGS mySensor1(&Serial1);
DGS mySensor2(&Serial2);
DGS mySensor3(&Serial3);
#define SEA_LEVEL_PRESSURE  1013.25f
#define BME_CS 10
LSM6 imu;
LPS ps;
LIS3MDL mag;
DFRobot_BME280 bme; //I2C
void setup()
{
  Serial5.begin(Baud9);
  Serial4.begin(57600);
  Serial3.begin(Baud9);
  Serial2.begin(Baud9);
  Serial1.begin(Baud9);
  Serial.begin(Baud9);
  Wire.begin();
  delay(5000);
//////////////////////////////////////////////////SD//////////////////////////////////////////
  Serial4.print("Initializing SD card...");
  if (!SD.begin(chipSelect))
  {
    Serial4.println("initialization failed!");
    return;
  }
  Serial4.println("initialization done.");
  Serial4.println("Finished Setup 1/3");
////////////////////////////////////////////////AltIMU-10////////////////////////////////////////////
  Serial4.print("Initializing I2C devices...");
  if (!imu.init())
  {
    Serial.println("Failed to detect and initialize IMU!");
    while (1);
  }
  if (!ps.init())
  {
    Serial.println("Failed to autodetect pressure sensor!");
    while (1);
  }
  if (!mag.init())
  {
    Serial.println("Failed to detect and initialize magnetometer!");
    while (1);
  }
  if (!bme.begin(0x77)) {
    Serial.println("No sensor device found, check line or address!");
    while (1);
  }
  imu.enableDefault();
  ps.enableDefault();
  mag.enableDefault();
  Serial4.println("initialization done.");
  Serial4.println("Finished Setup 2/3");
///////////////////////////////////////////////////DGS////////////////////////////////////////
  Serial4.print("Initializing DGS devices...");
  mySensor1.DEBUG = false;
  mySensor2.DEBUG = false;
  mySensor3.DEBUG = false;
  mySensor1.getData('\r');
  mySensor2.getData('\r');
  mySensor3.getData('\r');
  mySensor1.getEEPROM();
  mySensor2.getEEPROM();
  mySensor3.getEEPROM();
  Serial4.println("initialization done.");
  Serial4.println("Finished Setup 3/3");
  Serial4.println();
///////////////////////////////////////////////////HELP////////////////////////////////////////
  Serial4.println("DGS_Sensors_ HELP: Press: 'e' to show Sensor Data; 'f' to show Firmware Date; 'Z' to set it 0 current reading ");
}

void loop()
{
  dane.myFile = SD.open("45.txt", FILE_WRITE); //file open
  if (dane.myFile)
  {
///////////////////////////////////////////////////DATA_READING////////////////////////////////////////
      /////////////////////////////////////////////DGS/////////////////////////////////////////////////
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
    ////////////////////////////////////////////AltIMU-10///////////////////////////////////////////
    imu.read();
    snprintf(dane.report, sizeof(dane.report), "A: %6d x %6d y %6d z G: %6d x %6d y %6d z",
    imu.a.x, imu.a.y, imu.a.z,
    imu.g.x, imu.g.y, imu.g.z);
    mag.read();
    snprintf(dane.report_mag, sizeof(dane.report_mag), "M: %6d x %6d y %6d z",
    mag.m.x, mag.m.y, mag.m.z);  
    dane.pressure = ps.readPressureMillibars()*100;
    dane.altitude = ps.pressureToAltitudeMeters(dane.pressure/100);
    dane.temperature = ps.readTemperatureC();
    //////////////////////////////////////////BEM280//////////////////////////////////////////////////////
    dane.temp = bme.temperatureValue();
    dane.pa = bme.pressureValue();
    dane.hum = bme.humidityValue();
    dane.alt = bme.altitudeValue(SEA_LEVEL_PRESSURE);
    wypisz_pc(dane);
    wypisz_433(dane); 
    zapisz_SD(dane);
    printFloat(gps.location.lat(), gps.location.isValid(), 11, 6);
    printFloat(gps.location.lng(), gps.location.isValid(), 12, 6);
    printDateTime(gps.date, gps.time);
    printFloat(gps.altitude.meters(), gps.altitude.isValid(), 7, 2);
    printInt(gps.failedChecksum(), true, 9);
    Serial.println();
    Serial4.println();
    dane.myFile.println();
    smartDelay(1000);
    dane.myFile.close()
    }
  else
  {
    Serial4.println("error opening file");
  }
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS data received: check wiring"));
  }
  while (Serial4.available()) // read from Serial port, send to port SDK port
  {
    char U = Serial4.read();
    if (U == 'Z') {
      if (mySensor1.zero()) Serial4.println("Finished Setting Sensor 1 Zero"); //zeros returns 1/0
      if (mySensor2.zero()) Serial4.println("Finished Setting Sensor 2 Zero"); //zeros returns 1/0
      if (mySensor3.zero()) Serial4.println("Finished Setting Sensor 3 Zero"); //zeros returns 1/0
    }
    if (U == 'f')
    {
      Serial4.print(mySensor1.getFW());
      Serial4.print(", ");
      Serial4.print(mySensor2.getFW());
      Serial4.print(", ");
      Serial4.println(mySensor3.getFW());
    }
    if (U == 'e')
    {
      mySensor1.DEBUG = true;
      mySensor2.DEBUG = true;
      mySensor3.DEBUG = true;
      mySensor1.getEEPROM();
      mySensor2.getEEPROM();
      mySensor3.getEEPROM();
    }
  }
}


static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (Serial5.available())
      gps.encode(Serial5.read());
  } while (millis() - start < ms);
}
static void printFloat(float val, bool valid, int len, int prec)
{
  if (!valid)
  {
    while (len-- > 1){
      Serial.print('*');
      Serial4.print('*');
      dane.myFile.print('*');
    }
    Serial.print(' ');
    Serial4.print('*');
    dane.myFile.print('*');
  }
  else
  {
    Serial.print(val, prec);
    Serial4.print(val, prec);
    dane.myFile.print(val, prec);
    int vi = abs((int)val);
    int flen = prec + (val < 0.0 ? 2 : 1); // . and -
    flen += vi >= 1000 ? 4 : vi >= 100 ? 3 : vi >= 10 ? 2 : 1;
    for (int i=flen; i<len; ++i){
      Serial.print(' ');
      Serial4.print(' ');
      dane.myFile.print(' ');
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
  dane.myFile.print(sz);
  smartDelay(0);
}
static void printDateTime(TinyGPSDate &d, TinyGPSTime &t)
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
    Serial4.print(F("********** "));
    dane.myFile.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.day(), d.month(), d.year());
    Serial.print(sz);
    Serial4.print(sz);
    dane.myFile.print(sz);
  }
  if (!t.isValid())
  {
    Serial.print(F("******** "));
    Serial4.print(F("******** "));
    dane.myFile.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
    Serial4.print(sz);
    dane.myFile.print(sz);
  }
  printInt(d.age(), d.isValid(), 5);
  smartDelay(0);
}
