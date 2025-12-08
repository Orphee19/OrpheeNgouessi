const db = require('../firebase');

const Vendeur = {
  create: async (vendeurData) => {
    return await db.collection('vendeurs').add(vendeurData);
  },
  getAll: async () => {
    const snapshot = await db.collection('vendeurs').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('vendeurs').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('vendeurs').doc(id).delete();
  }
};

module.exports = Vendeur;
