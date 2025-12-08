// deliveryModel.js content placeholder
const db = require('../firebase');

const Delivery = {
  create: async (data) => {
    return await db.collection('deliveries').add(data);
  },
  getAll: async () => {
    const snapshot = await db.collection('deliveries').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('deliveries').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('deliveries').doc(id).delete();
  }
};

module.exports = Delivery;
sss