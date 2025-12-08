// TODO: Implement Client.jsconst db = require('../firebase');

const Client = {
  create: async (clientData) => {
    return await db.collection('clients').add(clientData);
  },
  getAll: async () => {
    const snapshot = await db.collection('clients').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('clients').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('clients').doc(id).delete();
  }
};

module.exports = Client;
