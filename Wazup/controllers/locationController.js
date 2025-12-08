const admin = require('../utils/firebase');
const db = admin.firestore();

// Rechercher des véhicules disponibles
exports.searchVehicles = async (req, res) => {
  try {
    const { type, startDate, endDate, location } = req.query;

    let query = db.collection('vehicles').where('available', '==', true);

    if (type) {
      query = query.where('type', '==', type);
    }

    // TODO: Filtrer par date et localisation selon modèle de données

    const snapshot = await query.get();
    const vehicles = snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));

    res.status(200).json(vehicles);
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Réserver un véhicule
exports.bookVehicle = async (req, res) => {
  try {
    const clientId = req.user.uid;
    const bookingData = req.body;
    bookingData.clientId = clientId;
    bookingData.status = 'réservé';
    bookingData.createdAt = new Date().toISOString();

    const bookingRef = await db.collection('vehicleBookings').add(bookingData);
    res.status(201).json({ message: 'Véhicule réservé', bookingId: bookingRef.id });
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};

// Consulter réservations client
exports.getBookingsByClient = async (req, res) => {
  try {
    const clientId = req.user.uid;
    const bookingsSnapshot = await db.collection('vehicleBookings').where('clientId', '==', clientId).get();
    const bookings = bookingsSnapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));

    res.status(200).json(bookings);
  } catch (error) {
    res.status(500).json({ message: 'Erreur serveur', error: error.message });
  }
};
