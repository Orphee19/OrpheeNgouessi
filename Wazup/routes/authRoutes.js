const express = require('express');
const router = express.Router();
const { register, resetPassword } = require('../controllers/authController');
const { validateRegister } = require('../utils/validators');

router.post('/register', validateRegister, register);
router.post('/reset-password', resetPassword);

module.exports = router;
