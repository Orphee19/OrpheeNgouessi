// orderModel.js content placeholder
const db = require('../firebase');

const Orders = {
  create: async (orderData) => {
    return await db.collection('orders').add(orderData);
  },
  getAll: async () => {
    const snapshot = await db.collection('orders').get();
    return snapshot.docs.map(doc => ({ id: doc.id, ...doc.data() }));
  },
  update: async (id, data) => {
    return await db.collection('orders').doc(id).update(data);
  },
  delete: async (id) => {
    return await db.collection('orders').doc(id).delete();
  }
};

module.exports = Orders;
