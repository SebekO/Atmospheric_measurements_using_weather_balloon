void zapisz_SD(all dane)
{
  dane.myFile.print("667769");
  dane.myFile.print('\t');
  dane.myFile.print(dane.temp);
  dane.myFile.print('\t');
  dane.myFile.print(dane.pa);
  dane.myFile.print('\t');
  dane.myFile.print(dane.alt);
  dane.myFile.println();
}
