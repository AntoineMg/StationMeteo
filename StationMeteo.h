#include <arduino.h>

//Definition des PINS
#define PIN_TEMPERATURE 0x00 //A0 = PC0
#define PIN_LUMINOSITE 0x01 //A1 = PC1
#define PIN_INTENSITE_LUMINEUSE 0x02 // A2 = PC2

//Definition des Masques
#define MSQ_ADC 0x40 //Masque pour accéder au bit de start conversion de l'ADC

void InitCan(uint8_t x_ui8_portConv);
uint16_t ConvAn(void);

void recupTemperature(void);
void recupLuminosite(void);
void recupIntensiteLumineuse(void);
