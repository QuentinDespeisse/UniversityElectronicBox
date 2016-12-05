// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int switchState = 0;
bool state = 1;

// the setup routine runs once when you press reset:
void setup() {
  // initialize the digital pin as an output.
  pinMode(3, OUTPUT);
  pinMode(2, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  switchState = digitalRead(2);
  if (switchState == HIGH) {
    if (state) {
      digitalWrite(3, HIGH);
  	  state = !state;
    }
    else {
      digitalWrite(3, LOW);
      state = !state;
    }
  }
  delay(200);
}
