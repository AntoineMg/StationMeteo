# StationMeteo
Projet de Station météo basée sur un arduino uno
Source : https://github.com/AntoineMg/StationMeteo

## Cahier des Charges
### Version 1
* Thermomètre
    * LM35DZ
        - [x] Retourne une température
        - [x] Précision au dixième de degré
        - [x] Calcul de la moyenne des 5 dernières mesures (a tester)
        - [x] Calcul de la tendance (a tester)
  
* Capteur d'intensité lumineuse
    * Cellule photovoltaïque random
        - [X] Retourne une valeur
        - [X] Valeur cohérente
        - [X] Calcul de la moyenne des 5 dernières mesures
        - [X] Calcul de la tendance
  
* Capteur de luminosité
    * Photorésistance
        - [X] Retourne une valeur
        - [X] Valeur cohérente
        - [X] Calcul de la moyenne des 5 dernières mesures
        - [X] Calcul de la tendance

* Anémomètre
    * Simulé par un GBF
        - [X] Retourne une valeur
        - [X] Valeur cohérente avec celle simulée par le GBF
        - [X] Calcul de la moyenne des 5 dernières mesures
        - [X] Calcul de la tendance

* Général
    - [X] Affichage des infos avec la moyenne et la tendance
    - [X] Mesures toutes les 3 secondes
 
## Améliorations à faire:

- [ ] Affichage tendance sur OLED
- [ ] Ameliorer conversions maths

## Quelques Fonctions :

### void InitCan(uint8_t x_ui8_portConv)
    Permets d'initialiser le CAN en vue d'une conversion sur le port x_ui8_portConv
   
### uint16_t ConvAn(void)
    Lance une conversion analogique sur le port spécifié lors de l'initialisation du CAN
    
### void recupTemperature(void)
    Permets de récupérer la température en lançant une Conversion AN sur la pin définie comme étant le pin du thermomètre (ici A0)
    Stockage de la température actuelle dans la variable globale g_float_temp
    Calcul de la moyenne des 5 dernières mesures et stockage de celle-ci dands le tableau g_float_tab_tempmoy[0]
    Calcul de la tendance, en comparant les 2 dernières moyennes, stockage de celle si dans la variable de type enum T_Tendance, g_tendance_temp

### void recupLuminosite(void)
    Permets de récupérer la luminosité en lançant une Conversion A-N sur la pin définie comme étant la pin de la photorésistance (ici A1)

### void recupIntensiteLumineuse(void)
    Permets de récupérer l'intensité lumineuse en lançant une Conversion A-N sur la pin définie comme étant la pin de la cellule photovoltaïque (ici A2)

### void recupVitesseVent(void)
    Permets de récupérer la vitesse du vent, en mesurant la fréquence sur le pin ...???? puis en convertissant cette fréquence en vitesse de vent à l'aide d'une formule [à déterminer]
    
    
