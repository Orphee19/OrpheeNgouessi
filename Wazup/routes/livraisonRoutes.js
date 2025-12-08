const express = require('express');
const router = express.Router();
const {
  createDeliveryRequest,
  getDeliveryStatus,
  updateDeliveryStatus
} = require('../controllers/livraisonController');
const { authenticateUser, isClient, isSeller, isAdmin } = require('../middlewares/authMiddleware');

router.post('/', authenticateUser, isClient, createDeliveryRequest);
router.get('/:deliveryId', authenticateUser, getDeliveryStatus);

// Mise à jour réservée au livreur ou admin (on considère ici isSeller ou isAdmin)
router.put('/:deliveryId/status', authenticateUser, (req, res, next) => {
  if (req.user.role === 'vendor' || req.user.role === 'admin') {
    next();
  } else {
    res.status(403).json({ message: 'Accès refusé' });
  }
}, updateDeliveryStatus);

module.exports = router;
