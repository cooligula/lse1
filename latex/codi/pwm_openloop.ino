int potpin = A1;
int potval = 0;
int pwm_pin = 3;
int duty_cycle;

void setup() {
  // put your setup code here, to run once:

// Serial.begin(9600);

TCCR2B = TCCR2B & B11111000 | B00000001; 

pinMode(potpin, INPUT);
pinMode(pwm_pin, OUTPUT);

}

void loop() {

potval = analogRead(potpin);

duty_cycle = constrain(map(potval, 0, 1023, 1, 254), 1, 254);//potval * 253/1024 + 1;

//Serial.println(duty_cycle);

analogWrite(pwm_pin, duty_cycle);

//delay(10);


}


