const db = require('../firebase');

const Livraison = {
  create: async (data) => {
    return await db.collection('livraisons').add(data);
  },
  getAll: async () => {
    const snapshot = await db.collection('livraisons').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('livraisons').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('livraisons').doc(id).delete();
  }
};

module.exports = Livraison;
