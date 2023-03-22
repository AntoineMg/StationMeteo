#include "StationMeteo.h"

//Variables globales :
float g_float_temp=0;


void setup() {
  Serial.begin(9600);
  DDRC = 0x00; //B0000 0000 =>Tous les ports C sont des inputs
}

void loop() {
  recupTemperature();
  Serial.println(g_float_temp);
}
