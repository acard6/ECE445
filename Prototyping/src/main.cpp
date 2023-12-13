#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <cstdint>
#include <driver/uart.h>
#include <string.h>
// #include <time.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"


#define BAUD_RATE 115200

// #define RELAY 6
// #define BUTTON 4 
#define DHTTYPE DHT22

//deinfing GPIO pins and their use
#define SO      4     /* data input from MAX31855 temperature sensors   : input  */
#define SCK     5     /* source clock for MAX31855 temp sensors         : output */
#define A       6     /* bit selector for MUX                           : output */
#define B       7     /* bit selector for MUX                           : output */
#define C       15    /* bit selector for MUX                           : output */
#define G2A     16    /* enable on MUX                                  : output */
#define G2B     17    /* enable on MUX                                  : output */
#define G1      18    /* enable on MUX                                  : output */
#define CS9     8     /* CS for 9 temp sensor not accessable by MUX     : output */

#define DHTPIN1 19    /* humidity sensor 1 data bus                     : input  */
#define DHTPIN2 20    /* humidity sensor 2 data bus                     : input  */

#define SUB1    10    /* pin to power board                             : I/O    */
#define SUB2    11    /* pin to power board                             : I/O    */
#define SUB3    12    /* pin to power board                             : I/O    */
#define SUB4    13    /* pin to power board                             : I/O    */
#define SUB5    14    /* pin to power board                             : I/O    */
#define SUB6    21    /* pin to power board                             : I/O    */
#define SUB7    47    /* pin to power board                             : I/O    */
#define SUB8    48    /* pin to power board                             : I/O    */
#define dummy   42    /* dummy pin for CS in temp sensors with MUX      : output */

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

Adafruit_MAX31855 temp9(SCK, CS9, SO);
Adafruit_MAX31855 MUX(SCK, G2B, SO);

double temp_c;
//  _________         _________ 
//  | 13  14 |        |  4   5 |
//  | 12  21 |        |  3   6 |
//  | 11  47 |  --->  |  2   7 |
//  | 10  48 |        |  1   8 |
//  | 5V  5V |        | 5V  5V |
//  |________|        |________|
int arr[8] = {SUB1, SUB2, SUB3, SUB4, SUB5, SUB6, SUB7, SUB8};
void setup() {
  Serial.begin(BAUD_RATE);
  /* -----set pins to I/O----- */
  pinMode(SO, INPUT);      // input         data bus for temp sensors
  pinMode(SCK, OUTPUT);    // output        clock bus for temp sensors
  // pinMode(CS9, OUTPUT);    // output        chip select for temp sensor not connected by mux
  pinMode(A, OUTPUT);      // output        3:8 mux chip select for temp sensors
  pinMode(B, OUTPUT);      // output        3:8 mux chip select for temp sensors
  pinMode(C, OUTPUT);      // output        3:8 mux chip select for temp sensors
  pinMode(G1, OUTPUT);     // output        3:8 mux enables for temp sensors
  pinMode(G2B, OUTPUT);    // output        3:8 mux enables for temp sensors
  pinMode(G2A, OUTPUT);    // output        3:8 mux enables for temp sensors

  pinMode(DHTPIN1, INPUT); // input         humidity sensors
  pinMode(DHTPIN2, INPUT); // input         humidity sensors
  pinMode(SUB1, INPUT);    // input/output  pinhears on main board
  pinMode(SUB2, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB3, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB4, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB5, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB6, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB7, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB8, OUTPUT);   // input/output  pinhears on main board
  /* -----initialize out pins----- */
  digitalWrite(G1, HIGH);   // based on sn74hc138 data sheet so that no sensors has cs enabled
  digitalWrite(G2B, HIGH);   // based on sn74hc138 data sheet so that no sensors has cs enabled
  digitalWrite(G2A, LOW);   // based on sn74hc138 data sheet so that no sensors has cs enableds
  Serial.println("Flashed & Running.");
  digitalWrite(SUB2, HIGH);
  dht1.begin();
  // dht2.begin();
  temp9.begin();    // U13 sensor
  // MUX.begin();    // U12 sensor
}

void loop() {

  int read = Serial.parseInt();

  /* get humidity sensor data */
  if(digitalRead(SUB1)){digitalWrite(SUB5, LOW);}
  else{digitalWrite(SUB5, HIGH);}

  if (read == 8){
    double H = getHumData();
    Serial.write((byte) H); //j added this
  }

  /* get lone thermocouple */
  if (read == 9){
    temp_c = temp9.readCelsius();       // temp sensor U13 temp reading
    temp_c = temp9.readCelsius();       // temp sensor U13 temp reading
    temp_c = temp9.readCelsius();       // temp sensor U13 temp reading
    double inter = temp9.readInternal();
    Serial.print("temp sensor:\t temp:");
    Serial.print(temp_c);
    Serial.println(" *C");
    Serial.print("internal:\t");
    Serial.print(inter);
    Serial.println(" *C");
  }
  /* get thermocouples in MUX data */
  if (10 <= read && read <18){
    uint8_t i = read % 10;
    setMuxChannel(i);
    //delay(5);
    temp_c = MUX.readCelsius();
    temp_c = MUX.readCelsius();
    temp_c = MUX.readCelsius();
    digitalWrite(G2B, HIGH);
    printTemp(temp_c, i);
    delay(100);
  }


  /* set pins to power */
  if (30 <= read && read < 50){
    powerBoard(read);
  }

  delay(100);
}

double getHumData(){
  double H = dht1.readHumidity();        // humidity sensor J10 humidity reading
  double t = dht1.readTemperature();     // humidity sensot J10 temp reading in *C
 
  Serial.print("humidity:");
  Serial.print(H);
  Serial.print(" %\t temp:");
  Serial.print(t);
  Serial.println(" *C");
  return H;
}



void powerBoard(int read){
  int i, port, val;
  if (read >= 30){
    if (read < 40){
      i = read % 10;
      port = arr[i];
      val = HIGH;
    }
  }
  if (read >= 40){
    if (read < 50){
      i = read % 10;
      port = arr[i];
      val = LOW;
    }
  }
  digitalWrite(port, val);
  Serial.print("port: ");
  Serial.print(port);
  Serial.print(" is ");
  Serial.println(val);
}

/* Set MUX to select the desired channel */
void setMuxChannel(int channel) {
  digitalWrite(A, (channel & 1));
  digitalWrite(B, ((channel >> 1) & 1));
  digitalWrite(C, ((channel >> 2) & 1));
  digitalWrite(G2B, LOW);
  //delay(100);
}

/* Print temperature values from MUX thermos  */
void printTemp(double temp_c, int i){
  Serial.print("MUX ");
  Serial.print(i);
  Serial.print("\t temp: ");
  Serial.print(temp_c);
  Serial.println(" *C");
  delay(10);
}

