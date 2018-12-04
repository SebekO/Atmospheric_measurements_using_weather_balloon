void wypisz_serial(String serial_n, long conc, long temp1, long hum, long time1,  char *report, char *report_mag, float temp, float pa, float hum1, float alt, float pressure, float altitude, float temperature){
      Serial.println(report);
      Serial.println(report_mag); 
      Serial.print(serial_n);
      Serial.print('\t');
      Serial.print(conc);
      Serial.print('\t');
      Serial.print(temp1);
      Serial.print('\t');
      Serial.print(hum);
      Serial.print('\t');
      Serial.println(time1);
      Serial.print("AltMU: ");
      Serial.print(temperature);
      Serial.print(" C ");
      Serial.print(pressure);
      Serial.print(" Pa ");
      Serial.print(altitude);
      Serial.println(" m ");
      Serial.print("BME:   ");
      Serial.print(temp);
      Serial.print(" C ");
      Serial.print(pa);
      Serial.print(" Pa ");
      Serial.print(alt);
      Serial.println(" m ");
}
