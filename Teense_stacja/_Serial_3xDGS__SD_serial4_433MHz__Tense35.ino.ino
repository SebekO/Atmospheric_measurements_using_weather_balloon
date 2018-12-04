//Created by SebekO https://github.com/SebekO used edited lib DGS from https://github.com/SPEC-Sensors/DGS#include <SD.h>
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <LSM6.h>
#include <LPS.h>
#include <LIS3MDL.h>
#include <DFRobot_BME280.h>
#include "DGS.h"

File myFile;
const int chipSelect = BUILTIN_SDCARD;
DGS mySensor1(&Serial1);
DGS mySensor2(&Serial2);
DGS mySensor3(&Serial3);
#define SEA_LEVEL_PRESSURE  1013.25f
#define BME_CS 10

LSM6 imu;
LPS ps;
LIS3MDL mag;
DFRobot_BME280 bme; //I2C
float temp, pa, hum, alt;

char report_mag[80];
char report[80];

void setup(){
  Serial4.begin(57600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.begin(9600);
  Wire.begin();
  delay(5000);
//////////////////////////////////////////////////SD//////////////////////////////////////////
  Serial4.print("Initializing SD card...");
  if (!SD.begin(chipSelect)) {
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

void loop(){
    myFile = SD.open("38.txt", FILE_WRITE); //file open
    if (myFile) {
///////////////////////////////////////////////////DATA_READING////////////////////////////////////////
      /////////////////////////////////////////////DGS/////////////////////////////////////////////////
      mySensor1.getData('\r');
      mySensor2.getData('\r');
      mySensor3.getData('\r');
      String serial_n1= mySensor1.getId('i');
      String serial_n2= mySensor2.getId('i');
      String serial_n3= mySensor3.getId('i');
      long conc1=(mySensor1.getConc('c'));
      long conc2=(mySensor2.getConc('c'));
      long conc3=(mySensor3.getConc('c'));
      long temp1=(mySensor1.getTemp('c'));
      long temp2=(mySensor2.getTemp('c'));
      long temp3=(mySensor3.getTemp('c'));
      long hum1=(mySensor1.getRh('c'));
      long hum2=(mySensor2.getRh('c'));
      long hum3=(mySensor3.getRh('c'));
      long time1=(mySensor1.getTime('s'));
      long time2=(mySensor2.getTime('s'));
      long time3=(mySensor3.getTime('s'));
      ////////////////////////////////////////////AltIMU-10///////////////////////////////////////////
      imu.read();
      snprintf(report, sizeof(report), "A: %6d x %6d y %6d z G: %6d x %6d y %6d z",
      imu.a.x, imu.a.y, imu.a.z,
      imu.g.x, imu.g.y, imu.g.z);
      Serial4.println(report);
      mag.read();
      snprintf(report_mag, sizeof(report_mag), "M: %6d x %6d y %6d z",
       mag.m.x, mag.m.y, mag.m.z);
      Serial4.println(report_mag); 
      float pressure = ps.readPressureMillibars()*100;
      float altitude = ps.pressureToAltitudeMeters(pressure/100);
      float temperature = ps.readTemperatureC();
      //////////////////////////////////////////BEM280//////////////////////////////////////////////////////
      temp = bme.temperatureValue();
      pa = bme.pressureValue();
      hum = bme.humidityValue();
      alt = bme.altitudeValue(SEA_LEVEL_PRESSURE);
///////////////////////////////////////////////////DATA_WRITING_TO_FILE/////////////////////////////////////
      myFile.print(serial_n1);
      myFile.print('\t');
      myFile.print(conc1);
      myFile.print('\t');
      myFile.print(temp1);
      myFile.print('\t');
      myFile.print(hum1);
      myFile.print('\t');
      myFile.println(time1);
      myFile.print(serial_n2);
      myFile.print('\t');
      myFile.print(conc2);
      myFile.print('\t');
      myFile.print(temp2);
      myFile.print('\t');
      myFile.print(hum2);
      myFile.print('\t');
      myFile.println(time2);
      myFile.print(serial_n3);
      myFile.print('\t');
      myFile.print(conc3);
      myFile.print('\t');
      myFile.print(temp3);
      myFile.print('\t');
      myFile.print(hum3);
      myFile.print('\t');
      myFile.println(time3);
      myFile.print("AltMU: ");
      myFile.print(temperature);
      myFile.print(" C ");
      myFile.print(pressure);
      myFile.print(" Pa ");
      myFile.print(altitude);
      myFile.println(" m ");
      myFile.print("BME:   ");
      myFile.print(temp);
      myFile.print(" C ");
      myFile.print(pa);
      myFile.print(" Pa ");
      myFile.print(alt);
      myFile.println(" m ");
      myFile.close();
///////////////////////////////////////////////////DATA_WRITING_TO_SERIAL4////////////////////////////////////////
      Serial4.print(serial_n1);
      Serial4.print('\t');
      Serial4.print(conc1);
      Serial4.print('\t');
      Serial4.print(temp1);
      Serial4.print('\t');
      Serial4.print(hum1);
      Serial4.print('\t');
      Serial4.println(time1);
      Serial4.print(serial_n2);
      Serial4.print('\t');
      Serial4.print(conc2);
      Serial4.print('\t');
      Serial4.print(temp2);
      Serial4.print('\t');
      Serial4.print(hum2);
      Serial4.print('\t');
      Serial4.println(time2);
      Serial4.print(serial_n3);
      Serial4.print('\t');
      Serial4.print(conc3);
      Serial4.print('\t');
      Serial4.print(temp3);
      Serial4.print('\t');
      Serial4.print(hum3);
      Serial4.print('\t');
      Serial4.println(time3);
      Serial4.print("AltMU: ");
      Serial4.print(temperature);
      Serial4.print(" C ");
      Serial4.print(pressure);
      Serial4.print(" Pa ");
      Serial4.print(altitude);
      Serial4.println(" m ");
      Serial4.print("BME:   ");
      Serial4.print(temp);
      Serial4.print(" C ");
      Serial4.print(pa);
      Serial4.print(" Pa ");
      Serial4.print(alt);
      Serial4.println(" m ");
///////////////////////////////////////////////////DATA_WRITING_TO_SERIAL////////////////////////////////////////
  wypisz_serial(serial_n1, conc1, temp1, hum1, time1, report, report_mag, temp, pa, hum, alt, pressure, altitude, temperature);

    }
  else {
    Serial4.println("error opening file");
  }
  while (Serial4.available()) // read from Serial port, send to port SDK port
  {
    char U = Serial4.read();
    if (U == 'Z') {
      if (mySensor1.zero()) Serial4.println("Finished Setting Sensor 1 Zero"); //zeros returns 1/0
      if (mySensor2.zero()) Serial4.println("Finished Setting Sensor 2 Zero"); //zeros returns 1/0
      if (mySensor3.zero()) Serial4.println("Finished Setting Sensor 3 Zero"); //zeros returns 1/0
    }
    if (U == 'f') {
    Serial4.print(mySensor1.getFW());
    Serial4.print(", ");
    Serial4.print(mySensor2.getFW());
    Serial4.print(", ");
    Serial4.println(mySensor3.getFW());
    }
    if (U == 'e') {
      mySensor1.DEBUG = true;
      mySensor2.DEBUG = true;
      mySensor3.DEBUG = true;
      mySensor1.getEEPROM();
      mySensor2.getEEPROM();
      mySensor3.getEEPROM();
    }
  }
}
