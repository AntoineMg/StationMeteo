#include "StationMeteo.h"

//Variables globales :
T_Etat g_enum_Etat=TEMP;    //Etat par défaut = température
uint16_t g_ui16_time_start=0;   //Heure de départ du programme
bool g_bool_BP;

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
float g_float_vitesse_vent=0;    //Vitesse du vent actuelle
float g_float_tab_vitesse_vent[5];  //5 dernières vitesses du vent
float g_float_vitesse_vent_moy;   // moyenne de 5 vitesses du vent
float g_float_vitesse_vent_min=9999999;  //Vitesse du vent minimale depuis le début
float g_float_vitesse_vent_max=0;   //Vitesse du vent maximale depuis le début
T_Tendance g_tendance_vitesse_vent;   //tendance de la vitesse du vent (Stable / Hausse / Baisse)

//Ecran
// create an instance of the library
TFT TFTscreen = TFT(PIN_SCREEN_CS, PIN_SCREEN_DC, PIN_SCREEN_RST);
char temperature[8];


void setup() {
  Serial.begin(9600);
  DDRC = 0x00; //B0000 0000 =>Tous les ports C sont des inputs
  DDRD = 0x03;

  //Registres pour la mesure de fréquence
  TCCR1A=0x00; 
  TCCR1B=0x07;  //Config afin de compter les fronts montants sur la tension externe
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
  //nb : on ajoute un léger délai entre deux mesures utilisant le CAN, pour éviter les problèmes de fausse mesures qu'on a eu auparavant
  //nb : on les mets dans le main afin de mieux visualiser le déroulement du programme et les délais
  recupTemperature();
  delay(20);
  recupLuminosite();
  delay(20);
  recupIntensiteLumineuse();
  delay(20);
  recupVitesseVent();

  //Essais du TFT
  /*
  TFTscreen.text("Temperature :", 0, 0);
  String str_temp=String(g_float_temp);
  str_temp.toCharArray(temperature, 8);
  TFTscreen.text(temperature, 6, 57);
  delay(3000);
  TFTscreen.background(0, 0, 0);
  */

  //Lecture des Entrées
  //Etat du bouton cablé sur PORT D1 = PD+
  g_bool_BP = ((PIND & PIN_BP) == PIN_BP);

  //MAJ de l'état
    switch (g_enum_Etat){
      case TEMP :
        if(g_bool_BP == true){
            g_enum_Etat = ATT_LUMI;
        }
        break;
      
      case ATT_LUMI :
        if(g_bool_BP == false){
            g_enum_Etat = LUMI;
        }
        break;

      case LUMI :
        if(g_bool_BP == true){
            g_enum_Etat = ATT_INTLUM;
        }
        break;
      
      case ATT_INTLUM :
        if(g_bool_BP == false){
            g_enum_Etat = INTLUM;
        }
        break;
      
      case INTLUM :
        if(g_bool_BP == true){
            g_enum_Etat = ATT_VENT;
        }
        break;
      
      case ATT_VENT :
        if(g_bool_BP == false){
            g_enum_Etat = VENT;
        }
        break;
      
      case VENT :
        if(g_bool_BP == true){
            g_enum_Etat = ATT_TEMP;
        }
        break;
      
      case ATT_TEMP :
        if(g_bool_BP == false){
            g_enum_Etat = TEMP;
        }
        break;

    }   //Fin du switch

  //MAJ des Sorties en fonction de l'état

    switch(g_enum_Etat){

        case TEMP :
          afficheTemperature();
          break;
        
        case LUMI :
          afficheLuminosite();
          break;
        
        case INTLUM :
          afficheIntensiteLumineuse();
          break;
        
        case VENT :
          afficheVitesseVent();
          break;
        
    }

  Serial.println(g_bool_BP);


}
