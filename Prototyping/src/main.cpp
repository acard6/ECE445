#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>


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

  // put function declarations here:
  // int myFunction(int, int);
  void butt_count();

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
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);


    Serial.print("humidity is(%):");
    Serial.print(h);
    Serial.print(" %\n");
    Serial.print("temperatire is(%):");
    Serial.print(t);
    Serial.print(" *C\t");
    Serial.print(f);
    Serial.print(" *F\n");

    if (t > 18){
      digitalWrite(TEMP_LED, HIGH);
    }
    else{
      digitalWrite(TEMP_LED, LOW);
    }

    h *= 100;
    if(h < 20){
      digitalWrite(ORANGE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(BLUE, HIGH);
    }

    else if(20 <= h < 40){
      digitalWrite(ORANGE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(WHITE, HIGH);
      digitalWrite(BLUE, LOW);
    }

    else if(40 <= h < 60){
      digitalWrite(ORANGE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, HIGH);
      digitalWrite(WHITE, LOW);
      digitalWrite(BLUE, LOW);
    }

    else if(60 <= h < 80){
      digitalWrite(ORANGE, LOW);
      digitalWrite(GREEN, HIGH);
      digitalWrite(YELLOW, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(BLUE, LOW);
    }

    else if(80 <= h){
      digitalWrite(ORANGE, LOW);
      digitalWrite(GREEN, LOW);
      digitalWrite(YELLOW, LOW);
      digitalWrite(WHITE, LOW);
      digitalWrite(BLUE, HIGH);
    }
  }



// put function definitions here:
/*
int myFunction(int x, int y) {
  return x + y;
}
*/

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
