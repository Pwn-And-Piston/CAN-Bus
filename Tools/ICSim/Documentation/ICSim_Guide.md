# Installation du simulateur ICSim 

## 1. Prérequis 

### 1.1. Télecharger le dépôt GitHub 

**URL :** https://github.com/zombieCraig/ICSim

**Commande Linux à taper (Ubuntu/Debian):** 
```bash
git clone https://github.com/zombieCraig/ICSim.git
```

### 1.2. Installer les bibliotèques "SDL" 

**Commande Linux à taper (Ubuntu/Debian) :** 
```bash 
apt install libsdl2-dev libsdl2-image-dev -y
```

## 2. Configuration 

### 2.1. Préparer le réseau virtuel CAN 

#### 2.1.1. Naviguer dans le répertoire *ICSim* 

**Commande Linux à taper (Ubuntu/Debian) :** 
```bash 
cd ICSim
```

#### 2.1.2. Lancer le script *setup_vcan.sh*

**Commande Linux à taper (Ubuntu/Debian) :**
```bash 
./setup.vcan.sh
```

#### 2.1.3. Vérifier l'interface *vcan0*

**Commande Linux à taper (Ubuntu/Debian) :**
```bash 
ip addr
```

**Retour Commande Linux :** 
```bash
5: vcan0: <NOARP,UP,LOWER_UP> mtu 72 qdisc noqueue state UNKNOWN group default qlen 1000 link/can 
```

### 2.2. Configurer le simulateur
#### 2.2.1. Créer le simulateur 

**Commande Linux à taper (Ubuntu/Debian) :**
```bash
make
```

#### 2.2.2. Charger le module du noyau 

**Commande Linux à taper (Ubuntu/Debian) :**
```bash
sudo modprobe vcan
```

#### 2.2.3. Vérifier que le module du noyau est bien chargé 

**Commande Linux à taper (Ubuntu/Debian) :**
```bash
lsmod | grep vcan
```

**Retour Commande Linux :**
```bash
vcan                   16384  0
```

## 3. Lancer le simulateur

### 3.1. Lancer le tableau de bord 

**Commande Linux à taper (Ubuntu/Debian) :**
```bash
./icsim vcan0
```

![[Image_Tableau_De_Bord.png]](Images/Image_Tableau_De_Bord.png)

### 3.2.  Lancer le contrôleur 

**Commande Linux à taper (Ubuntu/Debian) :**
```bash
./controls vcan0
```

![[Image_Controleur.png]](Images/Image_Controleur.png)

### 3.3. Commandes simulateur
A l'aide des combinaisons de touches ci-dessous, vous pouvez apporter des modifications dans le tableau de bord ICSim :

|                         Actions                         |                Touches                |
| :-----------------------------------------------------: | :-----------------------------------: |
|               Clignotant droit ou gauche                |        Flèche droite ou gauche        |
|   Déverrouiller la porte avant droite ou avant gauche   | Shift droite + B ou Shift droite + A  |
| Déverrouiller la porte arrière droite ou arrière gauche | Shift gauche + Y ou Shift gauche + X  |
|              Verrouiller toutes les portes              | Maintenir Shift droite + Shift gauche |
|             Déverrouiller toutes les portes             | Maintenir Shift gauche + Shift droite |