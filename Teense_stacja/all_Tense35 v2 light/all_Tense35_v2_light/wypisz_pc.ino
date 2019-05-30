void wypisz_pc(all dane)
{
  Serial.print("BME:   ");
  Serial.print(dane.temp);
  Serial.print(" C ");
  Serial.print(dane.pa);
  Serial.print(" Pa ");
  Serial.print(dane.alt);
  Serial.println(" m ");
}
