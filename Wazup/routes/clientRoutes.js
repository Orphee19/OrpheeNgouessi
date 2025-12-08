
const router = express.Router();
const { getClientProfile, updateClientProfile } = require('../controllers/clientController');
const { authenticateUser, isClient } = require('../middlewares/authMiddleware');

router.get('/profile', authenticateUser, isClient, getClientProfile);
router.put('/profile', authenticateUser, isClient, updateClientProfile);

module.exports = router;
