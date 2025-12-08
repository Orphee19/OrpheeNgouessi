// TODO: Implement vendeurWebController.jsconst admin = require('../utils/firebase');
const db = admin.firestore();

// Ajouter un produit
exports.addProduct = async (req, res) => {
  try {
    const sellerId = req.user.uid;
    const productData = req.body;
    productData.sellerId = sellerId;
    productData.createdAt = new Date().toISOString();

    const productRef = await db.collection('products').add(productData);
    res.status(201).json({ message: 'Produit ajouté', productId: productRef.id });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Modifier un produit
exports.updateProduct = async (req, res) => {
  try {
    const productId = req.params.productId;
    const sellerId = req.user.uid;
    const updateData = req.body;

    const productRef = db.collection('products').doc(productId);
    const productDoc = await productRef.get();

    if (!productDoc.exists || productDoc.data().sellerId !== sellerId) {
      return res.status(404).json({ message: 'Produit non trouvé ou accès refusé' });
    }

    await productRef.update(updateData);
    res.status(200).json({ message: 'Produit mis à jour' });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Supprimer un produit
exports.deleteProduct = async (req, res) => {
  try {
    const productId = req.params.productId;
    const sellerId = req.user.uid;

    const productRef = db.collection('products').doc(productId);
    const productDoc = await productRef.get();

    if (!productDoc.exists || productDoc.data().sellerId !== sellerId) {
      return res.status(404).json({ message: 'Produit non trouvé ou accès refusé' });
    }

    await productRef.delete();
    res.status(200).json({ message: 'Produit supprimé' });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Voir les produits du vendeur
exports.getProductsBySeller = async (req, res) => {
  try {
    const sellerId = req.user.uid;
    const productsSnapshot = await db.collection('products').where('sellerId', '==', sellerId).get();
    const products = productsSnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
    res.status(200).json(products);
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};
