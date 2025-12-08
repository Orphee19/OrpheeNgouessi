# CarflowServer

**Serveur backend pour le projet Car Flow**  
Projet collaboratif visant à gérer le transport urbain (bus et taxi) avec un système de cartes UID pour l’abonnement, le paiement et l’identification des usagers.

---

##  Description

`CarflowServer` est le serveur principal du projet **Car Flow**, permettant :

- La gestion des abonnements des usagers via carte UID  
- La gestion des paiements et validations d’accès aux transports urbains  
- Le suivi et la géolocalisation en temps réel du trafic bus et taxi  
- La communication sécurisée avec les applications clients et dispositifs embarqués  

Le serveur fournit des **API REST** pour gérer les données des usagers, des trajets et des véhicules.

---

##  Fonctionnalités principales

- **Gestion des usagers :** création, modification, suppression et identification via carte UID  
- **Gestion des abonnements et paiements** : suivi des transactions, validation des accès  
- **Suivi du trafic en temps réel** : géolocalisation des bus et taxis, mise à jour des positions  
- **API REST sécurisée** pour communication avec les applications web et mobiles  
- **Gestion multi-plateforme** pour les dispositifs embarqués et les clients PC/mobile  

---

##  Technologies utilisées

- **Langage :** C++  
- **Base de données :** PostgreSQL / InfluxDb 
- **API :** RESTful  
- **Gestion réseau :** HTTP et WebSocket pour suivi temps réel  
- **Outils complémentaires :** CMake, Git, Docker (optionnel)  

---

##  Installation & Lancement

