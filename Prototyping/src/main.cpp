#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <cstdint>
#include <driver/uart.h>
#include <string.h>
#include <time.h>

  #define ORANGE 1
  #define GREEN 2
  #define YELLOW 42
  #define WHITE 41
  #define BLUE 48

  #define BUTTON 4 
  #define DHTPIN 40
  #define DHTTYPE DHT22

  #define TEMP_LED 47

  #define MIN_INTERVAL  2000
  #define TIMEOUT UINT32_MAX

  #define BUF_SIZE 1024
  // put function declarations here:
  // int myFunction(int, int);
  



  void butt_count();
  void hum_to_led(uint8_t h);

  int counter = 0x0;
  DHT dht(DHTPIN, DHTTYPE);

  void setup() {
    // put your setup code here, to run once:
    // int result = myFunction(2, 3);
    pinMode(ORANGE, OUTPUT);
    pinMode(GREEN,  OUTPUT);
    pinMode(YELLOW, OUTPUT);
    pinMode(WHITE,  OUTPUT);
    pinMode(BLUE,   OUTPUT);

    pinMode(TEMP_LED, OUTPUT);
    pinMode(BUTTON, INPUT);
    Serial.begin(9600);
    Serial.println("DHTxx test!");

    dht.begin();
  }

  void loop() {
    // void butt_count();
    delay(500);
    time_t start = time(NULL);
    float H = dht.readHumidity();
    time_t end = time(NULL);
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    Serial.println(end - start);

    while (Serial.available() == 0) {
    }

    int read = Serial.parseInt();
    if (read == 1){

      Serial.print("humidity is(%):");
      Serial.print(H);
      Serial.print(" %\t");
      Serial.print("temperatire is(%):");
      Serial.print(t);
      Serial.print(" *C\t");
      Serial.print(f);
      Serial.print(" *F\n");    
    
      H /= 3;
      uint8_t h = (uint8_t)H;
      hum_to_led(h);
      
      digitalWrite(TEMP_LED, HIGH);

      delay(10000);
    }
    else{
      digitalWrite(TEMP_LED, LOW);
      digitalWrite(BLUE, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(ORANGE, LOW);
      digitalWrite(YELLOW, LOW);
      
    }
  }

// put function definitions here:
/*
int myFunction(int x, int y) {
  return x + y;
}
*/
    void hum_to_led(uint8_t h){
      if ((h&1) == 1)
        digitalWrite(ORANGE, HIGH);
      else
        digitalWrite(ORANGE, LOW);
    
      if (((h>>1)&1) == 1)
        digitalWrite(GREEN, HIGH);
      else
        digitalWrite(GREEN, LOW);

      if (((h>>2)&1) == 1)
        digitalWrite(YELLOW, HIGH);
      else
        digitalWrite(YELLOW, LOW);

      if (((h>>3)&1) == 1)
        digitalWrite(WHITE, HIGH);
      else
        digitalWrite(WHITE, LOW);

      if (((h>>4)&1) == 1)
        digitalWrite(BLUE, HIGH);
      else
        digitalWrite(BLUE, LOW);
    }

  void butt_count(){
    if (digitalRead(BUTTON) == HIGH){
      counter++;
      Serial.println(counter);
    }

    if ((counter % 2)){
      digitalWrite(ORANGE, HIGH);
    }else{
      digitalWrite(ORANGE, LOW);
    }


    if (((counter >> 1) % 2)){
      digitalWrite(GREEN, HIGH);
    }else{
      digitalWrite(GREEN, LOW);
    }

    if (((counter >> 2) % 2)){
      digitalWrite(YELLOW, HIGH);
    }else{
      digitalWrite(YELLOW, LOW);
    }

    if (((counter >> 3) % 2)){
      digitalWrite(WHITE, HIGH);
    }else{
      digitalWrite(WHITE, LOW);
    }

    if (((counter >> 4) % 2)){
      digitalWrite(BLUE, HIGH);
    }else{
      digitalWrite(BLUE, LOW);
    }

    if (counter >= 32){counter = 0;}
  }
