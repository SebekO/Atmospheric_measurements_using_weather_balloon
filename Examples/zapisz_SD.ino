void zapisz_SD(all dane){
      dane.myFile.println(dane.report);
      dane.myFile.println(dane.report_mag);
      dane.myFile.print(dane.serial_n1);
      dane.myFile.print('\t');
      dane.myFile.print(dane.conc1);
      dane.myFile.print('\t');
      dane.myFile.print(dane.temp1);
      dane.myFile.print('\t');
      dane.myFile.print(dane.hum1);
      dane.myFile.print('\t');
      dane.myFile.println(dane.time1);
      dane.myFile.print(dane.serial_n2);
      dane.myFile.print('\t');
      dane.myFile.print(dane.conc2);
      dane.myFile.print('\t');
      dane.myFile.print(dane.temp2);
      dane.myFile.print('\t');
      dane.myFile.print(dane.hum2);
      dane.myFile.print('\t');
      dane.myFile.println(dane.time2);
      dane.myFile.print(dane.serial_n3);
      dane.myFile.print('\t');
      dane.myFile.print(dane.conc3);
      dane.myFile.print('\t');
      dane.myFile.print(dane.temp3);
      dane.myFile.print('\t');
      dane.myFile.print(dane.hum3);
      dane.myFile.print('\t');
      dane.myFile.println(dane.time3);
      dane.myFile.print("AltMU: ");
      dane.myFile.print(dane.temperature);
      dane.myFile.print(" C ");
      dane.myFile.print(dane.pressure);
      dane.myFile.print(" Pa ");
      dane.myFile.print(dane.altitude);
      dane.myFile.println(" m ");
      dane.myFile.print("BME:   ");
      dane.myFile.print(dane.temp);
      dane.myFile.print(" C ");
      dane.myFile.print(dane.pa);
      dane.myFile.print(" Pa ");
      dane.myFile.print(dane.alt);
      dane.myFile.println(" m ");
}
