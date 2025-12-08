# Backend E-commerce API

Ce projet backend fournit une API REST pour une application e-commerce avec gestion des utilisateurs, produits, commandes, livraisons, notifications, et location de véhicules.  
Le backend est construit avec **Node.js**, **Express**, et utilise **Firebase Firestore** comme base de données.

---

## Fonctionnalités

- Gestion des utilisateurs (clients, vendeurs, administrateurs)
- Gestion des produits (CRUD côté vendeur web)
- Gestion des commandes (client, vendeur, admin)
- Gestion des livraisons (demande, suivi, mise à jour)
- Gestion des notifications push via Firebase Cloud Messaging
- Module location de véhicules (recherche, réservation, gestion)
- Authentification sécurisée avec Firebase Auth et middleware JWT
- Gestion des rôles et permissions

---

## Structure du projet

- `/controllers` : logique métier des routes
- `/routes` : définition des routes API
- `/middlewares` : gestion des authentifications et autorisations
- `/config` : configuration express et serveur
- `/utils` : utils pour Firebase et autres
- `/firebase` : fichiers Firebase (vides, gérés par frontend)
  
---

## Prérequis

- Node.js v16+  
- Firebase projet et clés d’API configurées  
- npm ou yarn

---

## Installation

1. Cloner le dépôt

```bash
git clone <url-du-projet>
cd backend
