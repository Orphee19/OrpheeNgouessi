const adminController = {
  // Créer un nouveau vendeur
  createVendor: async (req, res) => {
    try {
      const { name, email, phone } = req.body;

      // Exemple avec Firebase
      const newVendor = {
        name,
        email,
        phone,
        role: 'vendor',
        status: 'active',
        createdAt: new Date().toISOString()
      };

      await firebase.firestore().collection('users').add(newVendor);
      res.status(201).json({ message: 'Vendeur ajouté avec succès.' });
    } catch (error) {
      res.status(500).json({ error: error.message });
    }
  },

  // Lister tous les vendeurs
  getAllVendors: async (req, res) => {
    try {
      const snapshot = await firebase.firestore().collection('users').where('role', '==', 'vendor').get();
      const vendors = snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));

      res.status(200).json(vendors);
    } catch (error) {
      res.status(500).json({ error: error.message });
    }
  },

  // Bloquer un vendeur
  blockVendor: async (req, res) => {
    try {
      const vendorId = req.params.id;

      await firebase.firestore().collection('users').doc(vendorId).update({
        status: 'blocked'
      });

      res.status(200).json({ message: 'Vendeur bloqué.' });
    } catch (error) {
      res.status(500).json({ error: error.message });
    }
  },

  // Débloquer un vendeur
  unblockVendor: async (req, res) => {
    try {
      const vendorId = req.params.id;

      await firebase.firestore().collection('users').doc(vendorId).update({
        status: 'active'
      });

      res.status(200).json({ message: 'Vendeur débloqué.' });
    } catch (error) {
      res.status(500).json({ error: error.message });
    }
  }
};

module.exports = adminController;
