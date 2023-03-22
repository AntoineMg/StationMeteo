#include <arduino.h>

//Definition des PINS
#define PIN_TEMPERATURE 0x00 //A0 = PC0

//Definition des Masques
#define MSQ_ADC 0x40 //Masque pour accéder au bit de start conversion de l'ADC
#define MSQ_PORT0 0x01 //Masque pour accéder au Port 0

void InitCan(uint8_t x_ui8_portConv);
void recupTemperature(void);
uint16_t ConvAn(void);
