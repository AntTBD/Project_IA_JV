## Membres du groupe:
## Made by : Team Claude 3000 !
<img src="Images/TeamClaud3000.jpg" height="170" align="left">
<br>
- Aurélien CHEVALLEREAU CHEA22100006<br>
- Nicolas DEBEAULIEU DEBN28109909<br>
- Antoine TABARAUD TABA29049900<br>
<br>
Eté 2022, UQAC, CA
<br>
<br>

___

# Code source devoir 1:
## Nouvelles fonctionnalités:
- ajout de Claudius le Drunkman
- une classe DialogApp qui s'occupe des threads (3 pour les entités et 1 pour la synchronisation)
- une classe MessagesManagerClass instanciée sous le nom MessagesManager qui s'occupe de manager les messages et de les afficher (via le thread de synchronisation)


## Exécutable :
Buckland_Chapter2-State Machines\WestWorldWithMessaging\Debug\WestWorldWithMessaging.exe

## Illustrations :

| Diagramme d'état transition Miner | Diagramme d'état transition DrunkMan |
| :---: | :---: |
| ![image](Images/Tp1_diag_etat_trans_miner.png) | ![image](Images/Tp1_diag_etat_trans_drunkMan.png) |


|  | Dialogue |  |
| :---: | :---: | :---: |
| ![image](Images/Tp1_dialog1.png) | ![image](Images/Tp1_dialog2.png) | ![image](Images/Tp1_dialog3.png) |


# Code source devoir 2:
## Nouvelles fonctionnalités:
- Ajout d'un agent leader et 20 agents poursuiveurs
- Ajout de plusieurs type de formations: *(interchangeable via le menu "Poursuiveurs" de l'interface)*
    - wandering (par défaut)
    - en Ligne
    - en Cercle
    - en V
    - en Cercles multiples
- Possibilité de controler le leader avec les touches ZQSD ou WASD (wandering par défaut) *(interchangeable via le menu "Leader" de l'interface)*
- Changement de la couleur du leader : rouge
- Changement de la couleur des agents poursuiveurs : bleu
- Ajout d'aides supplémentaires dans l'affichage des contrôles pour la vitesse du leader et de direction
- Changement de la méthode de calcul par défaut : weighted sum
- Changement du titre de la fenêtre d'exécution
- *(undefine SHOAL dans le code)*


## Exécutable :
:warning: **Attention** : s'il n'y est pas déjà, copier/coller le fichier **[params.ini](Buckland_Chapter3-Steering%20Behaviors/params.ini)** dans le dossier [Debug](Buckland_Chapter3-Steering%20Behaviors/Debug) ou [Release](Buckland_Chapter3-Steering%20Behaviors/Release) généré par VS Studio.

Buckland_Chapter3-State Machines\Debug\Steering.exe

## Illustrations :

| Formation en V | Formation en Ligne |
| :---: | :---: |
| ![image](Images/Tp2_Formation_V.png) | ![image](Images/Tp2_Formation_Ligne.png) |


| Formation en Cercle | Formation en Cercles Multiplas |
| :---: | :---: |
| ![image](Images/Tp2_Formation_Cercle.png) | ![image](Images/Tp2_Formation_MultipleCircles.png) |


| Affichage infos |
| :---: |
| ![image](Images/Tp2_Helps.png) |