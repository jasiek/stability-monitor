const int xPin = 0;
const int yPin = 1;
const int zPin = 2;
const int rfPin = 4;
const int ledPin = 13;

int xx, yy, zz;

unsigned int tcnt2;
unsigned int counter;

void setup() {
  Serial.begin(57600);
  Serial.println("start");
  pinMode(ledPin, OUTPUT);
  pinMode(rfPin, OUTPUT);
  randomSeed(analogRead(3));
  setupTimer();
}

int readMovement() {
   int x, y, z;
   x = analogRead(xPin);
   y = analogRead(yPin);
   z = analogRead(zPin);
   int diff = abs(x - xx) + abs(y - yy) + abs(z - zz);
   xx = x;
   yy = y;
   zz = z;
   return diff;
}

int isMoving() {
  return readMovement() > 20;
}

void setupTimer() {
  /* First disable the timer overflow interrupt while we're configuring */
  TIMSK2 &= ~(1<<TOIE2);
 
  /* Configure timer2 in normal mode (pure counting, no PWM etc.) */
  TCCR2A &= ~((1<<WGM21) | (1<<WGM20));
  TCCR2B &= ~(1<<WGM22);
 
  /* Select clock source: internal I/O clock */
  ASSR &= ~(1<<AS2);
 
  /* Disable Compare Match A interrupt enable (only want overflow) */
  TIMSK2 &= ~(1<<OCIE2A);
 
  /* Now configure the prescaler to CPU clock divided by 1024 */
  TCCR2B |= (1<<CS22) | (1<<CS21) | (1<<CS20);

  tcnt2 = 0; 
 
  /* Finally load end enable the timer */
  TCNT2 = tcnt2;
  TIMSK2 |= (1<<TOIE2);
}

ISR(TIMER2_OVF_vect) {
  counter++;
  TCNT2 = tcnt2;
}

void resetTimer() {
  counter = 0;
  TCNT2 = tcnt2;
}

void transmit(int value) {
  tone(rfPin, 1000, value);
}

void loop() {
  if (isMoving()) {
    transmit(readMovement());
    resetTimer();
    // digitalWrite(ledPin, HIGH);
  } else {
    if (counter > 200) {
      digitalWrite(ledPin, LOW);
    }

  }
  delay(200);
}
