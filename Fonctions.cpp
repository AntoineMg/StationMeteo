#pragma once
#include "StationMeteo.h"

//Variables globales :
extern uint16_t g_ui16_time_start;   //Heure de départ du programme

// Variables – Température
extern float g_float_temp;   //Température actuelle
extern float g_float_tab_temp[];   //5 dernières températures
extern float g_float_tab_tempmoy[];    //2 dernières moyennes de 5 températures cf. Annexe 1  
extern T_Tendance g_tendance_temp;   //tendance de la température (Stable / Hausse / Baisse)
extern float g_float_temp_min;    //température minimale depuis le début
extern float g_float_temp_max;    //température maximale depuis le début

// Variables – Luminosité
extern float g_float_luminosite;   //Luminosité actuelle
extern float g_float_tab_luminosite[];    //5 dernières luminosités
extern float g_float_tab_luminositemoy[];     //2 dernières moyennes de 5 luminosités cf. Annexe 1
extern T_Tendance g_tendance_luminosite;    //tendance de la luminosité (Stable / Hausse / Baisse)

// Variables – Intensité lumineuse
extern float g_float_intensitelum;    //Intensité lumineuse actuelle
extern float g_float_tab_intensitelum[];   //5 dernières intensités lumineuse
extern float g_float_tab_intensitelummoy[];    //2 dernières moyennes de 5 intensités lumineuses cf. Annexe 1
extern T_Tendance g_tendance_intensitelum;    //tendance des intensités lumineuses (Stable / Hausse / Baisse)

// Variables – Vent
extern float g_float_vitesse_vent;    //Vitesse du vent actuelle
extern float g_float_tab_vitesse_vent;   //5 dernières vitesses du vent
extern float g_float_tab_vitesse_vent_moy;    //2 dernières moyennes de 5 vitesses du vent cf. Annexe 1
extern T_Tendance g_tendance_vitesse_vent[];    //tendance des vitesses du vent (Stable / Hausse / Baisse)


//Initialisation du CAN sur le portC X (doit être situé sur le port C)
void InitCan(uint8_t x_ui8_portConv){
	ADMUX = 0x40 | x_ui8_portConv; // Configure ADMUX pour que la conversion se fasse sur le port sélectionné
	ADCSRA = 0x84; // Active l'ADC et set division factor to 16
}

//Conversion Analogique-Numerique, retourne un int 0-1023 (src = tp4)
uint16_t ConvAn(void) {
	uint8_t ui8ResH;
	uint8_t ui8ResL;
	uint16_t ui16RetRes;

	ADCSRA = ADCSRA | MSQ_ADC;
	do {} while ((ADCSRA & MSQ_ADC) == MSQ_ADC);

	ui8ResL = ADCL;
	ui8ResH = ADCH;
	ui16RetRes = ((uint16_t)ui8ResH << 8) + (uint16_t)ui8ResL;
	return ui16RetRes;
}

//Recupération de la température et stockage de celle ci dans la variable globale g_float_temp
//Calcul de la moyenne des 5 dernières mesures et stockage de celle-ci dands le tableau g_float_tab_tempmoy[0]
//Calcul de la tendance, en comparant les 2 dernières moyennes, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_temp
void recupTemperature(void){
  uint16_t l_ui16_resultatConv;
  float l_float_sum = 0;
  float l_float_ratio=1;

  //initialisation du CAN sur le port A0
  InitCan(PIN_TEMPERATURE);

  //Conversion AN
  l_ui16_resultatConv = ConvAn();

  //Conversion du résultat CAN en température
  g_float_temp = l_ui16_resultatConv*0.488;

  //décalage des anciennes tmepérature
  //transformer en boucle for
  g_float_tab_temp[4]=g_float_tab_temp[3];
  g_float_tab_temp[3]=g_float_tab_temp[2];
  g_float_tab_temp[2]=g_float_tab_temp[1];
  g_float_tab_temp[1]=g_float_tab_temp[0];
  g_float_tab_temp[0]=g_float_temp;
  
  //additionne les 5 dernières températures
  for(int iBcl=0;iBcl<5;iBcl++){
    l_float_sum += g_float_tab_temp[iBcl];
  }

  g_float_tab_tempmoy[1] = g_float_tab_tempmoy[0];   //décalge de la moyenne précédente
  g_float_tab_tempmoy[0] = l_float_sum/5;   //stockage de la moyenne

  l_float_ratio = g_float_tab_tempmoy[1]/g_float_tab_tempmoy[0];  //Calcul du rapport entre les 2 moyennes précédentes

  //Si le ratio est aux alentours de 1, alors tendance stable, si ratio supérieur, alors tendance à la baisse, si ratio inférieur, alors tendance à la hausse
  if (l_float_ratio>1.02){
    //tendance à la baisse
    g_tendance_temp=BAISSE;
  }

  else if (l_float_ratio<0.98){
    //tendance à la hausse
    g_tendance_temp=HAUSSE;
  }

  else{
    //tendance stable
    g_tendance_temp=STABLE;
  }

  //actualisation de la température minimale si elle est dépassée
  if (g_float_temp<g_float_temp_min){
    g_float_temp_min = g_float_temp;
  }

  //actualisation de la température maximale si elle est dépassée
  if (g_float_temp>g_float_temp_max){
    g_float_temp_max = g_float_temp;
  }

}

//Récupération de la luminosité et stockage de celle-ci dans la variable globale g_float_lumi
//Calcul de la moyenne des 5 dernières mesures et stockage de celle-ci dands le tableau g_float_tab_luminositemoy[0]
//Calcul de la tendance, en comparant les 2 dernières moyennes, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_luminosite
void recupLuminosite(void){
  uint16_t l_ui16_resultatConv;
  float l_float_sum = 0;
  float l_float_ratio=1;

  //initialisation du CAN sur le port A1
  InitCan(PIN_LUMINOSITE);

  //Conversion AN
  l_ui16_resultatConv = ConvAn();

  //Conversion du résultat du CAN en lux
  g_float_luminosite = 1/(l_ui16_resultatConv*0.00488);

  //décalage des anciennes luminosités
  g_float_tab_luminosite[4]=g_float_tab_luminosite[3];
  g_float_tab_luminosite[3]=g_float_tab_luminosite[2];
  g_float_tab_luminosite[2]=g_float_tab_luminosite[1];
  g_float_tab_luminosite[1]=g_float_tab_luminosite[0];
  g_float_tab_luminosite[0]=g_float_luminosite;

  //additionne les 5 dernières luminosités
  for(int iBcl=0;iBcl<5;iBcl++){
    l_float_sum += g_float_tab_luminosite[iBcl];
  }

  g_float_tab_luminositemoy[1] = g_float_tab_luminositemoy[0];   //décalge de la moyenne précédente
  g_float_tab_luminositemoy[0] = l_float_sum/5;   //stockage de la moyenne

  l_float_ratio = g_float_tab_luminositemoy[1]/g_float_tab_luminositemoy[0];  //Calcul du rapport entre les 2 moyennes précédentes

  //Si le ratio est aux alentours de 1, alors tendance stable, si ratio supérieur, alors tendance à la baisse, si ratio inférieur, alors tendance à la hausse
  if (l_float_ratio>1.02){
    //tendance à la baisse
    g_tendance_luminosite=BAISSE;
  }

  else if (l_float_ratio<0.98){
    //tendance à la hausse
    g_tendance_luminosite=HAUSSE;
  }

  else{
    //tendance stable
    g_tendance_luminosite=STABLE;
  }

}

//Récupération de l'intensité lumineuse et stockage de celle-ci dans la variable globale g_float_intensitelum
void recupIntensiteLumineuse(void){
  uint16_t l_ui16_resultatConv;
  float l_float_sum = 0;
  float l_float_ratio=1;

  //initialisation du CAN sur le port A1
  InitCan(PIN_INTENSITE_LUMINEUSE);

  //Conversion AN
  l_ui16_resultatConv = ConvAn();

  //Conversion du résultat du CAN en lux
  g_float_intensitelum = (l_ui16_resultatConv*0.00488);

  //décalage des anciennes intensités lumineuses
  g_float_tab_intensitelum[4]=g_float_tab_intensitelum[3];
  g_float_tab_intensitelum[3]=g_float_tab_intensitelum[2];
  g_float_tab_intensitelum[2]=g_float_tab_intensitelum[1];
  g_float_tab_intensitelum[1]=g_float_tab_intensitelum[0];
  g_float_tab_intensitelum[0]=g_float_intensitelum;

  //additionne les 5 dernières intensités lumineuses
  for(int iBcl=0;iBcl<5;iBcl++){
    l_float_sum += g_float_tab_intensitelum[iBcl];
  }

  g_float_tab_intensitelummoy[1] = g_float_tab_intensitelummoy[0];   //décalge de la moyenne précédente
  g_float_tab_intensitelummoy[0] = l_float_sum/5;   //stockage de la moyenne

  l_float_ratio = g_float_tab_intensitelummoy[1]/g_float_tab_intensitelummoy[0];  //Calcul du rapport entre les 2 moyennes précédentes

  //Si le ratio est aux alentours de 1, alors tendance stable, si ratio supérieur, alors tendance à la baisse, si ratio inférieur, alors tendance à la hausse
  if (l_float_ratio>1.02){
    //tendance à la baisse
    g_tendance_intensitelum=BAISSE;
  }

  else if (l_float_ratio<0.98){
    //tendance à la hausse
    g_tendance_intensitelum=HAUSSE;
  }

  else{
    //tendance stable
    g_tendance_intensitelum=STABLE;
  }
}

void recupVitesseVent(void){
  uint16_t l_ui16_risingEdgeCounter = 0; //Init Variable compteur de fronts montants
  uint16_t l_ui16_temps = millis(); //heure
  float l_float_vitesseVent;

  //Actualisation non-bloquante chaque seconde
  if(g_ui16_time_start+1000 <= l_ui16_temps) {
    //phase acquisition
    //l_ui16_temps = millis() - g_ui16_time_start;
    l_ui16_risingEdgeCounter = TCNT1;
    l_float_vitesseVent = l_ui16_risingEdgeCounter*(0.16);

    //renvoi des datas ( ICI CONVERTIR ENSUITE EN MPH )
    Serial.println(l_float_vitesseVent);

    //phase reset
    TCNT1=0;
    g_ui16_time_start = l_ui16_temps; //le temps de depart est desormais le temps auquel cette mesure a ete effectuee

  }
}
