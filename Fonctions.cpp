#pragma once
#include "StationMeteo.h"

//Variables globales :
extern uint16_t g_ui16_time_start;   //Heure de départ du programme
extern TFT TFTscreen;

//char temperatures
extern char temp_actu[];
extern char temp_min[];
extern char temp_max[];
extern char temp_moy[];

//char luminosites
extern char lumi_actu[];
extern char lumi_min[];
extern char lumi_max[];
extern char lumi_moy[];

//char intensites lumineuses
extern char intlum_actu[];
extern char intlum_min[];
extern char intlum_max[];
extern char intlum_moy[];

//char vent
extern char vent_actu[];
extern char vent_min[];
extern char vent_max[];
extern char vent_moy[];

// Variables – Température
extern float g_float_temp;   //Température actuelle
extern float g_float_tab_temp[];   //5 dernières températures
extern float g_float_tempmoy;    //moyenne de 5 températures 
extern float g_float_temp_min;    //température minimale depuis le début
extern float g_float_temp_max;    //température maximale depuis le début
extern T_Tendance g_tendance_temp;   //tendance de la température (Stable / Hausse / Baisse)

// Variables – Luminosité
extern float g_float_luminosite;   //Luminosité actuelle
extern float g_float_tab_luminosite[];    //5 dernières luminosités
extern float g_float_luminositemoy;     //moyenne des 5 dernières luminosités
extern float g_float_luminosite_min;
extern float g_float_luminosite_max;
extern T_Tendance g_tendance_luminosite;    //tendance de la luminosité (Stable / Hausse / Baisse)

// Variables – Intensité lumineuse
extern float g_float_intensitelum;    //Intensité lumineuse actuelle
extern float g_float_tab_intensitelum[];   //5 dernières intensités lumineuse
extern float g_float_intensitelum_moy;    
extern float g_float_intensitelum_min;
extern float g_float_intensitelum_max;
extern T_Tendance g_tendance_intensitelum;    //tendance des intensités lumineuses (Stable / Hausse / Baisse)

// Variables – Vent
extern float g_float_vitesse_vent;    //Vitesse du vent actuelle
extern float g_float_tab_vitesse_vent[];   //5 dernières vitesses du vent
extern float g_float_vitesse_vent_moy;    //moyenne des 5 dernières vitesses du vent
extern float g_float_vitesse_vent_min;
extern float g_float_vitesse_vent_max;
extern T_Tendance g_tendance_vitesse_vent;    //tendance des vitesses du vent (Stable / Hausse / Baisse)


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
//Calcul de la moyenne des 5 dernières mesures, actualisation des minimales et maximales
//Calcul de la tendance, en comparant les 2 dernières mesures, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_temp
void recupTemperature(void){
  uint16_t l_ui16_resultatConv;
  float l_float_sum = 0;
  float l_float_delta = 0;

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

  g_float_tempmoy = l_float_sum/5;   //stockage de la moyenne

  l_float_delta = g_float_tab_temp[0]-g_float_tab_temp[1];   //Calcul de la différence entre la température actuelle et la température précédente

  //Si le delta est inférieur à -0.1, alors la tendance est à la baisse
  if (l_float_delta<-0.1){
    //tendance à la baisse
    g_tendance_temp=BAISSE;
  }

  //Si le delta est supérieur à 0.1, alors la tendance est à la hausse
  else if (l_float_delta>0.1){
    //tendance à la hausse
    g_tendance_temp=HAUSSE;
  }

  //Sinon la tendance est stable
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
//Calcul de la moyenne des 5 dernières mesures, actualisation des minimales et maximales
//Calcul de la tendance, en comparant les 2 dernières mesures, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_luminosite
void recupLuminosite(void){
  uint16_t l_ui16_resultatConv;
  float l_float_sum = 0;
  float l_float_delta = 0;

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

  g_float_luminositemoy = l_float_sum/5;   //stockage de la moyenne

  l_float_delta = g_float_tab_luminosite[0]-g_float_tab_luminosite[1];

  //Si le ratio est aux alentours de 1, alors tendance stable, si ratio supérieur, alors tendance à la baisse, si ratio inférieur, alors tendance à la hausse
  if (l_float_delta>0.1){
    //tendance à la baisse
    g_tendance_luminosite=BAISSE;
  }

  else if (l_float_delta<-0.1){
    //tendance à la hausse
    g_tendance_luminosite=HAUSSE;
  }

  else{
    //tendance stable
    g_tendance_luminosite=STABLE;
  }

  //actualisation de la luminosité minimale si elle est dépassée
  if (g_float_luminosite<g_float_luminosite_min){
    g_float_luminosite_min = g_float_luminosite;
  }

  //actualisation de la luminosité maximale si elle est dépassée
  if (g_float_luminosite>g_float_luminosite_max){
    g_float_luminosite_max = g_float_luminosite;
  }

}

//Récupération de l'intensité lumineuse et stockage de celle-ci dans la variable globale g_float_intensitelum
//Calcul de la moyenne des 5 dernières mesures, actualisation des minimales et maximales
//Calcul de la tendance, en comparant les 2 dernières mesures, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_intensitelum
void recupIntensiteLumineuse(void){
  uint16_t l_ui16_resultatConv;
  float l_float_sum = 0;
  float l_float_delta=0;

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

  g_float_intensitelum_moy = l_float_sum/5;   //stockage de la moyenne

  l_float_delta = g_float_tab_intensitelum[1]-g_float_tab_intensitelum[0];  //Calcul du delta entre les 2 mesures précédentes

  //Si le delta est inférieur à -0.1, alors la tendance est à la baisse
  if (l_float_delta>0.01){
    //tendance à la baisse
    g_tendance_intensitelum=BAISSE;
  }

  //Si le delta est supérieur à 0.1, alors la tendance est à la hausse
  else if (l_float_delta<-0.01){
    //tendance à la hausse
    g_tendance_intensitelum=HAUSSE;
  }

  //Sinon la tendance est stable
  else{
    //tendance stable
    g_tendance_intensitelum=STABLE;
  }

  //actualisation de l'intensite lumineuse minimale si elle est dépassée
  if (g_float_intensitelum<g_float_intensitelum_min){
    g_float_intensitelum_min = g_float_intensitelum;
  }

  //actualisation de l'intensite lumineuse maximale si elle est dépassée
  if (g_float_intensitelum>g_float_intensitelum_max){
    g_float_intensitelum_max = g_float_intensitelum;
  }
}

//Récupération de la vitesse du vent et stockage de celle-ci dans la variable globale g_float_vitesse_vent
//Calcul de la moyenne des 5 dernières mesures, actualisation des minimales et maximales
//Calcul de la tendance, en comparant les 2 dernières mesures, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_vitesse_vent
void recupVitesseVent(void){
  uint16_t l_ui16_risingEdgeCounter = 0; //Init Variable compteur de fronts montants
  uint16_t l_ui16_temps = millis(); //heure
  float l_float_sum;
  float l_float_delta;

  //Actualisation non-bloquante chaque seconde
  if(g_ui16_time_start+1000 <= l_ui16_temps) {
    //phase acquisition
    //l_ui16_temps = millis() - g_ui16_time_start;
    l_ui16_risingEdgeCounter = TCNT1;
    g_float_vitesse_vent = l_ui16_risingEdgeCounter*(0.16);

    //décalage des anciennes vitesses
    g_float_tab_vitesse_vent[4]=g_float_tab_vitesse_vent[3];
    g_float_tab_vitesse_vent[3]=g_float_tab_vitesse_vent[2];
    g_float_tab_vitesse_vent[2]=g_float_tab_vitesse_vent[1];
    g_float_tab_vitesse_vent[1]=g_float_tab_vitesse_vent[0];
    g_float_tab_vitesse_vent[0]=g_float_vitesse_vent;

    //additionne les 5 dernières vitesses
    for(int iBcl=0;iBcl<5;iBcl++){
      l_float_sum += g_float_tab_vitesse_vent[iBcl];
    }

    g_float_vitesse_vent_moy = l_float_sum/5;   //stockage de la moyenne

    l_float_delta = g_float_tab_vitesse_vent[1]-g_float_tab_vitesse_vent[0];  //Calcul du delta entre les 2 mesures précédentes

    //Si le delta est inférieur à -0.1, alors la tendance est à la baisse
    if (l_float_delta>0.01){
      //tendance à la baisse
      g_tendance_vitesse_vent=BAISSE;
    }

    //Si le delta est supérieur à 0.1, alors la tendance est à la hausse
    else if (l_float_delta<-0.01){
      //tendance à la hausse
      g_tendance_vitesse_vent=HAUSSE;
    }

    //Sinon la tendance est stable
    else{
      //tendance stable
      g_tendance_vitesse_vent=STABLE;
    }

    //actualisation de la vitesse minimale si elle est dépassée
    if (g_float_vitesse_vent<g_float_vitesse_vent_min){
      g_float_vitesse_vent_min = g_float_vitesse_vent;
    }

    //actualisation de la vitesse maximale si elle est dépassée
    if (g_float_vitesse_vent>g_float_vitesse_vent_max){
      g_float_vitesse_vent_max = g_float_vitesse_vent;
    }

    //phase reset
    TCNT1=0;
    g_ui16_time_start = l_ui16_temps; //le temps de depart est desormais le temps auquel cette mesure a ete effectuee

  }

}

//Affichage des Températures (Actuelle, Moyenne, Minimale, Maximale et Tendance) dans le moniteur série
void afficheTemperature(void){
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

    
    String str_temp_actu=String(g_float_temp);
    String str_temp_min=String(g_float_temp_min);
    String str_temp_max=String(g_float_temp_max);
    String str_temp_moy=String(g_float_tempmoy);
    str_temp_actu.toCharArray(temp_actu, 8);
    str_temp_min.toCharArray(temp_min, 8);
    str_temp_max.toCharArray(temp_max, 8);
    str_temp_max.toCharArray(temp_moy, 8);
    
    TFTscreen.setTextSize(2);
    TFTscreen.stroke(128,0,128);
    TFTscreen.background(0, 0, 0);
    TFTscreen.text("Temperatures :", 0, 0);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Actu :", 0, LINE_GAP);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(temp_actu, 80, LINE_GAP);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Min :", 0, LINE_GAP*2);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(temp_min, 80, LINE_GAP*2);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Max :", 0, LINE_GAP*3);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(temp_max, 80, LINE_GAP*3);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Moy :", 0, LINE_GAP*4);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(temp_moy, 80, LINE_GAP*4);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Tendance :", 0, LINE_GAP*5);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text("UP", 128, LINE_GAP*5);
    delay(50);
    
}

//Affichage des Luminosités (Actuelle, Moyenne, Minimale, Maximale et Tendance) dans le moniteur série
void afficheLuminosite(void){
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


    String str_lumi_actu=String(g_float_luminosite);
    String str_lumi_min=String(g_float_luminosite_min);
    String str_lumi_max=String(g_float_luminosite_max);
    String str_lumi_moy=String(g_float_luminositemoy);
    str_lumi_actu.toCharArray(lumi_actu, 8);
    str_lumi_min.toCharArray(lumi_min, 8);
    str_lumi_max.toCharArray(lumi_max, 8);
    str_lumi_max.toCharArray(lumi_moy, 8);

    TFTscreen.setTextSize(2);
    TFTscreen.stroke(128,0,128);
    TFTscreen.background(0, 0, 0);
    TFTscreen.text("Luminosites :", 0, 0);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Actu :", 0, LINE_GAP);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(lumi_actu, 80, LINE_GAP);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Min :", 0, LINE_GAP*2);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(lumi_min, 80, LINE_GAP*2);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Max :", 0, LINE_GAP*3);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(lumi_max, 80, LINE_GAP*3);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Moy :", 0, LINE_GAP*4);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(lumi_moy, 80, LINE_GAP*4);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Tendance :", 0, LINE_GAP*5);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text("UP", 128, LINE_GAP*5);
    delay(50);
}

//Affichage des Intensités lumineuses (Actuelle, Moyenne, Minimale, Maximale et Tendance) dans le moniteur série
void afficheIntensiteLumineuse(void){
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

    String str_intlum_actu=String(g_float_intensitelum);
    String str_intlum_min=String(g_float_intensitelum_min);
    String str_intlum_max=String(g_float_intensitelum_max);
    String str_intlum_moy=String(g_float_intensitelum_moy);
    str_intlum_actu.toCharArray(intlum_actu, 8);
    str_intlum_min.toCharArray(intlum_min, 8);
    str_intlum_max.toCharArray(intlum_max, 8);
    str_intlum_max.toCharArray(intlum_moy, 8);

    TFTscreen.setTextSize(2);
    TFTscreen.stroke(128,0,128);
    TFTscreen.background(0, 0, 0);
    TFTscreen.text("Intensites Lum :", 0, 0);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Actu :", 0, LINE_GAP);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(intlum_actu, 80, LINE_GAP);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Min :", 0, LINE_GAP*2);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(intlum_min, 80, LINE_GAP*2);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Max :", 0, LINE_GAP*3);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(intlum_max, 80, LINE_GAP*3);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Moy :", 0, LINE_GAP*4);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text(intlum_moy, 80, LINE_GAP*4);
    TFTscreen.stroke(255,255,255);
    TFTscreen.text("Tendance :", 0, LINE_GAP*5);
    TFTscreen.stroke(255,0,255);
    TFTscreen.text("UP", 128, LINE_GAP*5);
    delay(50);
}

//Affichage des Vitesses du vent (Actuelle, Moyenne, Minimale, Maximale et Tendance) dans le moniteur série
void afficheVitesseVent(void){
   //Affichage des Vitesses du vent
    Serial.println("Vitesses du Vent : ");
    Serial.print("Actuelle : ");
    Serial.println(g_float_vitesse_vent);
    Serial.print("Min : ");
    Serial.println(g_float_vitesse_vent_min);
    Serial.print("Max : ");
    Serial.println(g_float_vitesse_vent_max);
    Serial.print("Moy : ");
    Serial.println(g_float_vitesse_vent_moy);
    Serial.print("Tendance : ");
    Serial.println(g_tendance_vitesse_vent);
    Serial.println("================================================");
}
