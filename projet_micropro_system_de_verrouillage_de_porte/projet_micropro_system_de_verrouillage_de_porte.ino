/*
  Projet Microprocesseurs : System de verrouillage de porte
  AKKAL Aghilas 19009504
  lien Tinkercad :
  https://www.tinkercad.com/things/2LGuvAvqNk2-system-de-verrouillage-de-porte  
  
  NB : J'aurais pu utiliser EEPROMWrite() et EEPROMRead() pour sauvgarder le code octet par octet dans 
  la mémoire EEPROM de l'arduino et donc éviter de perdre le code à chaque fois qu'on redémarre,
  mais vu que c'est juste un simulateur, je ne sais pas si c'est utile d'implanter ça vu que la mémoire
  EEPROM est maintenant hardware, et donc unique à chaque carte arduino, mais ça reste faisable.
  
*/


/*************************************** LES DECLARATIONS ******************************************/

#include <LiquidCrystal.h>
#include <Keypad.h>				// librairie du keypad à télécharger et installer manuellement
#include <Servo.h>

#define buzzer 8				// pin du buzzer

int screenOffMsg =0; 			// variable d'état de l'écran d'acceuil
String password="1234"; 		// code actuel
String tempPassword;			// variable pour sauvgarder code temporaire de saisie
boolean activated = false; 		// variable d'état "en cours d'écriture"
boolean passChangeMode = false;	// variable d'état "changement du code"
boolean passChanged = false;	// variable d'état "code changé"


Servo myservo;					// déclaration de notre servo moteur


const byte ROWS = 4;															//	
const byte COLS = 4;															//	
char keypressed;																//	
char keyMap[ROWS][COLS] = {														//	définition
  {'1','2','3','A'},															//	des lignes 
  {'4','5','6','B'},															//	et colonnes
  {'7','8','9','C'},															//	à utiliser
  {'*','0','#','D'}};															//	par le keypad
byte rowPins[ROWS] = {7, 6, 5, 4};												//
byte colPins[COLS] = {3, 2, 1, 0};												//
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS);	//


LiquidCrystal lcd(A0, A1, A2, A3, A4, A5); 		// définition des pins du LCD

/**************************************** LE SETUP *************************************************/

void setup() { 
  lcd.begin(16,2);				// définition de la matrice du LCD
  myservo.attach(9);			// pin du servo moteur
  pinMode(buzzer, OUTPUT); 		// buzzer en output
}

/***************************************** LE LOOP *************************************************/

void loop() {
    if (screenOffMsg == 0 ){					// afficher le message d'acceuil
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("A - OUVRIR PORTE");
      lcd.setCursor(0,1);
      lcd.print("B - CHANGER CODE");
      screenOffMsg = 1;
    }
    keypressed = myKeypad.getKey();
    if (keypressed =='A'){       				// si on appuie sur A, on ouvre la porte
      tone(buzzer, 750, 25);
      enterPassword();
    }
    else if (keypressed =='B') { 				// si on appuie sur B, on change le code
      tone(buzzer, 750, 25);
      changePassword();
   }
}

/************************************* LES FONCTIONS ***********************************************/

void openServo() {								// fonction qui ouvre la porte
  int pos;
  for (pos=90; pos<=180; pos+=5){				//une boucle for, ou on spécifie les degrés, et le pas en degré
    myservo.write(pos);
    delay(15);
    }
}

void closeServo() {								// fonction qui ferme la porte
  int pos;
  for (pos=180; pos>=90; pos-=5){
    myservo.write(pos);
    delay(15);
  }
}



void enterPassword() {							// fonction qui recupere le code
  int i, k=7;
  tempPassword = "";
  activated = true;								// statut changé en "ntrain d'écrire
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" VEUILLEZ ENTRER LE CODE| * POUR VALIDER                        | # POUR EFFACER");
  for (i=0; i<66; i++){
    lcd.scrollDisplayLeft();					// faire défiler le text vers la gauche, patienter pour pouvoir
    delay(70);									//ecrire le code ensuite
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("* ENTREZ CODE *");
  lcd.setCursor(0,1);
  lcd.print("CODE : ");
      while(activated) {
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {	// on s'assure que juste les chiffres sont reconnus
          tone(buzzer, 250, 25);
          tempPassword += keypressed;
          lcd.setCursor(k,1);
          lcd.print("*");
          k++;
        }
      }
      if (k > 11 || keypressed == '#') {				// si on dépasse 4 chiffres, ou appuie sur #, effacer
        tone(buzzer, 35, 25);
        tempPassword = "";
        k=7;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("* ENTREZ CODE *");
        lcd.setCursor(0,1);
        lcd.print("CODE : ");
      }
      if (keypressed == '*') {							// * permet de valider
        if (tempPassword == password ) {				// on vérifie que le code est juste
          activated = false;
          tone(buzzer, 2000, 1000);
          lcd.clear();
          lcd.setCursor(0,0);  
          lcd.print(" ** BIENVENUE ** ");
          delay(1000);
          openServo();
          delay(2000);
          for (i=3; i>0; i--){							// i définit le délais avant la fermeture
            tone(buzzer, 4000, 250);
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(" FERMETURE DANS ");
            lcd.setCursor(7,1);
            lcd.print(i);
            delay(1000);
          }
          tone(buzzer, 40, 1000);
          closeServo();
          screenOffMsg = 0; 
        }
        else if (tempPassword != password) {
          tone(buzzer, 4000, 1000);
          lcd.setCursor(0,1);
          lcd.print("CODE INCORRECT !");
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("* ENTREZ CODE *");
          lcd.setCursor(0,1);
          lcd.print("CODE : ");
        }
      }
    }
}

void changePassword() {									// fonction qui permet de changer le code
      int i;
      lcd.clear();
  	  lcd.setCursor(0,0);
  	  lcd.print(" VEUILLEZ ENTRER LE CODE| * POUR VALIDER                        | # POUR EFFACER");
   	  for (i=0; i<66; i++){
     	 lcd.scrollDisplayLeft();
     	 delay(70);
    	}
      i=3;
      tempPassword = "";
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("CODE ACTUEL :");
      lcd.setCursor(0,1);
      lcd.print(" > ");
      passChangeMode = true;						// on est dans le mode "changer le code"
      passChanged = false;   						// code pas encore changé
      while(!passChanged) {      					// on reste dans la boucle tant que le code n'est pas changé
      keypressed = myKeypad.getKey();
      if (keypressed != NO_KEY){
        if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
            keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
            keypressed == '8' || keypressed == '9' ) {
         tone(buzzer, 250, 25);
         tempPassword += keypressed;
         lcd.setCursor(i,1);
         lcd.print("*");
         i++;
        }
      }
      if (i > 7 || keypressed == '#') {
        tempPassword = "";
        i=3;
        tone(buzzer, 35, 25);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("CODE ACTUEL :");
        lcd.setCursor(0,1);
        lcd.print(" > "); 
      }
      if (keypressed == '*') {
        tone(buzzer, 500, 25);
        if (password != tempPassword) {
          tempPassword = "";
          i=3;
          tone(buzzer, 4000, 1000);
          lcd.setCursor(0,1);
          lcd.print("CODE INCORRECT !");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("CODE ACTUEL :");
          lcd.setCursor(0,1);
          lcd.print(" > ");
        }
        if (password == tempPassword) {
          tempPassword="";
          i=3;
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("NOUVEAU CODE :");
          lcd.setCursor(0,1);
          lcd.print(" > ");
          while(passChangeMode) {// on reste dans la boucle tant tant qu'on le nouveau code n'est pas enregistré
            keypressed = myKeypad.getKey();
            if (keypressed != NO_KEY){
              if (keypressed == '0' || keypressed == '1' || keypressed == '2' || keypressed == '3' ||
                  keypressed == '4' || keypressed == '5' || keypressed == '6' || keypressed == '7' ||
                  keypressed == '8' || keypressed == '9' ) {
                tone(buzzer, 250, 25);
                tempPassword += keypressed;
                lcd.setCursor(i,1);
                lcd.print("*");
                i++;
              }
            }
            if (i > 7 || keypressed == '#') {
              tone(buzzer, 35, 25);
              tempPassword = "";
              i=3;
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("NOUVEAU CODE :");
              lcd.setCursor(0,1);
              lcd.print(" > ");
            }
            if ( keypressed == '*') {
              tone(buzzer, 500, 25);
              password = tempPassword;
              tone(buzzer, 2000, 1000);
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("  CODE CHANGE !");
              delay(2000);
              passChanged = true;							// code changé
              passChangeMode = false;						// on sort de mode "changer code"
              screenOffMsg = 0;
            }            
          }
        }
      }
    }  
}