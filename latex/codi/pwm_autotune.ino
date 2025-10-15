int pwm_pin = 3; // Pin de sortida del PWM
int duty_cycle = 100; // Variable per desar el duty cycle, amb un valor inicial donat
int timemeas_pin = 4; // Pin per marcar l'inici/final del bucle, per l'AD2
int feedback; // Variable per desar el valor de la tensió de feedback llegida
int fb_pin = A0; // Pin per llegir la tensió de sortida del buck
float r1 = 465; // Resistència 1 del divisor de tensió
float r2 = 1000 - r1; // Resistència 2 del divisor de tensió
float hist = 10; // Llindar d'histèresi pel mètode autotune
float h = 5; // Excitació diferencial de PWM
float vout = 5.0; // Tensió de sortida desitjada del buck
float vcent; // Tensió a la que es vol centrar el senyal, de 0 a 255
float tini, tfin, tspan;

void setup() {
  
  // Clear ADC prescaler bits; ADC a aprox. 100ksamples/sec
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));

  // Set ADC prescaler to 4 (division factor), about 8us per sample
  ADCSRA |= bit (ADPS1); 
    
  // Iniciem la comunicació serial, si cal
  // Serial.begin(9600);
  
  // PWM a 32kHz, als pins 3 i 11
  TCCR2B = TCCR2B & B11111000 | B00000001; 

  // Inicialitació ADC on es connecta V feedback
  pinMode(fb_pin, INPUT);

  // Inicialització GPIO de sortida PWM
  pinMode(pwm_pin, OUTPUT);
  pinMode(11, OUTPUT);
  analogWrite(11, 120); // Pin d'emergència per connectar si el 3 no té un comportament adequat

  // Pin que mesura el temps que triga a executar-se el loop
  pinMode(timemeas_pin, INPUT);

  // Càlcul de la tensió que farem arribar a l'ADC per Vout=5V, segons les resistències emprades al divisor de tensió
  vcent = 1023 * r2 / (r1 + r2) * vout / 5.0;

  // Serial.begin(9600);
}



void loop() {

  // Marca d'inici del loop
  // digitalWrite(timemeas_pin, HIGH);
  // delay(10);
  // digitalWrite(timemeas_pin, LOW);
  // delay(5);

  // Mesura de la durada del bucle: uns 44us aprox.
  // tspan = micros() - tini;
  // Serial.println(tspan);
  // tini = micros();

  // Ajustem la durada del bucle a 100us
  delayMicroseconds(200);



  // Lectura del feedback
  feedback = analogRead(fb_pin);

  // Conversió a 0,255
  // feedback = feedback * 255 / 1023;

  if (feedback >  vcent + hist) {
    duty_cycle -= h;
  } else if (feedback < vcent - hist) {
    duty_cycle += h;
  }

  duty_cycle = constrain(duty_cycle, 1, 254);

  // Apliquem el valor trobat de PWM a la sortida
  analogWrite(pwm_pin, duty_cycle);
  // delayMicroseconds(50);
  // delay(1);

  // Temps d'espera per igualar el temps que trigarà el loop en el PID
  // Serial.print(vcent);
  // Serial.print("\t");
  // Serial.print(feedback);
  // Serial.print("\t");
  // Serial.print(duty_cycle);
  // Serial.print("\n");
  // delay(10);

  // Marca de final del loop
  // digitalWrite(timemeas_pin, HIGH);
  // delay(5);
  // digitalWrite(timemeas_pin, LOW);
  // delay(5);
}

