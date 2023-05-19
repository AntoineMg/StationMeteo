#include "StationMeteo.h"

//Ici on parle d'heure lorsque un temps est figé, et de temps pour une duree

//Variables globales :
uint16_t g_ui16_time_start=0;   //Heure de départ du programme

// Variables – Température
float g_float_temp=0;   //Température actuelle
float g_float_tab_temp[5];   //5 dernières températures
float g_float_tempmoy;    //moyenne de 5 températures
float g_float_temp_min=60;   //Température minimale depuis le début
float g_float_temp_max=0;   //Température maximale depuis le début
T_Tendance g_tendance_temp;   //tendance de la température (Stable / Hausse / Baisse)

// Variables – Luminosité
float g_float_luminosite=0;   //Luminosité actuelle
float g_float_tab_luminosite[5];  //5 dernières luminosités
float g_float_luminositemoy;   // moyenne de 5 luminosités 
float g_float_luminosite_min=999;   //Luminosité minimale depuis le début
float g_float_luminosite_max=0;   //Luminosité maximale depuis le début
T_Tendance g_tendance_luminosite;   //tendance de la luminosité (Stable / Hausse / Baisse)

// Variables – Intensité lumineuse
float g_float_intensitelum=0;    //Intensité lumineuse actuelle
float g_float_tab_intensitelum[5];  //5 dernières intensités lumineuses
float g_float_intensitelum_moy;   // moyenne de 5 intensités lumineuses
float g_float_intensitelum_min=9999999;  //Intensité lumineuse minimale depuis le début
float g_float_intensitelum_max=0;   //Intensité Lumineuse maximale depuis le début
T_Tendance g_tendance_intensitelum;   //tendance de l'intensité lumineuse (Stable / Hausse / Baisse)

// Variables – Vent

//Ecran
// create an instance of the library
TFT TFTscreen = TFT(PIN_SCREEN_CS, PIN_SCREEN_DC, PIN_SCREEN_RST);
char temperature[8];


void setup() {
  Serial.begin(9600);
  DDRC = 0x00; //B0000 0000 =>Tous les ports C sont des inputs

  //Registres pour la mesure de fréquence
  TCCR1A=0x00; 
  TCCR1B=0x07;  //Congig afin de compter les fronts montants sur la tension externe
  TCCR1C=0x00;

  //initialisation de l'heure de depart du programme
  g_ui16_time_start=millis();

  //Initialisation de l'écran
  TFTscreen.begin();

  //Reset de l'affichage
  TFTscreen.background(0, 0, 0);

  //Definition taille du texte sur 1
  TFTscreen.setTextSize(2);
  TFTscreen.stroke(255,0,0);

}

void loop() {
  
  //recuperation des valeurs
  recupTemperature();
  delay(20);
  recupLuminosite();
  delay(20);
  recupIntensiteLumineuse();
  delay(20);
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
  Serial.println(g_float_tempmoy);
  Serial.print("Tendance : ");
  Serial.println(g_tendance_temp);
  Serial.println("================================================");

  //Affichage des Luminosites
  Serial.println("Luminosités : ");
  Serial.print("Actuelle : ");
  Serial.println(g_float_luminosite);
  Serial.print("Min : ");
  Serial.println(g_float_luminosite_min);
  Serial.print("Max : ");
  Serial.println(g_float_luminosite_max);
  Serial.print("Moy : ");
  Serial.println(g_float_luminositemoy);
  Serial.print("Tendance : ");
  Serial.println(g_tendance_luminosite);
  Serial.println("================================================");


   //Affichage des Intensités Lumineuses
  Serial.println("Intensité lumineuses : ");
  Serial.print("Actuelle : ");
  Serial.println(g_float_intensitelum);
  Serial.print("Min : ");
  Serial.println(g_float_intensitelum_min);
  Serial.print("Max : ");
  Serial.println(g_float_intensitelum_max);
  Serial.print("Moy : ");
  Serial.println(g_float_intensitelum_moy);
  Serial.print("Tendance : ");
  Serial.println(g_tendance_intensitelum);
  Serial.println("================================================");

  TFTscreen.text("Temperature :", 0, 0);
  String str_temp=String(g_float_temp);
  str_temp.toCharArray(temperature, 8);
  TFTscreen.text(temperature, 6, 57);
  delay(3000);
  TFTscreen.background(0, 0, 0);

}
