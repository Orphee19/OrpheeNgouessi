// rentalModel.js content placeholder
const db = require('../firebase');

const Rental = {
  create: async (rentalData) => {
    return await db.collection('rentals').add(rentalData);
  },
  getAll: async () => {
    const snapshot = await db.collection('rentals').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('rentals').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('rentals').doc(id).delete();
  }
};

module.exports = Rental;
