const router = express.Router();
const { listOrdersForSeller, getOrderDetails } = require('../controllers/vendeurMobileController');
const { authenticateUser, isSeller } = require('../middlewares/authMiddleware');

router.get('/orders', authenticateUser, isSeller, listOrdersForSeller);
router.get('/orders/:orderId', authenticateUser, isSeller, getOrderDetails);

module.exports = router;
