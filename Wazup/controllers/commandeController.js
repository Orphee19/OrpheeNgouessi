// TODO: Implement commandeController.jsconst admin = require('../utils/firebase');
const db = admin.firestore();

// Client passe une commande
exports.createOrder = async (req, res) => {
  try {
    const clientId = req.user.uid;
    const orderData = req.body;
    orderData.clientId = clientId;
    orderData.status = 'en attente';
    orderData.createdAt = new Date().toISOString();

    const orderRef = await db.collection('orders').add(orderData);
    res.status(201).json({ message: 'Commande créée', orderId: orderRef.id });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Voir les commandes d’un client
exports.getOrdersByClient = async (req, res) => {
  try {
    const clientId = req.user.uid;
    const ordersSnapshot = await db.collection('orders').where('clientId', '==', clientId).get();
    const orders = ordersSnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
    res.status(200).json(orders);
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Voir les commandes d’un vendeur
exports.getOrdersBySeller = async (req, res) => {
  try {
    const sellerId = req.user.uid;
    const ordersSnapshot = await db.collection('orders').where('sellerId', '==', sellerId).get();
    const orders = ordersSnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
    res.status(200).json(orders);
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Mettre à jour le statut d’une commande (vendeur/admin)
exports.updateOrderStatus = async (req, res) => {
  try {
    const orderId = req.params.orderId;
    const { status } = req.body;

    const orderRef = db.collection('orders').doc(orderId);
    const orderDoc = await orderRef.get();

    if (!orderDoc.exists) {
      return res.status(404).json({ message: 'Commande non trouvée' });
    }

    await orderRef.update({ status });
    res.status(200).json({ message: 'Statut de commande mis à jour' });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};
