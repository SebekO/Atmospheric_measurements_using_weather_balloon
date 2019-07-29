void zapisz_SD(all dane)
{
  dane.myFile = SD.open("test_11.txt", FILE_WRITE); //file open
  if (dane.myFile)
  {
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
    dane.myFile.print("667769");
    dane.myFile.print('\t');
    dane.myFile.print(dane.temp);
    dane.myFile.print('\t');
    dane.myFile.print(dane.hum);
    dane.myFile.print('\t');
    dane.myFile.print(dane.pa);
    dane.myFile.print('\t');
    dane.myFile.print(dane.alt);
    dane.myFile.println();
    dane.myFile.close(); //file close
  }
  else
  {
    Serial4.println("error opening file");
    delay(1000);
  }
}
