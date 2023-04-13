#include "StationMeteo.h"

//Ici on parle d'heure lorsque un temps est figé, et de temps pour une duree

//Variables globales :
uint16_t g_ui16_time_start=0;   //Heure de départ du programme

// Variables – Température
float g_float_temp=0;   //Température actuelle
float g_float_tab_temp[];   //5 dernières températures
float g_float_tab_tempmoy[];    //2 dernières moyennes de 5 températures cf. Annexe 1  
T_Tendance g_tendance_temp;   //tendance de la température (Stable / Hausse / Baisse)

// Variables – Luminosité
float g_float_luminosite=0;   //Luminosité actuelle

// Variables – Intensité lumineuse
float g_float_intensitelum=0;    //Intensité lumineuse actuelle

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



  //A implémenter aux autres fx
  /*
    recupTemperature();
    //décalage des anciennes tmepérature
    //transformer en boucle for
    g_float_tab_temp[4]=g_float_tab_temp[3];
    g_float_tab_temp[3]=g_float_tab_temp[2];
    g_float_tab_temp[2]=g_float_tab_temp[1];
    g_float_tab_temp[1]=g_float_tab_temp[0];
    g_float_tab_temp[0]=g_float_temp;
    
    float l_float_sum = 0;
    for(iBcl=0;iBcl<5;iBcl++){
      l_float_sum += g_float_tab_temp[iBcl];
    }

    g_float_tab_tempmoy[1] = g_float_tab_tempmoy[0];
    g_float_tab_tempmoy[0] = l_float_sum/5;

    g_float_ratio = g_float_tab_tempmoy[1]/g_float_tab_tempmoy[0];  //Calcul du rapport entre les 2 moyennes précédentes

    //Si le ratio est aux alentours de 1, alors tendance stable, si ratio supérieur, alors tendance à la baisse, si ratio inférieur, alors tendance à la hausse
    if (g_float_ratio>1.02){
      //tendance à la baisse
      g_tendance_temp=BAISSE;
    }

    else if (g_float_ration<0.98){
      //tendance à la hausse
      g_tendance_temp=HAUSSE;
    }

    else{
      //tendance stable
      g_tendance_temp=STABLE;
    }
  */


}
