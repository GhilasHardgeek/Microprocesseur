/*
  TP Microprocesseurs : textVersMorse
  AKKAL Aghilas 19009504
  lien Tinkercad :
  https://www.tinkercad.com/things/aIGAJaEriHn-tpmicroprotextversmorse  
*/

String data="";								// variable qui reçoit le code morse du serial port
											// (ou du moniteur série dans ce cas)


int led = 13;								// pin de la LED

int dotUnite = 300;							// pour un "." c'est 1 unité de temps
											// pour un "-" c'est 3 unités
											// éspace entre symbole d'une meme lettre c'est 1 unité
											// éspace entre chaque lettre est de 3 unités
											// éspace entre chaque mot est 7 unités


void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(13, LOW);  
}




void loop() {
  if(Serial.available() > 0) {				// on vérifie qu'il y'a input du sérial
    data = Serial.readString();				// on met l'input dans la variable "data"
    const char* source = data.c_str();
    int length = (int)strlen(source);
    for (int i = 0; i < length; i++) {		// on parcours la chaine de caractères
      if(source[i]=='.')					// pour chaque ".", la LED s'allume 1 UT
      {  
        digitalWrite(led, HIGH); 
        delay(dotUnite);       
        digitalWrite(led, LOW); 
        delay(dotUnite); 					// entre chaque symbole, il y'a un vide de 1 UT
      }
      else if (source[i]=='-')				// pour chaque "-", la LED s'allume 3 UT
      {
        digitalWrite(led, HIGH); 
        delay(3*dotUnite);       
        digitalWrite(led, LOW); 
        delay(dotUnite); 
      }
      else if(source[i]=='/')				// entre chaque mot, il y'a un vide de 7 UT
      {
        delay(7*dotUnite); 
      }
      else if(source[i]==' ')
      {
        delay(3*dotUnite);					// entre chaque lettre, il y'a un vide de 3 UT
      }
    }
  }
}