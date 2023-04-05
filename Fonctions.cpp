#pragma once
#include "StationMeteo.h"
extern float g_float_temp;
extern float g_float_luminosite;
extern uint16_t g_ui16_time_start;


//Initialisation du CAN sur le port ... (doit être situé sur le port C)
void InitCan(uint8_t x_ui8_portConv){
	ADMUX = 0x40 | x_ui8_portConv; // Configure ADMUX pour que la conversion se fasse sur le port sélectionné
	ADCSRA = 0x84; // Enable ADC and set division factor to 16
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
void recupTemperature(void){
  uint16_t l_ui16_resultatConv;
  //initialisation du CAN sur le port A0
  InitCan(PIN_TEMPERATURE);
  l_ui16_resultatConv = ConvAn();
  //Conversion du résultat CAN en température
  g_float_temp = l_ui16_resultatConv*0.488;
}

//Récupération de la luminosité et stockage de celle-ci dans la variable globale g_float_lumi
void recupLuminosite(void){
  uint16_t l_ui16_resultatConv;
  //initialisation du CAN sur le port A1
  InitCan(PIN_LUMINOSITE);
  l_ui16_resultatConv = ConvAn();
  //Conversion du résultat du CAN en lux
  g_float_luminosite = 1/(l_ui16_resultatConv*0.00488);
}

void recupIntensiteLumineuse(void){
  uint16_t l_ui16_resultatConv;
  //initialisation du CAN sur le port A1
  InitCan(PIN_INTENSITE_LUMINEUSE);
  l_ui16_resultatConv = ConvAn();
  //Conversion du résultat du CAN en lux
  Serial.println(l_ui16_resultatConv*0.00488);
}

void recupVitesseVent(void){
  uint16_t l_ui16_risingEdgeCounter = 0; //Init Variable compteur de fronts montants
  uint16_t l_ui16_temps = millis(); //heure

  if(g_ui16_time_start+1000 <= l_ui16_temps) {
    //phase acquisition
    //l_ui16_temps = millis() - g_ui16_time_start;
    l_ui16_risingEdgeCounter = TCNT1;

    //renvoi des datas ( ICI CONVERTIR ENSUITE EN MPH )
    Serial.println(l_ui16_risingEdgeCounter);

    //phase reset
    TCNT1=0;
    g_ui16_time_start = l_ui16_temps; //le temps de depart est desormais le temps auquel cette mesure a ete effectuee

  }
}
