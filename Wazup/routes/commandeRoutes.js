const router = express.Router();
const {
  createOrder,
  getOrdersByClient,
  getOrdersBySeller,
  updateOrderStatus
} = require('../controllers/commandeController');
const { authenticateUser, isClient, isSeller, isAdmin } = require('../middlewares/authMiddleware');

router.post('/', authenticateUser, isClient, createOrder);
router.get('/client', authenticateUser, isClient, getOrdersByClient);
router.get('/seller', authenticateUser, isSeller, getOrdersBySeller);
router.put('/:orderId/status', authenticateUser, (req, res, next) => {
  // Seulement vendeur ou admin peuvent changer statut
  if (req.user.role === 'vendor' || req.user.role === 'admin') {
    next();
  } else {
    res.status(403).json({ message: 'Accès refusé' });
  }
}, updateOrderStatus);

module.exports = router;
