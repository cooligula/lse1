int pwm_pin = 3;      // Pin de sortida del PWM
int duty_cycle = 100; // Variable per desar el duty cycle, amb un valor inicial donat
int timemeas_pin = 4; // Pin per marcar l'inici/final del bucle, per l'AD2
int feedback;         // Variable per desar el valor de la tensió de feedback llegida
int fb_pin = A0;      // Pin per llegir la tensió de sortida del buck
int vcons_pin = A1;   // Pin per fer la lectura del potenciòmetre que permetrà trobar la tensió de consigna.
float vcons;          // Variable on desarem la tensió de consigna desitjada
float r1 = 465;       // Resistència 1 del divisor de tensió
float r2 = 1000 - r1; // Resistència 2 del divisor de tensió
float hist = 10;      // Llindar d'histèresi pel mètode autotune
float h = 5;          // Excitació diferencial de PWM
float vout = 5.0;     // Tensió de sortida desitjada del buck
float vcentral;       // Tensió a la que es vol centrar el senyal, de 0 a 255
float d5v = 120;      // Duty cycle que dona lloc a una sortida de 5V (trobat amb open loop)

// Mesures del cicle d'autotune
float Au = 0.840 / 5.0 * 255;     // V/V, unitats de ADC
float Pu = 1240;   // us, les mateixes unitats en les que mesuren micros()

// Càlcul dels coeficients del PID
float Kp = 4 * h /(3.141592 * Au * 1.7);
float Ki = 2 * Kp / Pu;
float Kd = Kp * Pu / 8;
float Cprop = 0, Cder = 0, Cint = 0;  // On desarem els valors dels coeficients del PID per cada iteració
float pid_pwm;            // Variable on desarem el valor del PWM calculat pel PID

// Variables pel control de la durada del cicle
float tcurr, tprev, tspan;

// Variables per la mesura de l'error comès
float error;
float prevError = 0;
float intError = 0;
float varError = 0;


void setup() {

  // Serial.begin(9600);
  
  // PWM a 32kHz, als pins 3 i 11
  TCCR2B = TCCR2B & B11111000 | B00000001; 

  // Inicialitació ADC on es connecta V feedback
  pinMode(fb_pin, INPUT);

  // Inicialització GPIO de sortida PWM
  pinMode(pwm_pin, OUTPUT);

  // Pin que mesura el temps que triga a executar-se el loop
  pinMode(timemeas_pin, OUTPUT);

  
  // Clear ADC prescaler bits; ADC a aprox. 100ksamples/sec
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));

  // Set ADC prescaler to 4 (division factor), about 8us per sample
  ADCSRA |= bit (ADPS1); 
}




void loop() {


  digitalWrite(timemeas_pin, HIGH); // Indiquem l'inici del bucle


  tcurr = micros();                 // Iniciem el bucle
  tspan = tcurr - tprev;            // Trobem la durada del bucle anterior

  feedback = map(analogRead(fb_pin), 0, 1023, 0, 255); // Lectura del feedback

  vcons = map(analogRead(vcons_pin), 0, 1023, 0, 255); // Lectura de la tensió de consigna
  
  // Càlcul de la tensió que farem arribar a l'ADC per Vout=5V, segons les resistències emprades al divisor de tensió
  // vcentral = 255 * r2 / (r1 + r2) * vout / 5.0;
  vcentral = r2 / (r1 + r2) * vcons * 2;
  
  error = (vcentral - feedback);      // Càlcul de l'error de la iteració
  
  Cprop = Kp * error;               // Terme proporcional

  intError += error * tspan;        // Terme integral
  Cint = Ki * intError;

  varError = (error - prevError) / tspan;   // Terme derivatiu
  Cder = Kd * varError;

  pid_pwm = Cprop + Cint + Cder; // Trobem el valor del PWM corresponent al procediment PID

  pid_pwm = constrain(pid_pwm, 1, 254);     // Ens assegurem que el valor del pwm calculat es manté en un rang segur pel MOSFET

  analogWrite(pwm_pin, pid_pwm);    // Apliquem el valor trobat de PWM a la sortida


  // Un cop finalitzat el bucle, desem les variables actuals com a les anteriors
  tprev = tcurr;
  prevError = error;

  digitalWrite(timemeas_pin, LOW);  // Indiquem el final de la iteració

}