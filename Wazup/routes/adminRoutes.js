const express = require('express');
const router = express.Router();
const adminController = require('../controllers/adminController');

// Exemple de route protégée admin
router.get('/dashboard', adminController.getAdminDashboard);

// Ajouter d'autres routes admin ici
// ex : router.post('/create-user', adminController.createUser);

module.exports = router;
