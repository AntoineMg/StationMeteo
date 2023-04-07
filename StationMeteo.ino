#include "StationMeteo.h"

//Ici on parle d'heure lorsque un temps est figé, et de temps pour une duree

//Variables globales :
float g_float_temp=0;   //Température
float g_float_luminosite=0;   //Luminosité
float g_float_intensitelum=0;    //Intensité lumineuse
uint16_t g_ui16_time_start=0;   //Heure de départ du programme


void setup() {
  Serial.begin(9600);
  DDRC = 0x00; //B0000 0000 =>Tous les ports C sont des inputs

  //Registres pour la mesure de fréquence
  TCCR1A=0x00; 
  TCCR1B=0x07;  //Congig afin de compter les fronts montants sur la tension externe
  TCCR1C=0x00;

  //initialisation de l'heure de depart du programme
  g_ui16_time_start=millis();
}

void loop() {
  
  //recuperation et affichage de la température
  recupTemperature();
  Serial.print("Température : ");
  Serial.println(g_float_temp);
  delay(1000);

  //recuperation et affichage de la luminosité
  recupLuminosite();
  Serial.print("Luminosité : ");
  Serial.println(g_float_luminosite);
  delay(1000);

  //recuperation et affichage de l'intensité lumineuse
  Serial.print("Intensité lumineuse :");
  recupIntensiteLumineuse();
  
  //recuperation et affichage de la vitesse du vent
  recupVitesseVent();


}
