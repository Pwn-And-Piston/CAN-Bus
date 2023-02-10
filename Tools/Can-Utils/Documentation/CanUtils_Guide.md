# Guide - Can-Utils

## 1. Installation 

**Commande Linux à taper (Ubuntu/Debian):** 
```bash
sudo apt install can-utils 
```

## 2. Commandes

### 2.1 cangen

**cangen** permet de générer des paquets CAN factices à des fins de test. 

Pour utiliser cangen, vous devez spécifier l'interface dans laquelle la trame CAN doit être générée. 

```
cangen vcan0
```

## 2.2. candump

**candump** permet de vider ou d'enregistrer les trames. 

```
candump -l vcan0
```

**La sortie de candump :**

Lorsque vous enregistrez les trames CAN, un fichier sera créé et préfixé par candump suivi de la date. 

## 2.3. canplayer

**canplayer** permet de rejouer les fichiers enregistrées par candump. 

```
canplayer -I fichier.log
```

## 2.4. cansniffer

**cansniffer** permet de voir le changement dans le trafic CAN. Par exemple, il est utile pour voir un changement dans un octet particulier. De plus, l'option **-c** permet de voir le changement d'octet de manière colorée. 

```
cansniffer -c vcan0
```

## 2.5. cansend

**cansend** permet d'envoyer les trames CAN à une interface CAN spécifique. 

```
cansend interface frame
```
