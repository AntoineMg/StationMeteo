#include <arduino.h>

//Definition des PINS
#define PIN_TEMPERATURE 0x00 //A0 = PC0
#define PIN_LUMINOSITE 0x01 //A1 = PC1

//Definition des Masques
#define MSQ_ADC 0x40 //Masque pour accéder au bit de start conversion de l'ADC
<<<<<<< HEAD
=======
#define MSQ_PORT0 0x00 //Masque pour accéder au Port 0
>>>>>>> 3f355982d0d788a74bab3fb9aebd1bc6d2aed537

void InitCan(uint8_t x_ui8_portConv);
uint16_t ConvAn(void);

void recupTemperature(void);
void recupLuminosite(void);
