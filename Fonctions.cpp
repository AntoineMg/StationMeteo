#pragma once
#include "StationMeteo.h"
extern float g_float_temp;



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
  InitCan(MSQ_PORT0);
  l_ui16_resultatConv = ConvAn();
  //Conversion du résultat CAN en température
  g_float_temp = (l_ui16_resultatConv*(5/1024))*100;
}
