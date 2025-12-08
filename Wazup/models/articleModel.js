// articleModel.js content placeholder
const db = require('../firebase');

const Articles = {
  create: async (data) => {
    return await db.collection('articles').add(data);
  },
  getAll: async () => {
    const snapshot = await db.collection('articles').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('articles').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('articles').doc(id).delete();
  }
};

module.exports = Articles;
