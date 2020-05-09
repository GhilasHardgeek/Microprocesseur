import serial

morseArduino = serial.Serial('com4', 9600)  # le 'com4' peut varier, selon le port série branché dans le PC

def textToMorse (text):
  CODE = {
                # alphabet
                'A': '.-',     'B': '-...',   'C': '-.-.',   'D': '-..',
                'E': '.',      'F': '..-.',   'G': '--.',    'H': '....',
                'I': '..',     'J': '.---',   'K': '-.-',    'L': '.-..',
                'M': '--',     'N': '-.',     'O': '---',    'P': '.--.',
                'Q': '--.-',   'R': '.-.',    'S': '...',    'T': '-',
                'U': '..-',    'V': '...-',   'W': '.--',    'X': '-..-',
                'Y': '-.--',   'Z': '--..',
                # Numbers
                '0': '-----',  '1': '.----',  '2': '..---',  '3': '...--',
                '4': '....-',  '5': '.....',  '6': '-....',  '7': '--...',
                '8': '---..',  '9': '----.',
                # Extended
                ' ': '/',       '.': '.-.-.-', ',': '--..--',   ':': '---...',
                '?': '..--..', "'": '.----.',  '-': '-....-',   '/': '-..-.',
                '@': '.--.-.', '=': '-...-',   '(': '-.--.',    ')': '-.--.-',
                '+': '.-.-.'
            }
  morseCode = ""
  for x in text:
    morseCode += CODE[x.upper()]
    morseCode += " "
  
  return morseCode

while (True):
  try:   
    text = input("Veuillez entrer le text à convertir en Morse: ")
    print(textToMorse(text))
    morseArduino.write(textToMorse(text))
  except KeyError:
    print("\nUn ou plusieurs caractères non-supportés détectés !\n\n[INFO] Les caractères du code Morse international sont:\n\nL'alphabet: 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'\n\nLes chiffres: '0', '1', '2', '3', '4', '5', '6', '7', '8', '9'\n\nLes caractères spéciaux: 'éspace', '.', 'virgule', ':', '?', 'apostrophe', '-', '/', '@', '=', '(', ')', '+'\n\n")
  except KeyboardInterrupt:
    print("\n\n**************** Au revoir ! ****************")
    break