/*
  TP 01 Microprocesseurs : Faire clignoter une LED
  AKKAL Aghilas 19009504
  lien Tinkercad :
  https://www.tinkercad.com/things/0M5rwuTbX1S-tp-microprocesseurs-faire-clignoter-une-led
  
*/

void setup()
{
  // Initialisation du port 13 comme sortie pour alimenter la LED
  pinMode(13, OUTPUT);
}

void loop()
{
  // On allume la LED en mettant le statu du por 13 en HIGH
  digitalWrite(13, HIGH);
  delay(1000); // On crée un délais de 1000 ms ou 1s
  // On éteint la LED en mettant le statu du por 13 en HIGH
  digitalWrite(13, LOW);
  delay(1000); // On crée un délais de 1000 ms ou 1s
}