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
Adafruit_MAX31855 temp7(SCK, dummy, SO);
// Adafruit_MAX31855 temp6(SCK, dummy, SO);
// Adafruit_MAX31855 temp5(SCK, dummy, SO);
// Adafruit_MAX31855 temp4(SCK, dummy, SO);
// Adafruit_MAX31855 temp3(SCK, dummy, SO);
// Adafruit_MAX31855 temp2(SCK, dummy, SO);
// Adafruit_MAX31855 temp1(SCK, dummy, SO);
// Adafruit_MAX31855 temp0(SCK, dummy, SO);
//  _________         _________ 
//  | 13  14 |        |  4   5 |
//  | 12  21 |        |  3   6 |
//  | 11  47 |  --->  |  2   7 |
//  | 10  48 |        |  1   8 |
//  | 5V  5V |        | 5V  5V |
//  |_______ |        |_______ |
int arr[8] = {SUB1, SUB2, SUB3, SUB4, SUB5, SUB6, SUB7, SUB8};
void setup() {
  Serial.begin(BAUD_RATE);
  /* -----set pins to I/O----- */
  pinMode(SO, INPUT);      // input         data bus for temp sensors
  pinMode(SCK, OUTPUT);    // output        clock bus for temp sensors
  pinMode(CS9, OUTPUT);    // output        chip select for temp sensor not connected by mux
  pinMode(A, OUTPUT);      // output        3:8 mux chip select for temp sensors
  pinMode(B, OUTPUT);      // output        3:8 mux chip select for temp sensors
  pinMode(C, OUTPUT);      // output        3:8 mux chip select for temp sensors
  pinMode(G1, OUTPUT);     // output        3:8 mux enables for temp sensors
  pinMode(G2B, OUTPUT);    // output        3:8 mux enables for temp sensors
  pinMode(G2A, OUTPUT);    // output        3:8 mux enables for temp sensors

  pinMode(DHTPIN1, INPUT); // input         humidity sensors
  pinMode(DHTPIN2, INPUT); // input         humidity sensors
  pinMode(SUB1, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB2, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB3, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB4, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB5, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB6, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB7, OUTPUT);   // input/output  pinhears on main board
  pinMode(SUB8, OUTPUT);   // input/output  pinhears on main board
  /* -----initialize out pins----- */
  digitalWrite(G1, HIGH);   // based on sn74hc138 data sheet so that no sensors has cs enabled
  digitalWrite(G2B, LOW);   // based on sn74hc138 data sheet so that no sensors has cs enabled
  digitalWrite(G2A, LOW);   // based on sn74hc138 data sheet so that no sensors has cs enableds
  Serial.println("Flashed & Running.");
  dht1.begin();
  // dht2.begin();
  temp9.begin();    // U13 sensor
  temp7.begin();    // U12 sensor
//  temp6.begin();    // U11 sensor
//  temp5.begin();    // U10 sensor
//  temp4.begin();    // U9 sensor
//  temp3.begin();    // U8 sensor
//  temp2.begin();    // U7 sensor
//  temp1.begin();    // U6 sensor
//  temp0.begin();    // U5 sensor
}

void loop() {

  int read = Serial.parseInt();

  /* get humidity sensor data */
  if (read == 8){
    getHumData();
  }

  /* get lone thermocouple */
  if (read == 9){
    double c = temp9.readCelsius();       // temp sensor U13 temp reading
    Serial.print("temp sensor:\t temp:");
    Serial.print(c);
    Serial.println(" *C");
  }
  /* get thermocouples in MUX data */
  if (10 <= read && read <18){
    uint8_t i = read % 10;
    float temp_c;
    setMuxChannel(i);
    if (i == 0){
      // temp_c = temp0.readCelsius();
    }
    if (i == 1){
      //temp_c = temp1.readCelsius();
    }
    if (i == 2){
      //temp_c = temp2.readCelsius();
    }
    if (i == 3){
      //temp_c = temp3.readCelsius();
    }
    if (i == 4){
      //temp_c = temp4.readCelsius();
    }
    if (i == 5){
      //temp_c = temp5.readCelsius();
    }
    if (i == 6){
      //temp_c = temp6.readCelsius();
    }
    if (i == 7){
      //temp_c = temp7.readCelsius();
    }
    digitalWrite(G2B, HIGH);
    printTemp(temp_c, i);
  }


  /* set pins to power */
  if (30 <= read && read < 50){
    powerBoard(read);
  }

}

void getHumData(){
  float H = dht1.readHumidity();        // humidity sensor J10 humidity reading
  float t = dht1.readTemperature();     // humidity sensot J10 temp reading in *C
 
  Serial.print("humidity:");
  Serial.print(H);
  Serial.print(" %\t temp:");
  Serial.print(t);
  Serial.println(" *C");
  
}


/* Set pins for the power subsystem board (note: works) */
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
  digitalWrite(G2B, LOW);
  digitalWrite(A, (channel & 1));
  digitalWrite(B, ((channel >> 1) & 1));
  digitalWrite(C, ((channel >> 2) & 1));
}

/* Print temperature values from MUX thermos  */
void printTemp(int temp_c, int i){
  Serial.println("temp reading with MUX:");
  Serial.print("Thermoucouple ");
  Serial.print(i);
  Serial.print(" temp: ");
  Serial.print(temp_c);
  Serial.println(" *C");
  delay(10);
}

