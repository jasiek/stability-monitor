const int xPin = 0;
const int yPin = 1;
const int zPin = 2;
const int rfPin = 4;
const int ledPin = 13;

int xx, yy, zz;

unsigned int counter;
int moving = 0;

void setup() {
  Serial.begin(57600);
  Serial.println("start");
  pinMode(ledPin, OUTPUT);
  Serial.end();
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

int movementDetected() {
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
}

ISR(TIMER2_OVF_vect) {
  if (counter++ > 200) {
    stopTimer();
    moving = 0;
    digitalWrite(ledPin, LOW);
  }
}

void resetTimer() {
  Serial.println("resetTimer");
  counter = 0;
}

void startTimer() {
  Serial.println("startTimer");
  TCNT2 = 0;
  counter = 0;
  moving = 1;
  TIMSK2 |= (1<<TOIE2);
}

void stopTimer() {
  Serial.println("stopTimer");
  TIMSK2 &= ~(1<<TOIE2);
}

void loop() {
  if (movementDetected()) {
    if (moving) {
      resetTimer();
    } else {
      startTimer();
    }
    
    digitalWrite(ledPin, HIGH);
  }
  delay(100);
}
