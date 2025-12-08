const admin = require('../utils/firebase');
const db = admin.firestore();

// Demande de livraison
exports.createDeliveryRequest = async (req, res) => {
  try {
    const userId = req.user.uid;
    const deliveryData = req.body;
    deliveryData.requesterId = userId;
    deliveryData.status = 'en attente';
    deliveryData.createdAt = new Date().toISOString();

    const deliveryRef = await db.collection('deliveries').add(deliveryData);
    res.status(201).json({ message: 'Demande de livraison créée', deliveryId: deliveryRef.id });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Suivi livraison (client ou vendeur)
exports.getDeliveryStatus = async (req, res) => {
  try {
    const deliveryId = req.params.deliveryId;
    const deliveryDoc = await db.collection('deliveries').doc(deliveryId).get();

    if (!deliveryDoc.exists) {
      return res.status(404).json({ message: 'Livraison non trouvée' });
    }

    res.status(200).json({ id: deliveryDoc.id, ...deliveryDoc.data() });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Mise à jour statut livraison (livreur, admin)
exports.updateDeliveryStatus = async (req, res) => {
  try {
    const deliveryId = req.params.deliveryId;
    const { status } = req.body;

    const deliveryRef = db.collection('deliveries').doc(deliveryId);
    const deliveryDoc = await deliveryRef.get();

    if (!deliveryDoc.exists) {
      return res.status(404).json({ message: 'Livraison non trouvée' });
    }

    await deliveryRef.update({ status });
    res.status(200).json({ message: 'Statut livraison mis à jour' });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};
