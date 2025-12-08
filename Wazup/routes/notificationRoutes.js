const admin = require('../utils/firebase');
const db = admin.firestore();

// Envoyer une notification push via Firebase Cloud Messaging (FCM)
exports.sendNotification = async (req, res) => {
  try {
    const { token, title, body, data } = req.body;

    if (!token || !title || !body) {
      return res.status(400).json({ message: 'Paramètres incomplets' });
    }

    const message = {
      notification: { title, body },
      data: data || {},
      token: token,
    };

    await admin.messaging().send(message);

    // Enregistrer notification dans Firestore (optionnel)
    await db.collection('notifications').add({
      token,
      title,
      body,
      data,
      createdAt: new Date().toISOString(),
    });

    res.status(200).json({ message: 'Notification envoyée' });
  } catch (error) {
    res.status(500).json({ message: 'Erreur envoi notification', error: error.message });
  }
};
