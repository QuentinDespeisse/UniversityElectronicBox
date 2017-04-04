// Initialisation des variables
int switchState = 0;
bool stateA = 0;
bool stateB = 0;
bool stateC = 0;
bool stateD = 0;
bool stateCorr = 0;
bool stateV = 1;
String str = "";

// On prépare les broches en spécifiant si elles sont en entrée ou en sortie
void setup() {
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

// Algorithme tournant en boucle
void loop() {
  // Bouton A
  switchState = digitalRead(7); // On enregistre l'état de la broche
  if (switchState == HIGH) { // Si il y a changement de broche, alors
    stateA = 1; // On passe le bouléen à 1
  }
  
  // Pour les boutons A, B, C, et D, le même processus s'effectue
  
  // Bouton B
  switchState = digitalRead(6);
  if (switchState == HIGH) {
    stateB = 1;
  }
  
  // Bouton C
  switchState = digitalRead(5);
  if (switchState == HIGH) {
    stateC = 1;
  }
  
  // Bouton D
  switchState = digitalRead(4);
  if (switchState == HIGH) {
    stateD = 1;
  }
  
  // Pour le bouton de correction, on réinitialise les booléens à 0, et on éteint les LED
  
  // Bouton Corr
  switchState = digitalRead(3);
  if (switchState == HIGH) {
    digitalWrite(12, LOW);
    digitalWrite(11, LOW);
    digitalWrite(10, LOW);
    digitalWrite(9, LOW);
    stateA, stateB, stateC, stateD = 0;
  }
  
  // Pour le bouton de validation, on crée la phrase que l'on enverra ultérieurement en Wi-Fi au programme
  
  // Bouton V
  switchState = digitalRead(2);
  if (switchState == HIGH) {
    if (stateA) str += "A";
    if (stateB) str += "B";
    if (stateC) str += "C";
    if (stateD) str += "D";
  }
  delay(200);
}
