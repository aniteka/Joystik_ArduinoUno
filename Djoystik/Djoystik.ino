#define button_a_pin 5
#define button_b_pin 4
#define button_c_pin 3
#define button_d_pin 2

#define joystik_Y       A2
#define joystik_X       A1

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(button_a_pin, INPUT);
  pinMode(button_b_pin, INPUT);
  pinMode(button_c_pin, INPUT);
  pinMode(button_d_pin, INPUT);

  pinMode(joystik_Y, INPUT);
  pinMode(joystik_X, INPUT);
}

void loop() {
  
  Serial.print(digitalRead(button_a_pin),DEC);
  Serial.print(digitalRead(button_b_pin),DEC);
  Serial.print(digitalRead(button_c_pin),DEC);
  Serial.print(digitalRead(button_d_pin),DEC);


  Serial.print( char( analogRead(joystik_X)/8 ) );
  Serial.print( char( analogRead(joystik_Y)/8 ) );
  
  Serial.println();
}
