const db = require('../firebase');

const Notification = {
  create: async (data) => {
    return await db.collection('notifications').add(data);
  },
  getAll: async () => {
    const snapshot = await db.collection('notifications').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('notifications').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('notifications').doc(id).delete();
  }
};

module.exports = Notification;
