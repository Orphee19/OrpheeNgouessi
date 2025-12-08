// TODO: Implement vendeurWebRoutes.jsconst express = require('express');
const router = express.Router();
const {
  addProduct,
  updateProduct,
  deleteProduct,
  getProductsBySeller
} = require('../controllers/vendeurWebController');
const { authenticateUser, isSeller } = require('../middlewares/authMiddleware');

router.post('/products', authenticateUser, isSeller, addProduct);
router.put('/products/:productId', authenticateUser, isSeller, updateProduct);
router.delete('/products/:productId', authenticateUser, isSeller, deleteProduct);
router.get('/products', authenticateUser, isSeller, getProductsBySeller);

module.exports = router;
