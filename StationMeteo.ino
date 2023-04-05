#include "StationMeteo.h"

//Variables globales :
float g_float_temp=0;
float g_float_luminosite=0;
uint16_t g_ui16_time_start=0;


void setup() {
  Serial.begin(9600);
  DDRC = 0x00; //B0000 0000 =>Tous les ports C sont des inputs
  TCCR1A=0x00;
  TCCR1B=0x07;
  TCCR1C=0x00;
  g_ui16_time_start=millis();
}

void loop() {
  /*
  recupTemperature();
  Serial.print("Température : ");
  Serial.println(g_float_temp);
  delay(1000);

  recupLuminosite();
  Serial.print("Luminosité : ");
  Serial.println(g_float_luminosite);
  delay(1000);

  recupIntensiteLumineuse();
  */

  recupVitesseVent();


}
