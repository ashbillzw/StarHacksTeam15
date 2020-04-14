#include <Arduino_FreeRTOS.h>

int toggle3 = 0;
int toggle8 = 0;
int buzz_count = 0;
int bf = 0;
int* buzz_freq = &bf;
char incomingByte = 0;


void togglePin(int pin, int* toggle){
  if (*toggle){
    digitalWrite(pin ,LOW);
    *toggle = 0;
  }
  else{
    digitalWrite(pin ,HIGH);
    *toggle = 1;
  }
}


void TaskBlink(){
  while(1){
    togglePin(3, &toggle3);
    vTaskDelay(20 / portTICK_PERIOD_MS);
    togglePin(3, &toggle3);
    vTaskDelay(1980 / portTICK_PERIOD_MS);
  }
}


ISR(TIMER1_COMPA_vect){
  if(*buzz_freq != 0){
    if(buzz_count > 25000 / *buzz_freq){
     buzz_count = 0;
     togglePin(8, &toggle8);
    }
    else{
      buzz_count++;
    }
  }
}


void setup() {
  Serial.begin(9600);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  analogWrite(5, 50);
  
  cli();
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;
  OCR1A = 1;
  TCCR1B |= (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  TIMSK1 |= (1 << OCIE1A);
  sei();

  xTaskCreate(TaskBlink, "blink", 128, NULL, 2, NULL);
}


void a(){
  analogWrite(5, 50);
  digitalWrite(4, HIGH);
  *buzz_freq = 6000;
  delay(200);
  *buzz_freq = 4000;
  delay(200);
  *buzz_freq = 2000;
  delay(200);
  *buzz_freq = 0;
  digitalWrite(4, LOW);
}


void b(){
  analogWrite(5, 200);
  digitalWrite(4, HIGH);
  *buzz_freq = 2000;
  delay(200);
  *buzz_freq = 4000;
  delay(200);
  *buzz_freq = 6000;
  delay(200);
  *buzz_freq = 0;
  digitalWrite(4, LOW);
}


void loop() {
  if (Serial.available() > 0) {
    incomingByte = Serial.read();
    if(incomingByte == 'a'){
     Serial.println(">>a");
     a();
    }
    if(incomingByte == 'b'){
     Serial.println(">>b");
     b();
    }
  }
}
