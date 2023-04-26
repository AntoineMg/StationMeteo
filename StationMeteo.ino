#include "StationMeteo.h"

//Ici on parle d'heure lorsque un temps est figé, et de temps pour une duree

//Variables globales :
uint16_t g_ui16_time_start=0;   //Heure de départ du programme

// Variables – Température
float g_float_temp=0;   //Température actuelle
float g_float_tab_temp[5];   //5 dernières températures
float g_float_tab_tempmoy[2];    //2 dernières moyennes de 5 températures cf. Annexe 1  
float g_float_temp_min=0;   //Température minimale depuis le début
float g_float_temp_max=0;   //Température maximale depuis le début
T_Tendance g_tendance_temp;   //tendance de la température (Stable / Hausse / Baisse)

// Variables – Luminosité
float g_float_luminosite=0;   //Luminosité actuelle
float g_float_tab_luminosite[5];  //5 dernières luminosités
float g_float_tab_luminositemoy[2];   //2 dernières moyennes de 5 luminosités cf. Annexe 1
T_Tendance g_tendance_luminosite;   //tendance de la luminosité (Stable / Hausse / Baisse)

// Variables – Intensité lumineuse
float g_float_intensitelum=0;    //Intensité lumineuse actuelle
float g_float_tab_intensitelum[5];  //5 dernières intensités lumineuses
float g_float_tab_intensitelummoy[2];   //2 dernières moyennes de 5 intensités lumineuses cf. Annexe 1
T_Tendance g_tendance_intensitelum;   //tendance de l'intensité lumineuse (Stable / Hausse / Baisse)

// Variables – Vent


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
  
  //recuperation des valeurs
  recupTemperature();
  recupLuminosite();
  recupIntensiteLumineuse();
  recupVitesseVent();


  //Affichage des températures
  Serial.println("Températures : ");
  Serial.print("Actuelle : ");
  Serial.println(g_float_temp);
  Serial.print("Min : ");
  Serial.println(g_float_temp_min);
  Serial.print("Max : ");
  Serial.println(g_float_temp_max);
  Serial.print("Moy : ");
  Serial.println(g_float_tab_tempmoy[0]);
  Serial.print("Tendance : ");
  Serial.println(g_tendance_temp);

  Serial.write(27);       // ESC command
  Serial.print("[2J");    // clear screen command
  Serial.write(27);
  Serial.print("[H");     // cursor to home command

}
