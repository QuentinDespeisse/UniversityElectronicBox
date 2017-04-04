// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int switchState = 0;
bool stateA = 1;
bool stateB = 1;
bool stateC = 1;
bool stateD = 1;
bool stateCorr = 1;
bool stateV = 1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // Bouton A
  switchState = digitalRead(7);
  if (switchState == HIGH) {
    if (stateA) {
      digitalWrite(12, HIGH);
  	  stateA = !stateA;
    }
    else {
      digitalWrite(12, LOW);
      stateA = !stateA;
    }
  }
  
  // Bouton B
  switchState = digitalRead(6);
  if (switchState == HIGH) {
    if (stateB) {
      digitalWrite(11, HIGH);
  	  stateB = !stateB;
    }
    else {
      digitalWrite(11, LOW);
      stateB = !stateB;
    }
  }
  
  // Bouton C
  switchState = digitalRead(5);
  if (switchState == HIGH) {
    if (stateC) {
      digitalWrite(10, HIGH);
  	  stateC = !stateC;
    }
    else {
      digitalWrite(10, LOW);
      stateC = !stateC;
    }
  }
  
  // Bouton D
  switchState = digitalRead(4);
  if (switchState == HIGH) {
    if (stateD) {
      digitalWrite(9, HIGH);
  	  stateD = !stateD;
    }
    else {
      digitalWrite(9, LOW);
      stateD = !stateD;
    }
  }
  
  // Bouton Corr
  switchState = digitalRead(3);
  if (switchState == HIGH) {
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
  }
  
  // Bouton V
  switchState = digitalRead(2);
  delay(200);
}
