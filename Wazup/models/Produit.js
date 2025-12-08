const db = require('../firebase');

const Produit = {
  create: async (produitData) => {
    return await db.collection('produits').add(produitData);
  },
  getAll: async () => {
    const snapshot = await db.collection('produits').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('produits').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('produits').doc(id).delete();
  }
};

module.exports = Produit;
