#include <arduino.h>
#include <TFT.h>  
#include <SPI.h>

//Definition des PINS
#define PIN_TEMPERATURE 0x00 //A0 = PC0
#define PIN_LUMINOSITE 0x01 //A1 = PC1
#define PIN_INTENSITE_LUMINEUSE 0x02 // A2 = PC2
#define PIN_SCREEN_CS 10
#define PIN_SCREEN_DC 9
#define PIN_SCREEN_RST 8
#define PIN_BP 0x01 //D1 = PD1

//Definition des Masques
#define MSQ_ADC 0x40 //Masque pour accéder au bit de start conversion de l'ADC

//type indiquant la tendance d'une grandeur physique
typedef enum{STABLE, HAUSSE, BAISSE} T_Tendance;

//type indiquant l'état actuel du système
typedef enum{TEMP, ATT_LUMI, LUMI, ATT_INTLUM, INTLUM, ATT_VENT, VENT, ATT_TEMP} T_Etat;


void InitCan(uint8_t x_ui8_portConv);
uint16_t ConvAn(void);

void recupTemperature(void);
void recupLuminosite(void);
void recupIntensiteLumineuse(void);
void recupVitesseVent(void);

void afficheTemperature(void);
void afficheLuminosite(void);
void afficheIntensiteLumineuse(void);
void afficheVitesseVent(void);
