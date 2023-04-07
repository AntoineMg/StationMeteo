# StationMeteo
Projet de Station météo basée sur un arduino uno

## Cahier des Charges
### Version 1
* Thermomètre
    * LM35DZ
        - [x] Retourne une température
        - [x] Précision au dixième de degré
  
* Capteur d'intensité lumineuse
    * Cellule photovoltaïque random
        - [X] Retourne une valeur
        - [ ] Valeur cohérente
  
* Capteur de luminosité
    * Photorésistance
        - [X] Retourne une valeur
        - [ ] Valeur cohérente

* Anémomètre
    * Simulé par un GBF
        - [X] Retourne une valeur
        - [X] Valeur cohérente avec celle simulée par le GBF
  
 
## Tâches à effectuer :

- [ ] trouver la bonne formule pour convertir la luminosité
- [ ] Rendre toutes les fonctions non bloquantes

## Fonctions :

### void InitCan(uint8_t x_ui8_portConv)
    Permets d'initialiser le CAN en vue d'une conversion sur le port x_ui8_portConv
   
### uint16_t ConvAn(void)
    Lance une conversion analogique sur le port spécifié lors de l'initialisation du CAN
    
### void recupTemperature(void)
    Permets de récupérer la température en lançant une Conversion AN sur la pin définie comme étant le pin du thermomètre (ici A0)

### void recupLuminosite(void)
    Permets de récupérer la luminosité en lançant une Conversion A-N sur la pin définie comme étant la pin de la photorésistance (ici A1)

### void recupIntensiteLumineuse(void)
    Permets de récupérer l'intensité lumineuse en lançant une Conversion A-N sur la pin définie comme étant la pin de la cellule photovoltaïque (ici A2)

### void recupVitesseVent(void)
    Permets de récupérer la vitesse du vent, en mesurant la fréquence sur le pin ...???? puis en convertissant cette fréquence en vitesse de vent à l'aide d'une formule [à déterminer]
    
    
