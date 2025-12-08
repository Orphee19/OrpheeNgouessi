# I-LOG_SERVER

**Serveur backend pour l’application I-LOG**  
Projet collaboratif destiné à gérer les ressources humaines (RH) et la gestion de stock sur PC et appareils mobiles.

---

##  Description

I-LOG_SERVER est le serveur principal de l’application **I-LOG**, permettant :  

- La gestion des employés et des ressources humaines  
- La gestion des stocks et des commandes  
- L’accès simultané depuis PC et mobile  
- La sécurisation des données et des transactions  

Le serveur fournit des **API REST** robustes pour la communication avec les clients (applications front-end) et la base de données.

---

##  Fonctionnalités principales

- **Gestion RH :** ajout, modification, suppression d’employés, suivi des départements et postes  
- **Gestion Stock :** ajout et suivi des produits, gestion des commandes, inventaire en temps réel  
- **API REST sécurisée** pour communication avec les applications clients  
- **Multi-plateforme :** compatible PC et mobile  
- **Authentification et rôles utilisateurs** pour sécuriser l’accès aux données  

---

## Technologies utilisées

- **Langage :** C++  
- **Base de données :** PostgreSQL / InfluxDb (à configurer selon le projet)  
- **API :** RESTful  
- **Gestion réseau :** HTTP et WebSocket (si nécessaire pour la synchronisation temps réel)  
- **Outils complémentaires :** CMake, Git, Docker (optionnel)  

---

## Installation & Lancement

1. Cloner le projet :

```bash
git clone https://github.com/owner/I-LOG_SERVER.git
cd I-LOG_SERVER
