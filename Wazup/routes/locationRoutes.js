const express = require('express');
const router = express.Router();
const {
  searchVehicles,
  bookVehicle,
  getBookingsByClient
} = require('../controllers/locationController');
const { authenticateUser, isClient } = require('../middlewares/authMiddleware');

router.get('/search', authenticateUser, isClient, searchVehicles);
router.post('/book', authenticateUser, isClient, bookVehicle);
router.get('/bookings', authenticateUser, isClient, getBookingsByClient);

module.exports = router;
