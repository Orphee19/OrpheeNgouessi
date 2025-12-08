const db = admin.firestore();

// Récupérer le profil client connecté
exports.getClientProfile = async (req, res) => {
  try {
    const uid = req.user.uid;
    const userDoc = await db.collection('users').doc(uid).get();
    if (!userDoc.exists) {
      return res.status(404).json({ message: 'Client non trouvé' });
    }
    return res.status(200).json({ id: userDoc.id, ...userDoc.data() });
  } catch (error) {
    return res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Mettre à jour le profil client
exports.updateClientProfile = async (req, res) => {
  try {
    const uid = req.user.uid;
    const updateData = req.body;
    await db.collection('users').doc(uid).update(updateData);
    return res.status(200).json({ message: 'Profil mis à jour' });
  } catch (error) {
    return res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};
