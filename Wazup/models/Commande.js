const db = require('../firebase');

const Commandes = {
  create: async (commandeData) => {
    return await db.collection('commandes').add(commandeData);
  },
  getAll: async () => {
    const snapshot = await db.collection('commandes').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('commandes').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('commandes').doc(id).delete();
  }
};

module.exports = Commandes;
