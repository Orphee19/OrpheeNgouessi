const db = admin.firestore();

// Liste commandes pour vendeur (mobile)
exports.listOrdersForSeller = async (req, res) => {
  try {
    const sellerId = req.user.uid;
    const ordersSnapshot = await db.collection('orders').where('sellerId', '==', sellerId).get();
    const orders = ordersSnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
    return res.status(200).json(orders);
  } catch (error) {
    return res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Détails d'une commande (mobile)
exports.getOrderDetails = async (req, res) => {
  try {
    const orderId = req.params.orderId;
    const orderDoc = await db.collection('orders').doc(orderId).get();
    if (!orderDoc.exists) {
      return res.status(404).json({ message: 'Commande non trouvée' });
    }
    return res.status(200).json({ id: orderDoc.id, ...orderDoc.data() });
  } catch (error) {
    return res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};
