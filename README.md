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
    
