const int ledPin1 = A0;
const int ledPin2 = A1;
const int ledPin3 = A2;
const int ledPin4 = A3;
const int buttonPin = A7;

int oldButtonState = LOW;
int x = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  int newButtonState = digitalRead(buttonPin);

  if (newButtonState == HIGH && oldButtonState == LOW)
  {
    if (x == 0) {
      // Toggle ON
      digitalWrite(ledPin1, HIGH);
      delay(100);
      digitalWrite(ledPin1, LOW);
      delay(100);
      digitalWrite(ledPin2, HIGH);
      delay(100);
      digitalWrite(ledPin2, LOW);
      delay(100);
      digitalWrite(ledPin3, HIGH);
      delay(100);
      digitalWrite(ledPin3, LOW);
      delay(100);
      digitalWrite(ledPin4, HIGH);
      delay(100);
      digitalWrite(ledPin4, LOW);
      delay(100);

      x = 1;
    }
    else {
      digitalWrite(ledPin1, LOW);
      digitalWrite(ledPin2, LOW);
      digitalWrite(ledPin3, LOW);
      digitalWrite(ledPin4, LOW);

      x = 0;
    }
  }
  oldButtonState = newButtonState;
}
